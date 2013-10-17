/** \file
 * \brief x86-specific parts of in-kernel GDB stub.
 *
 * This file implements x86 architecture support for the kernel-side GDB stubs.
 */

/*
 * Copyright (c) 2007, 2008, 2009, ETH Zurich.
 * All rights reserved.
 *
 * This file is distributed under the terms in the attached LICENSE file.
 * If you do not find this file, copies can be found by writing to:
 * ETH Zurich D-INFK, Haldeneggsteig 4, CH-8092 Zurich. Attn: Systems Group.
 */

#include <kernel.h>
#include <stdio.h>
#include <gdb_stub.h>
#include <barrelfish_kpi/cpu.h>
#include <exec.h>


uintptr_t *gdb_arch_registers;

__asm__ (
    ".global gdb_handle_exception       \n"
    "gdb_handle_exception:              \n"
    /* "mov gdb_stack_top(%rip), %rsp      \n" */
    "j gdb_handle_exception_onstack   \n"
);

/** \brief Entry point for an exception; we are now on our own stack.
 *
 * This function sets up the GDB-format register save frame, constructs the
 * initial message to the remote GDB and calls into the generic debugger entry
 * point.
 */
void gdb_handle_exception_onstack(int vector, uintptr_t * NONNULL
        COUNT(NUM_REGS) save_area);
void gdb_handle_exception_onstack(int vector, uintptr_t * NONNULL
        COUNT(NUM_REGS) save_area)
{
    printf("No GDB backend\n");
    halt();
}

/** \brief Get the value of a single register in the frame.
 * \param regnum register number (as defined by the #gdb_register_nums enum)
 * \param value pointer to location in which to return current value
 * \return Zero on success, nonzero on failure (invalid regnum).
 */
int gdb_arch_get_register(int regnum, uintptr_t *value)
{
    panic("NYI");
    return -1;
}

/** \brief Set the value of a single register in the frame.
 * \param regnum register number (as defined by the #gdb_register_nums enum)
 * \param value new value
 * \return Zero on success, nonzero on failure (invalid regnum).
 */
int gdb_arch_set_register(int regnum, uintptr_t value)
{
    panic("NYI");
    return -1;
}

/** \brief Resume execution.
 *
 * Resumes execution with the CPU state stored in the #gdb_arch_registers frame.
 */
void gdb_resume(void);
void gdb_resume(void)
{
    panic("NYI");
}

/** \brief Resume program execution.
 * \param addr Address to resume at, or 0 to continue at last address.
 */
void gdb_arch_continue(lvaddr_t addr)
{
    gdb_resume(); /* doesn't return */
}

/** \brief Single-step program execution.
 * \param addr Address to resume at, or 0 to continue at last address.
 */
void gdb_arch_single_step(lvaddr_t addr)
{
    gdb_resume(); /* doesn't return */
}

/** \brief Writes a byte to an arbitrary address in kernel memory.
 * \return Zero on success, nonzero on error (invalid address)
 */
int gdb_arch_write_byte(uint8_t *addr, uint8_t val)
{
    panic("NYI");
    return 0;
}

/** \brief Reads a byte from an arbitrary address in kernel memory.
 * \return Zero on success, nonzero on error (invalid address)
 */
int gdb_arch_read_byte(uint8_t *addr, uint8_t *val)
{
    panic("NYI");
    return 0;
}
