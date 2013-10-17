/*
 * memory_test.c
 *
 *  Created on: Sep 18, 2012
 *      Author: robert
 */

#include "testcase.h"
#include "test_panic.h"
#include <phys_mem.h>

// This test just handles the first 2

void XassertSize(uint64_t first, uint64_t second, char file[], int line) {
	XassertEquals_64t_in_hex(first, mem[0].bytes_free, file, line);
	XassertEquals_64t_in_hex(second, mem[1].bytes_free, file, line);
}

#define assertSize(f, s) XassertSize(f, s, __FILE__, __LINE__)

void setup_memory() {
	int i;
	for (i = 0; i < 2; i++) {
		uint64_t addr = 0;
		uint64_t size = LARGE_PAGE_SIZE;
		int controller = 0;
		HV_PhysAddrRange hv_range = { addr, size, controller };
		struct physical_range r = { hv_range, hv_range.size };
		mem[i] = r;
	}
	for (i = 2; i < MAX_MEM_RANGES; i++) {
		uint64_t addr = 0;
		uint64_t size = 0;
		int controller = 0;
		HV_PhysAddrRange hv_range = { addr, size, controller };
		struct physical_range r = { hv_range, hv_range.size };
		mem[i] = r;
	}
}

void test_alloc_large_size() {
	setup_memory();
	app_alloc_phys(LARGE_PAGE_SIZE);
	assertSize(0, LARGE_PAGE_SIZE);
}

void test_alloc_less_than_large_size() {
	setup_memory();
	app_alloc_phys(LARGE_PAGE_SIZE - 1);
	assertSize(0, LARGE_PAGE_SIZE);
}

void test_alloc_small_pages() {
	setup_memory();
	for (int i = 0; i < (LARGE_PAGE_SIZE / SMALL_PAGE_SIZE) - 1; i++) {
		app_alloc_phys(1);
	}
	assertSize(SMALL_PAGE_SIZE, LARGE_PAGE_SIZE);
	app_alloc_phys(1);
	assertSize(0, LARGE_PAGE_SIZE);
	for (int i = 0; i < (LARGE_PAGE_SIZE / SMALL_PAGE_SIZE) - 1; i++) {
		app_alloc_phys(1);
	}
	assertSize(0, SMALL_PAGE_SIZE);
	app_alloc_phys(1);
	assertSize(0, 0);
}

void test_oom()
{
	app_alloc_phys(1);
	app_alloc_phys(1);
	assertEquals_32t_in_hex(0, panic_occured);
	app_alloc_phys(1);
	assertEquals_32t_in_hex(1, panic_occured);
	reset_panic();
}

void memory_test() {
	test_alloc_large_size();
	test_alloc_less_than_large_size();
	test_alloc_small_pages();
}
