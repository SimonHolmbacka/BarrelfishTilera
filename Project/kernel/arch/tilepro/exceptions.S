
#include <arch/interrupts.h>
#include <mem_loc.h>

#include <arch/spr_def.h>
#include <arch/sim_def.h>

#include <asmoffsets.h>


.macro	push_on_stack reg
	swadd sp, \reg, -4
.endm

.macro	pop_from_stack reg
	lwadd \reg, sp, 4
.endm

.macro	push_on_stack_10_regs reg_start
	push_on_stack r\reg_start\()0
	push_on_stack r\reg_start\()1
	push_on_stack r\reg_start\()2
	push_on_stack r\reg_start\()3
	push_on_stack r\reg_start\()4
	push_on_stack r\reg_start\()5
	push_on_stack r\reg_start\()6
	push_on_stack r\reg_start\()7
	push_on_stack r\reg_start\()8
	push_on_stack r\reg_start\()9
.endm

.macro	pop_from_stack_10_regs reg_start
	pop_from_stack r\reg_start\()9
	pop_from_stack r\reg_start\()8
	pop_from_stack r\reg_start\()7
	pop_from_stack r\reg_start\()6
	pop_from_stack r\reg_start\()5
	pop_from_stack r\reg_start\()4
	pop_from_stack r\reg_start\()3
	pop_from_stack r\reg_start\()2
	pop_from_stack r\reg_start\()1
	pop_from_stack r\reg_start\()0
.endm

// Macro to get the base address of current dispatcher.
// @param offset, the offset of dcb's dispatcher.
// @return out, return the address of dispatcher_shared_tilepro.
.macro get_dispatcher_shared_tilepro offset, out
	// out = address of pointer to dcb_current
	moveli \out, lo16(dcb_current)
	auli \out, \out, ha16(dcb_current)
	// out = dcb_current
	lw \out, \out
	// new out = dispatcher_shared_tilepro
	add \out, \out, \offset
	lw \out, \out
.endm

// Macro to check if dispatcher is disabled.
// Conditions: disp->disabled || (disp->crit_pc_lo <= pc < disp->crit_pc_hi)
// @param disp, the base address of dispatcher_shared_tilepro.
// @param pc, current program counter.
// @param offset_disabled, OFFSETOF_DISP_DISABLED
// @param offset_crit_lo, OFFSETOF_DISP_CRIT_PC_LOW
// @param offset_crit_hi, OFFSETOF_DISP_CRIT_PC_HIGH
// @return out, if is disabled, return 1; otherwise return 0.
.macro disp_is_disabled disp, pc, offset_disabled, offset_crit_lo, offset_crit_hi, out
	// at first we are looking at the first condition
	// if disp->disabled == 1 ?
	add \out, \disp, \offset_disabled  // out = disabled area
	lw \out, \out
	// if out != 0. then go to the end of this macro, then finish this macro
	bnz \out, 0f

	// if out == 0, then we need to test the second condition
	add \out, \disp, \offset_crit_lo  // out = crtical section lower bound
	slt \out, \pc, \out  // if pc < crtical section lower bound, then out = 1
	{
		bnz \out, 0f  // then go to the end of the macro
		move \out, zero  // at the same time, zero the output
	}

	// otherwise, test if the pc is within the crtical section upper bound
	add \out, \disp, \offset_crit_hi  // out = crtical section upper bound
	// if pc < crtical section upper bound, then out = 1, otherwise, 0
	slt \out, \pc, \out
0:
.endm

/*
 * interrupt subroutine handler.
 *
 * the main task for this handler is to stack all the neccessary registers,
 * including caller-save, sp, lr, and pc.
 * the stacking order is:
 * push sp;
 * push lr;
 * push r0-r29; (caller-save)
 * push pc;
 * push ICS.
 * when the subrontine returns, pop all these register values back in reverse order.
 *
 * @param vecnum, interrupt number, defined by Tilera;
 * @param vecname, interrupt number;
 * @param c_routine, C subroutine handling function.
 */
.macro	interrupt_handler vecnum, vecname, c_routine, unused
.pushsection .rodata, "a"
int_\vecname:
	.asciz "\vecname"
.popsection
.org	(\vecnum << 8)
intvec_\vecname:
	// As we can only use 32 instruction-bundles,
	// don't do too much in this vector.
	// r0-r29 are caller-saved, we save r0-r9 here.
	// The bundles are not necessarily things which belong together,
	// they just fit together

	// Save r0 & r1, so we can use them
	mtspr SYSTEM_SAVE_1_1, r0
	mtspr SYSTEM_SAVE_1_2, r1

	// When the debugger is set to "activate on panic", it activates
	// here, for debugging.
//		movei	r0, SIM_CONTROL_PANIC
//		mtspr	SPR_SIM_CONTROL, r0

	mfspr r0, EX_CONTEXT_1_1
	// Use only the protection level part and zero out the ICS
	andi r0, r0, 3

	{
		// If PL is zero we create a new stack area
		bzt r0, 0f
		// Else (PL==1) we use the existing stack
		move r0, sp
	}
	j 1f
0:
	// Put the stack pointer into r0
	moveli	r0, lo16(MEM_KERNEL_STACK_POINTER_V)
	{
		auli	r0, r0, ha16(MEM_KERNEL_STACK_POINTER_V)
		// Get the cpu number
		mfspr	r1, SYSTEM_SAVE_1_0
	}
	// Shift by log2 of the stack size,
	// which means multiply with the stack size.
	shli	r1, r1, PER_TILE_KERNEL_STACK_LOG2
	// Grow back on stack to the stack of the cpu
	sub		r0, r0, r1
1:
	addi	r0, r0, -C_ABI_SAVE_AREA_SIZE

	// get r1 back
	mfspr r1, SYSTEM_SAVE_1_2

	// we save sp to this special register, we need to use it later
	mtspr SYSTEM_SAVE_1_2, sp

 	{
		// r0 is now the pointer to our stack, we save sp there.
 		sw		r0, sp
 		addi	r0, r0, -4
 	}
 	{
 		// We saved sp, so now we can use it as stack pointer
 		move	sp, r0
		// Get r0 & r1 back again
		mfspr	r0, SYSTEM_SAVE_1_1
	}

	// At this point, sp is saved, all other registers are "untouched"
	// and sp is the right stack pointer.
	push_on_stack lr
	// Push r0-r9
	push_on_stack_10_regs

	// We saved all, which we want to save in this function.
	{
		// Fits in 16 bits, the first arguement of interrupt subroutine
		moveli	r0, lo16(\vecnum)
		// the third arguement of interrupt subroutine, passing the fault address
		mfspr	r2, SYSTEM_SAVE_1_3
	}
	{
		// the second arguement of interrupt subroutine
		moveli	r1, lo16(int_\vecname)
	 	moveli	r9, lo16(\c_routine)
	}
	{
	 	auli	r1, r1, ha16(int_\vecname)
	 	auli	r9, r9, ha16(\c_routine)
	}

	j intr_contd
.endm

.section .rodata, "a"
msg_end_of_intr:
	.asciz "end_of_intr\n"
.Lmsg_end_of_intr:

.text
intr_contd:
	push_on_stack_10_regs 1
	push_on_stack_10_regs 2

	mfspr r29, EX_CONTEXT_1_0
	push_on_stack r29

	mfspr r29, EX_CONTEXT_1_1
	{
		// push_on_stack, but don't move the pointer forwards
		sw sp, r29
		addi sp, sp, -C_ABI_SAVE_AREA_SIZE
	}
	// after above instruction, we have already stack all the necessary registers.

	j intr_go_to_save_area

intr_go_to_save_area:
	// if we are a timer interrupt, move to the no_trap area
	// Now the kind of interrupts, which need a save_area (enabled or disabled),
	// but which should not be handled by the trap_entry of the dispatcher.

	// Timer interrupt
	addi r10, r0, -INT_TILE_TIMER
	bz r10, no_trap

	// Page fault
	addi r10, r0, -INT_DTLB_MISS
	bz r10, no_trap

	j 3f
no_trap:

	// otherwise according to Barrelfish, page fault is a special interrupt needed to be dealt with diferently.
	// When a page fault occurs, the dispatcher can be either enabled or disabled.
	// In the case of the enabled, we need to save the register state into the "enabled_save_area";
	// in the case of the disabled, we need to save the register state into the "trap_save_area".

	// save the offset of dcb's dispatcher to r12
	moveli r12, lo16(OFFSETOF_DCB_DISP)
	auli r12, r12, ha16(OFFSETOF_DCB_DISP)
	// return the dispatcher_shared tilepro base address to r11
	get_dispatcher_shared_tilepro r12, r11

	{
		// now we get the current pc, saving it to r12
		mfspr r12, EX_CONTEXT_1_0
		// save the offset of disp's disabled to r13
		moveli r13, lo16(OFFSETOF_DISP_DISABLED)
	}
	{
		auli r13, r13, ha16(OFFSETOF_DISP_DISABLED)
		// save the offset of critic section lower bound to r14
		moveli r14, lo16(OFFSETOF_DISP_CRIT_PC_LOW)
	}
	{
		auli r14, r14, ha16(OFFSETOF_DISP_CRIT_PC_LOW)
		// save the offset of critic section upper bound to r15
		moveli r15, lo16(OFFSETOF_DISP_CRIT_PC_HIGH)
	}
	auli r15, r15, ha16(OFFSETOF_DISP_CRIT_PC_HIGH)
	// determine if dispatcher is disabled, return value in r16
	// r16 = 1, if disabled
	disp_is_disabled r11, r12, r13, r14, r15, r16

	// since we know it is disabled or not,
	// then we can go to disabled_save_area or enabled_save_area

	bnz r16, 3f  // if r16 != 0, then jump to "3"

	// now r12 = OFFSETOF_DISP_ENABLED_AREA
	moveli r12, lo16(OFFSETOF_DISP_ENABLED_AREA)
	auli r12, r12, ha16(OFFSETOF_DISP_ENABLED_AREA)

	// in this case, this interrupt is a page fault, and the dispatcher is enabled
	// r11 points to enabled area
	{
		add r11, r11, r12
		j intr_save_context
	}

3:
	// if this interrupt is not a page fault, or it is a page fault but dispatcher is disable
	// then we still need to save all the register states into "trap_save_area"
	// now r13 = OFFSETOF_DISP_TRAP_AREA
	moveli r13, lo16(OFFSETOF_DISP_TRAP_AREA)
	auli r13, r13, ha16(OFFSETOF_DISP_TRAP_AREA)
	{
		add r11, r11, r13
		j intr_save_context
	}

intr_save_context:
	// XXX: now we need to save all callee-save registers and other special registers to the save-area.
	// r11 is pointing to the entry in the memory

	//save the entry address of save area to r3, as the fouth argument of subroutine
	move r3, r11

	// skip the rest of caller-save registers
	addi r11, r11, 120

	// save all callee-save registers
	swadd r11, r30, 4
	swadd r11, r31, 4
	swadd r11, r32, 4
	swadd r11, r33, 4
	swadd r11, r34, 4
	swadd r11, r35, 4
	swadd r11, r36, 4
	swadd r11, r37, 4
	swadd r11, r38, 4
	swadd r11, r39, 4
	swadd r11, r40, 4
	swadd r11, r41, 4
	swadd r11, r42, 4
	swadd r11, r43, 4
	swadd r11, r44, 4
	swadd r11, r45, 4
	swadd r11, r46, 4
	swadd r11, r47, 4
	swadd r11, r48, 4
	swadd r11, r49, 4
	swadd r11, r50, 4
	swadd r11, r51, 4
	swadd r11, r52, 4

	// save other registers
	swadd r11, tp, 4
	swadd r11, lr, 4
	mfspr r12, SYSTEM_SAVE_1_2  // return SP to r12, we previously saved it
	swadd r11, r12, 4
	mfspr r12, EX_CONTEXT_1_0  // get current pc
	sw r11, r12

	// jump to interrupt subroutine saved in r9
	jalr r9

intr_routine_ret:
	movei r29, 1
	{
		mtspr INTERRUPT_CRITICAL_SECTION, r29
		addi sp, sp, C_ABI_SAVE_AREA_SIZE
	}
	pop_from_stack r29
	mtspr EX_CONTEXT_1_1, r29
	pop_from_stack r29
	mtspr EX_CONTEXT_1_0, r29

	pop_from_stack_10_regs 2
	pop_from_stack_10_regs 1

	pop_from_stack_10_regs
	pop_from_stack lr
	// Like pop_from_stack, but afterwards the sp contains
	// the original stack pointer, because we don't need our sp anymore.
	lw sp, sp
	iret

/*
 * system call handler
 */
.macro	syscall_handler vecnum, vecname
syscall:
	.org	(\vecnum << 8)
	syscall_\vecname:
	// The assumption is that all caller-saved registers
	// are saved by the caller of the C-function
	// and nothing important is inside that function.

	// The other assumption is that we call a C-function
	// which will save all callee-saved registers.

	// When the debugger is set to "activate on panic", it activates
	// here, for debugging.
//	movei	r0, SIM_CONTROL_PANIC
//	mtspr	SPR_SIM_CONTROL, r0

	// save sp temporarily before we push sp into stack,
	// we are gonna use it later for possible dispatching
	// r28 will not be changed since then
	move r28, sp

	// Use the kernel stack
	moveli	sp, lo16(MEM_KERNEL_STACK_POINTER_V)
	{
		auli	sp, sp, ha16(MEM_KERNEL_STACK_POINTER_V)
		// Get the cpu number
		mfspr	r29, SYSTEM_SAVE_1_0
	}
	// Shift by log2 of the stack size,
	// which means multiply with the stack size.
	shli	r29, r29, PER_TILE_KERNEL_STACK_LOG2
	// Grow back on stack to the stack of the cpu
	sub		sp, sp, r29
	addi	sp, sp, -C_ABI_SAVE_AREA_SIZE


	// Just use the given sp.
	// save sp
	push_on_stack r28
	push_on_stack lr

	mfspr r29, EX_CONTEXT_1_1
	// Use only the protection level part and zero out the ICS
	andi r29, r29, 3
	push_on_stack r29

	mfspr r29, EX_CONTEXT_1_0
	// Increase pc, so the swint1 isn't done in an endless loop
	addi r29, r29, 8
	{
		// push_on_stack, but don't move the pointer forwards
		sw sp, r29
		addi sp, sp, -C_ABI_SAVE_AREA_SIZE
	}
	mtspr INTERRUPT_CRITICAL_SECTION, zero

	j syscall_go_to_save_area
.endm

syscall_go_to_save_area:
	// since our r0-r9 are used to pass arguments,
	// and r10 is used to contain the syscall number,
	// but r11-r29 are caller-save registers, so we can use them here freely.

	// save the offset of dcb's dispatcher to r12
	moveli r12, lo16(OFFSETOF_DCB_DISP)
	auli r12, r12, ha16(OFFSETOF_DCB_DISP)
	// return the dispatcher_shared tilepro base address to r11
	get_dispatcher_shared_tilepro r12, r11

	{
		// now we get the current pc, saving it to r12
		mfspr r12, EX_CONTEXT_1_0
		// save the offset of disp's disabled to r13
		moveli r13, lo16(OFFSETOF_DISP_DISABLED)
	}
	{
		auli r13, r13, ha16(OFFSETOF_DISP_DISABLED)
		// save the offset of critic section lower bound to r14
		moveli r14, lo16(OFFSETOF_DISP_CRIT_PC_LOW)
	}
	{
		auli r14, r14, ha16(OFFSETOF_DISP_CRIT_PC_LOW)
		// save the offset of critic section upper bound to r15
		moveli r15, lo16(OFFSETOF_DISP_CRIT_PC_HIGH)
	}
	auli r15, r15, ha16(OFFSETOF_DISP_CRIT_PC_HIGH)
	// determine if dispatcher is disabled, return value in r16
	// r16 = 1, if disabled
	disp_is_disabled r11, r12, r13, r14, r15, r16

	// since we know it is disabled or not,
	// then we can go to disabled_save_area or enabled_save_area

	// now r12 = OFFSETOF_DISP_ENABLED_AREA
	moveli r12, lo16(OFFSETOF_DISP_ENABLED_AREA)
	{
		auli r12, r12, ha16(OFFSETOF_DISP_ENABLED_AREA)
		// now r13 = OFFSETOF_DISP_DISABLED_AREA
		moveli r13, lo16(OFFSETOF_DISP_DISABLED_AREA)
	}
	{
		auli r13, r13, ha16(OFFSETOF_DISP_DISABLED_AREA)
		bnz r16, 1f  // if r16 != 0, then jump to "1",
	}

	// if r16 == 0, r11 points to enabled area
	{
		add r11, r11, r12
		j syscall_save_context
	}

1:
	// r11 points to disabled area
	{
		add r11, r11, r13
		j syscall_save_context
	}

syscall_save_context:
	// here we save registers to the corresponding area.
	// these include r0-r10, for syscall arguments,
	// all the callee-save registers,
	// and others, e.g. pc, sp, and so on.
	// now r11 contains the address to the corresponding area in memory.

	// temporarily save the entry address of save area to r12
	move r12, r11

	// save r0-r10
	swadd r11, r0, 4
	swadd r11, r1, 4
	swadd r11, r2, 4
	swadd r11, r3, 4
	swadd r11, r4, 4
	swadd r11, r5, 4
	swadd r11, r6, 4
	swadd r11, r7, 4
	swadd r11, r8, 4
	swadd r11, r9, 4
	sw r11, r10

	// skip the rest of caller-save registers
	addi r11, r11, 80

	// save all callee-save registers
	swadd r11, r30, 4
	swadd r11, r31, 4
	swadd r11, r32, 4
	swadd r11, r33, 4
	swadd r11, r34, 4
	swadd r11, r35, 4
	swadd r11, r36, 4
	swadd r11, r37, 4
	swadd r11, r38, 4
	swadd r11, r39, 4
	swadd r11, r40, 4
	swadd r11, r41, 4
	swadd r11, r42, 4
	swadd r11, r43, 4
	swadd r11, r44, 4
	swadd r11, r45, 4
	swadd r11, r46, 4
	swadd r11, r47, 4
	swadd r11, r48, 4
	swadd r11, r49, 4
	swadd r11, r50, 4
	swadd r11, r51, 4
	swadd r11, r52, 4

	// save other registers
	swadd r11, tp, 4
	swadd r11, lr, 4
	swadd r11, r28, 4  // this is the previous sp we stored before
	mfspr r13, EX_CONTEXT_1_0  // get current pc
	// pc goes to next instruction after this current pc to avoid endless loop after it is resumed
	addi r13, r13, 8
	sw r11, r13

	{
		// sep up the pointer argument of sys_syscall and call it
		move r0, r12
		jal sys_syscall
	}

syscall_ret:
	movei r29, 1
	{
		mtspr INTERRUPT_CRITICAL_SECTION, r29
		addi sp, sp, C_ABI_SAVE_AREA_SIZE
	}

	pop_from_stack r29
	mtspr EX_CONTEXT_1_0, r29

	pop_from_stack r29
	mtspr EX_CONTEXT_1_1, r29

	pop_from_stack lr
	// Like pop_from_stack, but afterwards the sp contains
	// the original stack pointer, because we don't need our sp anymore.
	lw sp, sp
	iret
//.endm

// interrupt table
// "handler name" "interrupt number" "interrupt name" "C subrontine" "unused parameter"
.section ".intr_pl1", "ax"
	interrupt_handler INT_ITLB_MISS, ITLB_MISS, unknown_interrupt, 1
	interrupt_handler INT_MEM_ERROR, MEM_ERROR, unknown_interrupt, 1
	interrupt_handler INT_ILL, ILL, unknown_interrupt, 1
	interrupt_handler INT_GPV, GPV, unknown_interrupt, 1
	interrupt_handler INT_SN_ACCESS, SN_ACCESS, unknown_interrupt, 1
	interrupt_handler INT_IDN_ACCESS, IDN_ACCESS, unknown_interrupt, 1
	interrupt_handler INT_UDN_ACCESS, UDN_ACCESS, unknown_interrupt, 1
	interrupt_handler INT_IDN_REFILL, IDN_REFILL, unknown_interrupt, 1
	interrupt_handler INT_UDN_REFILL, UDN_REFILL, unknown_interrupt, 1
	interrupt_handler INT_IDN_COMPLETE, IDN_COMPLETE, unknown_interrupt, 1
	interrupt_handler INT_UDN_COMPLETE, UDN_COMPLETE, unknown_interrupt, 1
	interrupt_handler INT_SWINT_3, SWINT_3, unknown_interrupt, 1
	interrupt_handler INT_SWINT_2, SWINT_2, unknown_interrupt, 1
	syscall_handler	  INT_SWINT_1, SWINT_1
	interrupt_handler INT_SWINT_0, SWINT_0, unknown_interrupt, 1
	interrupt_handler INT_UNALIGN_DATA, UNALIGN_DATA, unknown_interrupt, 1
	interrupt_handler INT_DTLB_MISS, DTLB_MISS, page_fault, 1
	interrupt_handler INT_DTLB_ACCESS, DTLB_ACCESS, unknown_interrupt, 1
	interrupt_handler INT_DMATLB_MISS, DMATLB_MISS, unknown_interrupt, 1
	interrupt_handler INT_DMATLB_ACCESS, DMATLB_ACCESS, unknown_interrupt, 1
	interrupt_handler INT_SNITLB_MISS, SNITLB_MISS, unknown_interrupt, 1
	interrupt_handler INT_SN_NOTIFY, SN_NOTIFY, unknown_interrupt, 1
	interrupt_handler INT_SN_FIREWALL, SN_FIREWALL, unknown_interrupt, 1
	interrupt_handler INT_IDN_FIREWALL, IDN_FIREWALL, unknown_interrupt, 1
	interrupt_handler INT_UDN_FIREWALL, UDN_FIREWALL, unknown_interrupt, 1
	interrupt_handler INT_TILE_TIMER, TILE_TIMER, handle_tile_timer_irq, 1
	interrupt_handler INT_IDN_TIMER, IDN_TIMER, unknown_interrupt, 1
	interrupt_handler INT_UDN_TIMER, UDN_TIMER, unknown_interrupt, 1
	interrupt_handler INT_DMA_NOTIFY, DMA_NOTIFY, unknown_interrupt, 1
	interrupt_handler INT_IDN_CA, IDN_CA, unknown_interrupt, 1
	interrupt_handler INT_UDN_CA, UDN_CA, unknown_interrupt, 1
	interrupt_handler INT_IDN_AVAIL, IDN_AVAIL, unknown_interrupt, 1
	interrupt_handler INT_UDN_AVAIL, UDN_AVAIL, unknown_interrupt, 1
	interrupt_handler INT_PERF_COUNT, PERF_COUNT, unknown_interrupt, 1
	interrupt_handler INT_INTCTRL_3, INTCTRL_3, unknown_interrupt, 1
	interrupt_handler INT_INTCTRL_2, INTCTRL_2, unknown_interrupt, 1
	interrupt_handler INT_INTCTRL_1, INTCTRL_1, unknown_interrupt, 1
	interrupt_handler INT_INTCTRL_0, INTCTRL_0, unknown_interrupt, 1
	interrupt_handler INT_BOOT_ACCESS, BOOT_ACCESS, unknown_interrupt, 1
	interrupt_handler INT_WORLD_ACCESS, WORLD_ACCESS, unknown_interrupt, 1
	interrupt_handler INT_I_ASID, I_ASID, unknown_interrupt, 1
	interrupt_handler INT_D_ASID, D_ASID, unknown_interrupt, 1
	interrupt_handler INT_DMA_ASID, DMA_ASID, unknown_interrupt, 1
	interrupt_handler INT_SNI_ASID, SNI_ASID, unknown_interrupt, 1
	interrupt_handler INT_DMA_CPL, DMA_CPL, unknown_interrupt, 1
	interrupt_handler INT_SN_CPL, SN_CPL, unknown_interrupt, 1
	interrupt_handler INT_DOUBLE_FAULT, DOUBLE_FAULT, unknown_interrupt, 1
	interrupt_handler INT_SN_STATIC_ACCESS, SN_STATIC_ACCESS, unknown_interrupt, 1
	interrupt_handler INT_AUX_PERF_COUNT, AUX_PERF_COUNT, unknown_interrupt, 1
