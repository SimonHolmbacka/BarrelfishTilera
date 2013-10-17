/*
 * registers_arch.h
 *
 *  Created on: Aug 17, 2012
 *      Author: robert
 */

#ifndef ARCH_TILEPRO_BARRELFISH_KPI_REGISTERS_ARCH_H_
#define ARCH_TILEPRO_BARRELFISH_KPI_REGISTERS_ARCH_H_

#define TILE_THREAD_REG tp

#ifndef __ASSEMBLER__

#include <stdio.h>

struct registers_tilepro {
	// general purpose registers
	uint32_t r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, r13, r14,
			r15, r16, r17, r18, r19, r20, r21, r22, r23, r24, r25, r26, r27,
			r28, r29, r30, r31, r32, r33, r34, r35, r36, r37, r38, r39, r40,
			r41, r42, r43, r44, r45, r46, r47, r48, r49, r50, r51;

	// optional frame pointer
	uint32_t r52;

	// thread-local data, according to the ABI
	uint32_t tp; // Sometimes called r53 in the doc, but tile-as would warn.

	// stack pointer and link register
	uint32_t lr, sp;

	// Fake pc
	uint32_t pc;

	/**
	 These registers are special, in the meaning that they can't be saved,
	 because sending or reading them triggers communication over the
	 static or dynamic network.
	 uint32_t sn, idn0, idn1, udn0, udn1, udn2, udn3;
	 */

	/**
	Read-only zero register
	uint32_t zero;
	*/
};
#define GDB_TILEPRO_NUM_REGS (sizeof(struct registers_tilepro) / sizeof(uint32_t))

///< Opaque handle for the register state
typedef struct registers_tilepro arch_registers_state_t;

///< Opaque handle for the FPU register state
typedef uint32_t arch_registers_fpu_state_t;

static inline void
registers_set_entry(arch_registers_state_t *regs, lvaddr_t entry)
{
	//printf("PC set to: %.8lX\n", entry);
	regs->pc = entry;
}

static inline void
registers_set_param(arch_registers_state_t *regs, uint32_t param)
{
	regs->r0 = param;
}

static inline void
registers_get_param(arch_registers_state_t *regs, uint32_t *param)
{
	*param = regs->r0;
}

static inline uint32_t
registers_get_ip(arch_registers_state_t *regs)
{
	return regs->pc;
}

static inline uint32_t
registers_get_sp(arch_registers_state_t *regs)
{
	return regs->sp;
}

#endif // __ASSEMBLER__

#endif /* ARCH_TILEPRO_BARRELFISH_KPI_REGISTERS_ARCH_H_ */
