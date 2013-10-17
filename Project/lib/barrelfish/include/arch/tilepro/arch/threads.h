/*
 * threads.h
 *
 *  Created on: Aug 24, 2012
 *      Author: robert
 */

#ifndef LIBBARRELFISH_ARCH_THREADS_H
#define LIBBARRELFISH_ARCH_THREADS_H

#include <assert.h>

/**
 * Returns true iff the thread with the given save area has successfully
 * performed a syscall. Used for the thread_invoke_cap_and_exit() hack.
 */
static inline bool thread_check_syscall_succeeded(uintptr_t *save_area)
{
    assert(!"thread_check_syscall_succeeded: called");
    abort();
#if 0
    return ((save_area[RIP_REG] == (vaddr_t)barrelfish_cap_invoke_post_syscall_instr
             || save_area[RIP_REG] == (vaddr_t)barrelfish_lrpc_post_syscall_instr)
            && save_area[RAX_REG] == 0);
#endif
}
#endif /* LIBBARRELFISH_ARCH_THREADS_H */
