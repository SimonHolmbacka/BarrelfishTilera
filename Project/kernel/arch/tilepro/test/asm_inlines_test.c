/*
 * asm_inline_test.c
 *
 *  Created on: Sep 11, 2012
 *      Author: robert
 */

#include "testcase.h"
#include "barrelfish_kpi/asm_inlines_arch.h"

void rdtsc_test()
{
	uint64_t rdtsc1 = rdtsc();
	uint64_t rdtsc2 = rdtsc();
	assertNotEquals_64t_in_hex(rdtsc1, 0);
	assertNotEquals_64t_in_hex(rdtsc2, 0);
	assertSmallerThan_u64t(rdtsc1, rdtsc2);
}

void asm_inlines_test()
{
	rdtsc_test();
}
