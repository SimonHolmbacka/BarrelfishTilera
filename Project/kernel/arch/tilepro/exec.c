/*
 * exec.c
 *
 *  Created on: Sep 3, 2012
 *      Author: robert
 */

#include <assert.h>
#include <barrelfish_kpi/types.h>
#include <barrelfish_kpi/registers_arch.h>
#include <kernel.h>
#include <dispatch.h>
#include <exec.h>

#include <misc.h>

#include "hv/hypervisor.h"
#include <arch/spr_def.h>
#include <arch/sim_def.h>

static struct registers_tilepro upcall_regs;

#define STR(X) #X
#define XSTR(X) STR(X)

// the curly brackets define a "very long instruction word" (VLIW)
// The sp is used as pointer to go through all saved registers.
// lw = Load Word. The word at the address of sp is loaded to reg
// addi = Add Immediate Word. The 4 is added to sp and the result is put in sp
#define LOAD_SINGLE_REG(reg) "{ 				\n\t" \
                              "	lw "#reg", sp	\n\t" \
                              "	addi sp, sp, 4	\n\t" \
                              "} 				\n\t"

#define _LOAD_10_REGS(x) LOAD_SINGLE_REG(r##x##0) \
                          LOAD_SINGLE_REG(r##x##1) \
                          LOAD_SINGLE_REG(r##x##2) \
                          LOAD_SINGLE_REG(r##x##3) \
                          LOAD_SINGLE_REG(r##x##4) \
                          LOAD_SINGLE_REG(r##x##5) \
                          LOAD_SINGLE_REG(r##x##6) \
                          LOAD_SINGLE_REG(r##x##7) \
                          LOAD_SINGLE_REG(r##x##8) \
                          LOAD_SINGLE_REG(r##x##9)

#define LOAD_r0_TO_r52 _LOAD_10_REGS() /* empty argument to create r0-r9 */ \
                        _LOAD_10_REGS(1) \
                        _LOAD_10_REGS(2) \
                        _LOAD_10_REGS(3) \
                        _LOAD_10_REGS(4) \
                        LOAD_SINGLE_REG(r50) \
                        LOAD_SINGLE_REG(r51) \
                        LOAD_SINGLE_REG(r52)

void reboot(void)
{
	hv_restart((HV_VirtAddr)0, (HV_VirtAddr)0);
	halt();
}

static inline __attribute__((noreturn))
void do_resume(struct registers_tilepro *regs)
{
	// Store the pc in the EX_CONTEXT_X_0, this is where interrupt returns to.
	__insn_mtspr(SPR_EX_CONTEXT_1_0, regs->pc);
	// We want to go to Protection Level 0
	// So, store a 0 in the special registers EX_CONTEXT_X_1
	__insn_mtspr(SPR_EX_CONTEXT_1_1, 0);

	// This code is a little bit tricky, as we can't specify the registers, we want to use.
	// So move it to the register, we want, immediately. As all other registers have no
	// value to us, don't care about the intermediate register, chosen by the compiler.
	__asm volatile(
			// sp is our pointer to current register in the snapshot.
			"move sp, %[regs]                      \n\t"

			// Panic, for the bm debugger to enable and putting the values from
			// EX_CONTEXT_X to r0 & r1 so it is visible in the debugger.
			/*
			"movei r0, " XSTR(SIM_CONTROL_PANIC) " \n\t"
			"mtspr SIM_CONTROL, r0                 \n\t"
			"mfspr r0, EX_CONTEXT_1_0              \n\t"
			"mfspr r1, EX_CONTEXT_1_1              \n\t"
			*/

			// Load all the registers, sp is the pointer, so don't touch that one yet.
			// This section is depending on the ordering in the registers_tilepro structure.
			// sp must be the last one (pc is even more special and does not count).
			LOAD_r0_TO_r52
			LOAD_SINGLE_REG(tp)
			LOAD_SINGLE_REG(lr)
			// Load the value of sp to sp. Our pointer is gone afterwards.
			"lw sp, sp                             \n\t"
			// Interrupt return, jump to the address of EX_CONTEXT_X_0
			"iret                                  \n\t"
			: /* No output */
			: [regs] "r" (regs)
			/* Every register is clobbered. That's the point. So, we don't tell the compiler. */
	);
	tilepro_panic("Resume function error! Should not be reached!\n");
}

void resume(arch_registers_state_t *state)
{
	printf("resume(pc=0x%.8lX, sp=0x%.8lX, tp=0x%.8lX, r0=0x%.8lX)\n", state->pc, state->sp, state->tp, state->r0);
	do_resume(state);
}

void execute(lvaddr_t entry)
{
	dispatcher_handle_t handle = dcb_current->disp;
	struct dispatcher_shared_generic *disp_gen = get_dispatcher_shared_generic(handle);

	struct registers_tilepro *regs = &upcall_regs;

	regs->r0 = disp_gen->udisp;  // this is the only parameter of disp_run()
	regs->tp = disp_gen->udisp;
	regs->pc = entry;
	// we set up sp in entry.S
	//regs->sp = MEM_USER_STACK_TOP - C_ABI_SAVE_AREA_SIZE;

	//printf("execute(entry=0x%.8lX)\n", entry);
	do_resume(regs);
}

/**
 * \brief Halt processor until an interrupt arrives
 * For use in the idle loop when nothing is runnable.
 */
void wait_for_interrupt(void)
{
	__asm volatile(
			"sleep: nap        \n\t"
			"       nop        \n\t"
			"       j sleep    \n\t"
			"		jrp lr	   \n\t"
	);

	tilepro_panic("wait_for_interrupt() must not reach here!\n");
}
