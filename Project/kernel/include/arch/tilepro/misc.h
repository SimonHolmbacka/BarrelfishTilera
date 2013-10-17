/*
 * misc.h
 *
 *  Created on: Aug 17, 2012
 *      Author: robert
 */

#ifndef ARCH_MISC_H_
#define ARCH_MISC_H_

#include <arch/spr_def.h>

#define STARTUP_CORE_TAG -1

// The panic method creates so much problems in the imports, we use our own!
void tilepro_panic(const char * NTS, ...)
    __attribute__((noreturn, format(printf, 1, 2)));

static inline void arch_set_thread_register(uintptr_t val)
{
	//printf("arch_set_thread_register(0x%.8lX)\n", val);
	__asm __volatile("move tp, %[val]"
			: /* no outputs */
			: [val] "r" (val) /* inputs */
			: "tp"); /* other registers clobbered */
}

static inline int get_current_coreid(void) {
	return __insn_mfspr(SPR_SYSTEM_SAVE_1_0);
}

void unknown_interrupt(int vecnum, char vecname[]);

#endif /* ARCH_MISC_H_ */
