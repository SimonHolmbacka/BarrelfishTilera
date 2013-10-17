/*
 * registers.h
 *
 *  Created on: Aug 24, 2012
 *      Author: robert
 */

#ifndef ARCH_TILEPRO_BARRELFISH_REGISTERS_H
#define ARCH_TILEPRO_BARRELFISH_REGISTERS_H

#include <barrelfish_kpi/registers_arch.h>
#include <threads_priv.h>
#include <barrelfish/curdispatcher_arch.h> // XXX For curdispatcher()
#include <arch/abi.h>


static inline void registers_set_initial(arch_registers_state_t *regs,
										 struct thread *thread,
										 lvaddr_t entry,
										 lvaddr_t stack,
										 uint32_t arg1,
										 uint32_t arg2,
										 uint32_t arg3,
										 uint32_t arg4)
{
	regs->r0 = arg1;
	regs->r1 = arg2;
	regs->r2 = arg3;
	regs->r3 = arg4;
	regs->pc = entry;
	regs->sp = stack - C_ABI_SAVE_AREA_SIZE;
	regs->tp = (uint32_t)curdispatcher();
}

static inline bool registers_is_stack_invalid(struct dispatcher_generic *disp_gen, arch_registers_state_t *archregs)
{
	struct registers_tilepro *regs = archregs;

	return (regs->sp > (lvaddr_t)disp_gen->current->stack) ||
			(regs->sp <= (lvaddr_t)disp_gen->current->stack_top);
}

#endif /* ARCH_TILEPRO_BARRELFISH_REGISTERS_H */
