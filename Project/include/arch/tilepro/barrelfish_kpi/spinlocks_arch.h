/*
 * spinlocks_arch.h
 * this file implements spinlock for thread.
 *
 * wikipedia -> spinlocks
 * Implementing spin locks correctly is difficult because one must take into account
 * the possibility of simultaneous access to the lock to prevent race conditions.
 * Generally this is only possible with special assembly language instructions,
 * such as atomic test-and-set operations, and cannot be easily implemented in high-level
 * programming languages or those languages which don't support truly atomic operations.
 *
 * pseudo-code algorithm using the Test and Set method:
 *
 * acquire_spinlock(s)
 * {
 *   do {
 *     set s = 0;
 *     if(s == 0 && previous value of s == 1) {
 *       break; // we obtained the lock
 *     }
 *   } while(true);
 * }
 *
 * release_spinlock(s)
 * {
 *   set s = 1; // release the lock
 * }
 *
 *  Created on: Aug 17, 2012
 *      Author: robert
 */

#ifndef ARCH_TILEPRO_BARRELFISH_KPI_SPINLOCKS_H
#define ARCH_TILEPRO_BARRELFISH_KPI_SPINLOCKS_H

#include <arch/spr_def.h>


typedef volatile uint32_t spinlock_t;


/*
 * acquire a lock
 */
static inline void acquire_spinlock(spinlock_t* spinlock)
{
	int exp = 0;
	// we use the test and set method:
	// load the previous value from the memory first, and then save 1 to that address afterwards.
	// the assembly code here to prevent more than one threads accessing the lock at the same time.
	while(__insn_tns((void*)spinlock) != 0) {
		int loops = (exp > 6) ? (1UL << 6) : (1UL << exp);
		for( ; loops > 0; loops--) {
			__insn_mfspr(SPR_PASS);
		}
		exp++;
	}
	__insn_mf();  // maybe not necessary
}

/*
 * release the lock
 */
static inline void release_spinlock(spinlock_t* spinlock)
{
	// set up the memory fence,
	// to make sure all the transaction is finished before releasing the lock
	__insn_mf();

	if(spinlock) {
		*(spinlock_t*)spinlock = 0;
	}
}


#endif /* ARCH_TILEPRO_BARRELFISH_KPI_SPINLOCKS_H */
