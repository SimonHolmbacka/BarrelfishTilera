/*
 * virt_mem.c
 *
 *  Created on: Oct 15, 2012
 *      Author: robert
 */
#include <stdio.h>

#include <misc.h>
#include <assert.h>
#include "virt_mem.h"

// Simplest ASID management possible
uint8_t number_of_asids = 0;
uint32_t current_asid = 0;

uint8_t get_next_asid(void)
{
	if (current_asid >= number_of_asids)
	{
		tilepro_panic("Out of ASIDs.\n");
	}
	return ++current_asid;
}

static void init_asids(void)
{
	HV_ASIDRange range = hv_inquire_asid(0);
	// We don't calculate with offsets or something.
	assert(!range.start);
	// We assume 8 bit.
	assert(range.size <= 256);
	// Assert to get a valid range.
	assert(range.size);
	number_of_asids = range.size - 1;
}

static void virt_mem_sanity_check(void)
{
	int index = 0;
	HV_VirtAddrRange v_range;
	int check_passed = 0;
	do {
		v_range = hv_inquire_virtual(index);
		if (!v_range.size)
		{
			break;
		}
		uint64_t end = v_range.start + v_range.size;
		if (v_range.start <= MEM_DYN_USER_START_V && end >= MEM_DYN_USER_END_V) {
			check_passed = 1;
		}
		//printf("[Virtual memory range %d] : 0x%.8lX to 0x%.8llX\n", index, v_range.start, end);
		index++;
	} while (v_range.size != 0);
	if (!check_passed)
	{
		tilepro_panic("ERROR! Our virtual memory is in not in any code region, known by the hypervisor!");
	}
}

void init_virt_mem(void) {
	init_asids();
	virt_mem_sanity_check();
}

