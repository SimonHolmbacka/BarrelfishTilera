/*
 * bench_arch.h
 *
 *  Created on: Aug 22, 2012
 *      Author: robert
 */

#ifndef ARCH_TILEPRO_BARRELFISH_BENCH_H
#define ARCH_TILEPRO_BARRELFISH_BENCH_H

#include <bench/bench.h>

void bench_arch_init(void);

/**
 * \brief Take a timestamp
 */
static inline cycles_t bench_tsc(void)
{
	return rdtsc();
}


#endif /* ARCH_TILEPRO_BARRELFISH_BENCH_H */
