/*
 * misc.c
 *
 *  Created on: Sep 3, 2012
 *      Author: robert
 */

#include <kernel.h>
#include <stdarg.h>
#include <stdio.h>
#include <exec.h>
#include <misc.h>
#include <hv/hypervisor.h>

#include <arch/spr_def.h>

#define DEFAULT_LOGLEVEL        (LOG_DEBUG)
#define DEFAULT_SUBSYSTEM_MASK  (~0L)

/**
 * Global kernel loglevel.
 */
int kernel_loglevel = DEFAULT_LOGLEVEL;

/**
 * Default kernel subsystem message mask. Determines messages of what subsystems
 * get output.
 */
int kernel_log_subsystem_mask = DEFAULT_SUBSYSTEM_MASK;

/**
 * The current time since kernel start in timeslices.
 */
size_t kernel_now = 0;

/**
 * \brief Print a message and halt the kernel.
 *
 * Something irrecoverably bad happened. Print a panic message, then halt.
 */
void panic(const char *msg, ...)
{
    va_list ap;
    static char buf[256];

    va_start(ap, msg);
    vsnprintf(buf, sizeof(buf), msg, ap);
    va_end(ap);

    printf("kernel PANIC! %.*s\n", (int)sizeof(buf), buf);

    // Used for crashing the process, to get a stack-trace.
	__asm __volatile ("ill");
    halt();
}

// Just a copy, no need around to play with the var-args.
void tilepro_panic(const char *msg, ...)
{
    va_list ap;
    static char buf[256];

    va_start(ap, msg);
    vsnprintf(buf, sizeof(buf), msg, ap);
    va_end(ap);

    printf("kernel PANIC! %.*s\n", (int)sizeof(buf), buf);

    // Used for crashing the process, to get a stack-trace.
	__asm __volatile ("ill");
    halt();
}

/**
 * \brief Log a kernel message.
 *
 * Logs printf()-style message 'msg', having loglevel 'level' to the default
 * kernel console(s). Additional arguments are like printf(). Whether the
 * message is put out depends on the current kernel log level.
 *
 * \param level Loglevel of message.
 * \param msg   The message (printf() format string)
 */
void printk(int level, const char *msg, ...)
{
    if(kernel_loglevel >= level) {
        va_list ap;
        static char buf[256];

        va_start(ap, msg);
        vsnprintf(buf, sizeof(buf), msg, ap);
        va_end(ap);

        printf("kernel %.*s", (int)sizeof(buf), buf);
    }
}

/**
 * BSP = BootStrap processor
 * Returns true if called on core 0
 */
bool arch_core_is_bsp(void)
{
	int cpu_id =__insn_mfspr(SPR_SYSTEM_SAVE_1_0);
	return !cpu_id;
}


void unknown_interrupt(int vecnum, char vecname[])
{
	uint32_t last_pc = __insn_mfspr(SPR_EX_CONTEXT_1_0);
	tilepro_panic("Unimplemented interrupt(%d) (last_pc at: %.8lX) %s\n", vecnum, last_pc, vecname);
}

/**
 * Helper function used in the implementation of assert()
 */
#ifdef CONFIG_NEWLIB
void __assert_func(const char *file, int line, const char *func, const char *exp)
#else
void __assert(const char *exp, const char *file, const char *func, int line)
#endif
{
	tilepro_panic("kernel assertion \"%s\" failed at %s:%d", exp, file, line);
}
