/**
 * \file
 * \brief User-side system call implementation
 */

/*
 * Copyright (c) 2007, 2008, 2009, 2010, ETH Zurich.
 * All rights reserved.
 *
 * This file is distributed under the terms in the attached LICENSE file.
 * If you do not find this file, copies can be found by writing to:
 * ETH Zurich D-INFK, Haldeneggsteig 4, CH-8092 Zurich. Attn: Systems Group.
 */

#include <barrelfish/barrelfish.h>
#include <barrelfish/caddr.h>
#include <barrelfish/dispatch.h>
#include <barrelfish/syscall_arch.h>
#include <barrelfish_kpi/sys_debug.h>

/* For documentation on system calls see include/barrelfish/syscalls.h */

errval_t sys_yield(capaddr_t target)
{
	STATIC_ASSERT_SIZEOF(target, sizeof(uintptr_t));
	return syscall2(SYSCALL_YIELD, (uintptr_t)target).error;
}

errval_t sys_print(const char *string, size_t length)
{
	return syscall3(SYSCALL_PRINT, (uintptr_t)string, (uintptr_t)length).error;
}

/*
 * we define this breakpoint debug function for debugging the code manually.
 * this function can be put in any place in the code you want to start debugging,
 * and then compile the code and type "make debugsim" to debug the code according to the debugging instructions.
 * you may also need to put the function's prototype in the debugging code.
 */
errval_t sys_debug_panic(void);
errval_t sys_debug_panic(void)
{
	return syscall2(SYSCALL_DEBUG, DEBUG_SET_BREAKPOINT).error;
}
