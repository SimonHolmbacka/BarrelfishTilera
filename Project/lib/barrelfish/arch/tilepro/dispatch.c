/*
 * dispatch.c
 *
 *  Created on: Nov 18, 2012
 *      Author: jackie
 */

#include <barrelfish/barrelfish.h>
#include <barrelfish/dispatch.h>
#include <barrelfish/dispatcher_arch.h>
#include <barrelfish/curdispatcher_arch.h>
#include <barrelfish/syscalls.h>
#include "threads_priv.h"
#include <arch/spr_def.h>
#include <arch/sim_def.h>

// the curly brackets define a "very long instruction word" (VLIW)
// The sp is used as pointer to go through all saved registers.
// lw = Load Word. The word at the address of sp is loaded to reg
// addi = Add Immediate Word. The 4 is added to sp and the result is put in sp
#define LOAD_SINGLE_REG(reg) "{ 					\n\t" \
                              "	lw " #reg ", sp		\n\t" \
                              "	addi sp, sp, 4 		\n\t" \
                              "} 					\n\t"

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

#define SAVE_SINGLE_REG(reg) "swadd sp, "#reg", 4	\n\t"

#define SAVE_10_REGS(x) SAVE_SINGLE_REG(r##x##0)	\
						SAVE_SINGLE_REG(r##x##1)	\
						SAVE_SINGLE_REG(r##x##2)	\
                       	SAVE_SINGLE_REG(r##x##3)	\
                       	SAVE_SINGLE_REG(r##x##4)	\
                       	SAVE_SINGLE_REG(r##x##5)	\
                       	SAVE_SINGLE_REG(r##x##6)	\
                       	SAVE_SINGLE_REG(r##x##7)	\
                       	SAVE_SINGLE_REG(r##x##8)	\
                       	SAVE_SINGLE_REG(r##x##9)

#define SAVE_r0_TO_r52 SAVE_10_REGS()	\
					   SAVE_10_REGS(1)	\
					   SAVE_10_REGS(2)	\
					   SAVE_10_REGS(3)	\
					   SAVE_10_REGS(4)	\
					   SAVE_SINGLE_REG(r50) \
					   SAVE_SINGLE_REG(r51) \
					   SAVE_SINGLE_REG(r52)

// entry points defined in assembler code entry.S
extern void run_entry(void);
extern void pagefault_entry(void);
extern void disabled_pagefault_entry(void);
extern void trap_entry(void);

void __attribute__ ((visibility ("hidden"))) disp_resume_end(void);
void __attribute__ ((visibility ("hidden"))) disp_save_end(void);

/**
 * \brief Architecture-specific dispatcher initialization
 */
void disp_arch_init(dispatcher_handle_t handle)
{
	struct dispatcher_shared_tilepro *disp_tilepro = get_dispatcher_shared_tilepro(handle);

	disp_tilepro->d.dispatcher_run = (lvaddr_t)run_entry;
	disp_tilepro->d.dispatcher_pagefault = (lvaddr_t)pagefault_entry;
	disp_tilepro->d.dispatcher_pagefault_disabled = (lvaddr_t)disabled_pagefault_entry;
	disp_tilepro->d.dispatcher_trap = (lvaddr_t)trap_entry;

	disp_tilepro->crit_pc_low = (lvaddr_t)disp_resume;
	disp_tilepro->crit_pc_high = (lvaddr_t)disp_resume_end;
}

/**
 * \brief Resume execution of a given register state
 *
 * This function resumes the execution of the given register state on the
 * current dispatcher. It may only be called while the dispatcher is disabled.
 *
 * \param disp Current dispatcher pointer
 * \param regs Register state snapshot
 */
void disp_resume(dispatcher_handle_t handle, arch_registers_state_t *archregs)
{
	// in this function, PC enters the critical section, which means the dispatcher is disabled now.
	struct dispatcher_shared_generic *disp_generic = get_dispatcher_shared_generic(handle);
	assert_disabled(disp_generic->disabled);
	assert_disabled(disp_generic->haswork);

	struct registers_tilepro* regs = archregs;

//	char tmp[50];
//	snprintf(tmp, 50, "resume pc : 0x%.8lX\n", regs->pc);
//	sys_print(tmp, 23);

	// Re-enable dispatcher
	disp_generic->disabled = 0;  // doesn't take effect while we're in disp_resume()

	// Store the pc in the EX_CONTEXT_X_0, this is where interrupt returns to.
	__insn_mtspr(SPR_EX_CONTEXT_0_0, regs->pc);
	// We want to go to Protection Level 0
	// So, store a 0 in the special registers EX_CONTEXT_X_1
	__insn_mtspr(SPR_EX_CONTEXT_0_1, 0);
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

			// jump to pc
			"iret                                  \n\t"

			: /* No output */
			: [regs] "r" (regs),
			  [pc] "r" (regs->pc)
			/* Every register is clobbered. That's the point. So, we don't tell the compiler. */
	);

	__asm volatile("disp_resume_end:");
	// after this function, PC leaves the critical section
}

/**
 * \brief Switch execution between two register states
 *
 * This function saves as much as necessary of the current register state
 * (which, when resumed will return to the caller), and switches execution
 * by resuming the given register state.  It may only be called while the
 * dispatcher is disabled.
 *
 * \param disp Current dispatcher pointer
 * \param from_regs Location to save current register state
 * \param to_regs Location from which to resume new register state
 */
void disp_switch(dispatcher_handle_t handle, arch_registers_state_t *from_state, arch_registers_state_t *to_state)
{
	sys_print("implement me!\n", 14);
	__asm volatile("ill	\n\t");
}

/**
 * \brief Save the current register state and optionally yield the CPU
 *
 * This function saves as much as necessary of the current register state
 * (which, when resumed will return to the caller), and then either
 * re-enters the thread scheduler or yields the CPU.
 * It may only be called while the dispatcher is disabled.
 *
 * \param disp Current dispatcher pointer
 * \param regs Location to save current register state
 * \param yield If true, yield CPU to kernel; otherwise re-run thread scheduler
 * \param yield_to Endpoint capability for dispatcher to which we want to yield
 */
void disp_save(dispatcher_handle_t handle, arch_registers_state_t *state, bool yield, capaddr_t yield_to)
{
	struct dispatcher_shared_generic *disp = get_dispatcher_shared_generic(handle);
	assert_disabled(disp->disabled);
	assert_disabled(curdispatcher() == handle);

	struct registers_tilepro *regs = state;
	regs->pc = (lvaddr_t)disp_save_end;  // save PC

	// here we save all the registers for current thread
	__asm volatile(
			"mtspr SYSTEM_SAVE_0_0, sp             \n\t"
			// here use sp as a pointer
			"move sp, %[regs]                      \n\t"
			SAVE_r0_TO_r52
			SAVE_SINGLE_REG(tp)
			SAVE_SINGLE_REG(lr)
			// Save r0 for short usage
			"mtspr SYSTEM_SAVE_0_1, r0             \n\t"
			// Old sp is now in r0
			"mfspr r0, SYSTEM_SAVE_0_0             \n\t"
			// Store the old sp (r0 now) in the registers (sp points to that)
			"sw sp, r0                             \n\t"
			// Restore r0, maybe needed for this function
			"mfspr r0, SYSTEM_SAVE_0_1             \n\t"
			// Restore sp, maybe needed for this function
			"mfspr sp, SYSTEM_SAVE_0_0             \n\t"
			: /* no output, we don't change the value of the register [regs], only the memory it points to */
			: [regs] "r" (regs)
	);

	if(yield) {
		sys_yield(yield_to);
		// may fail if target doesn't exist; if so, just fall through
	}
	// this code won't run if the yield succeeded

	// enter thread scheduler again
	// this doesn't return, and will call disp_yield if there's nothing to do
	thread_run_disabled(handle);

	__asm volatile("disp_save_end:");
}
