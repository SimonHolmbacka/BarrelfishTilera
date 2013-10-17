/*
 * Copyright (c) 2007, 2008, 2009, ETH Zurich.
 * All rights reserved.
 *
 * This file is distributed under the terms in the attached LICENSE file.
 * If you do not find this file, copies can be found by writing to:
 * ETH Zurich D-INFK, Haldeneggsteig 4, CH-8092 Zurich. Attn: Systems Group.
 */

#include <barrelfish/barrelfish.h>
#include <barrelfish/caddr.h>
#include <barrelfish/debug.h>
#include <barrelfish/dispatch.h>

#include <assert.h>

void debug_dump(arch_registers_state_t *archregs)
{
#define dpr(reg) debug_printf(#reg " %08"PRIu32, archregs->reg)

	dpr(r0);  dpr(r1);  dpr(r2);  dpr(r3);  dpr(r4);
	dpr(r5);  dpr(r6);  dpr(r7);  dpr(r8);  dpr(r9);
    dpr(r10); dpr(r11); dpr(r12); dpr(r13); dpr(r14);
    dpr(r15); dpr(r16); dpr(r17); dpr(r18); dpr(r19);
    dpr(r20); dpr(r21); dpr(r22); dpr(r23); dpr(r24);
    dpr(r25); dpr(r26); dpr(r27); dpr(r28); dpr(r29);
    dpr(r30); dpr(r31); dpr(r32); dpr(r33); dpr(r34);
    dpr(r35); dpr(r36); dpr(r37); dpr(r38); dpr(r39);
    dpr(r40); dpr(r41); dpr(r42); dpr(r43); dpr(r44);
    dpr(r45); dpr(r46); dpr(r47); dpr(r48); dpr(r49);
    dpr(r50); dpr(r51);
    dpr(r52); dpr(tp); dpr(sp); dpr(lr);
}

void debug_call_chain(arch_registers_state_t *archregs)
{
	assert(!"implement me");
}

void debug_print_save_area(arch_registers_state_t *state)
{
    debug_dump(state);
}
