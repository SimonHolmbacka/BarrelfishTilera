/**
 * \file
 * \brief Dispatcher entry points.
 */

/*
 * Copyright (c) 2007, 2008, 2009, ETH Zurich.
 * All rights reserved.
 *
 * This file is distributed under the terms in the attached LICENSE file.
 * If you do not find this file, copies can be found by writing to:
 * ETH Zurich D-INFK, Haldeneggsteig 4, CH-8092 Zurich. Attn: Systems Group.
 */

#include <barrelfish/dispatch.h>
#include <barrelfish_kpi/registers_arch.h>
#include <asmoffsets.h>

#include <arch/abi.h>
#include <arch/chip.h>

#include <arch/spr_def.h>
#include <arch/sim_def.h>

.section .rodata, "a"
run_entry_msg:
	.ascii  "run_entry\n"

.text
.globl run_entry, pagefault_entry, disabled_pagefault_entry, trap_entry

/*
 * sep up proper SP for the following dispatcher entry function
 * @param offset (in), the stack offset pre-computed in compile time, saved in asmoffsets.h
 */
.macro setup_sp offset
	// sp saves the offset of dispatcher generic
	moveli sp, lo16(OFFSETOF_DISP_GENERIC)
	auli sp, sp, ha16(OFFSETOF_DISP_GENERIC)

	// sp points to the dispatcher_generic
	add sp, sp, TILE_THREAD_REG  // dispatcher base address is saved in tp

	// then sp points to the stack top
	add sp, sp, \offset
	addi sp, sp, -C_ABI_SAVE_AREA_SIZE
.endm

//
// void run_entry(struct disp_priv* p),
// this function is to set up stack for disp_run(...)
//        
run_entry:
	// get stack offset
	moveli r29, lo16(OFFSETOF_DISP_PRIV_STACK_LIMIT)
	auli r29, r29, ha16(OFFSETOF_DISP_PRIV_STACK_LIMIT)
	// set up SP
	setup_sp r29

	// jump to C function to continue
	j disp_run

//
// void pagefault_entry(disp ptr, vaddr_t fault_addr, uintptr_t error, vaddr_t pc)
//
pagefault_entry:
	// get stack offset
	moveli r29, lo16(OFFSETOF_DISP_PRIV_STACK_LIMIT)
	auli r29, r29, ha16(OFFSETOF_DISP_PRIV_STACK_LIMIT)
	// set up SP
	setup_sp r29
	// then jump to enabled barrelfish page fault subroutine
	j disp_pagefault

//
// void disabled_pagefault_entry(disp ptr, vaddr_t fault_addr, uintptr_t error, vaddr_t pc)
//
disabled_pagefault_entry:
	// get trap stack offset
	moveli r29, lo16(OFFSETOF_DISP_PRIV_TRAP_STACK_LIMIT)
	auli r29, r29, ha16(OFFSETOF_DISP_PRIV_TRAP_STACK_LIMIT)
	// set up SP
	setup_sp r29
	// then jump to disabled barrelfish page fault subroutine
	j disp_pagefault_disabled

//
// void trap_entry(disp ptr, uintptr_t irq, uintptr_t error, vaddr_t pc)
//
trap_entry:
	// get trap stack offset
	moveli r29, lo16(OFFSETOF_DISP_PRIV_TRAP_STACK_LIMIT)
	auli r29, r29, ha16(OFFSETOF_DISP_PRIV_TRAP_STACK_LIMIT)
	// set up SP
	setup_sp r29
	// then jump to interrupt subroutine
	j disp_trap
