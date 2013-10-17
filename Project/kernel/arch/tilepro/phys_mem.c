/*
 * memory.c
 *
 *  Created on: Sep 18, 2012
 *      Author: robert
 */

#include <assert.h>
#include <stdio.h>
#include <barrelfish_kpi/types.h>
#include <page_table.h>
#include <offsets.h>

#include <exec.h>
#include <misc.h>
#include <phys_mem.h>
#include <mem_loc.h>

#include <arch/spr_def.h>

#define bit64_as_2_bit32(x) (uint32_t)((x) >> 32), (uint32_t)(x)

// Created by the linker script
extern int _end_data;

static struct physical_mem_range mem_range = {0, 0, 0};
static struct physical_mem_range bsp_mem_range = {0, 0, 0};

static void phys_mem_sanity_check(void)
{
	// We compare the virtual addresses here, but the size is the same,
	// so we know that certain physical addresses are not crossed.
	uint32_t end_data_v = (uint32_t)&_end_data;
//	printf("_end_data: 0x%.8" PRIX32 "\n", end_data_v);
//	printf("MEM_LINK_DATA_END_V: 0x%.8" PRIX32 "\n", MEM_LINK_DATA_END_V);
	assert(end_data_v <= MEM_LINK_DATA_END_V);

	assert(mem_range.start >= MEM_KERNEL_START_P);
	assert(mem_range.size >= MEM_KERNEL_APP_CORE_SIZE);
	assert(mem_range.bytes_free >= MEM_KERNEL_APP_CORE_SIZE);
	// MAX_COREID is used in the barrelfish code. Assure that they would never use more cores,
	// than our tilepro implementation could handle
	assert(MAX_COREID <= MAX_TILES);
}

static inline void cut_out_memory(HV_PhysAddrRange* hv_range, uint64_t amount, int from_begin) {
	if (from_begin) {
		hv_range->start += amount;
	}
	hv_range->size -= amount;
}

struct tilepro_core_data init_phys_mem_bsp_core(struct physical_mem_range **first_core_range) {
	struct tilepro_core_data core_data;
	core_data.n_data = 0;
	int for_core = 0;
	int index = 0;
	while (for_core < MAX_PHYSICAL_CORES) {
		HV_PhysAddrRange hv_range = hv_inquire_physical(index++);
		if (!hv_range.size) {
			printf("No more memory to serve core %d\n", for_core);
			break;
		}
		printf("[Physical memory range %d] : 0x%lX_%.8lX to 0x%lX_%.8lX\n",
				index, bit64_as_2_bit32(hv_range.start), bit64_as_2_bit32(hv_range.start + hv_range.size));
		if (hv_range.start < MEM_KERNEL_START_P) {
			uint32_t diff = (MEM_KERNEL_START_P - hv_range.start);
			cut_out_memory(&hv_range, diff, 1);
		}
		uint64_t too_much_at_begin = hv_range.start % BASE_PAGE_SIZE;
		if (too_much_at_begin) {
			printf("Cutting out 0x%lX_%.8lX at the begin to adjust to the page size\n", bit64_as_2_bit32(too_much_at_begin));
			cut_out_memory(&hv_range, BASE_PAGE_SIZE - too_much_at_begin, 1);
		}
		uint64_t too_much_at_end = hv_range.size % BASE_PAGE_SIZE;
		if (too_much_at_end) {
			printf("Cutting out 0x%lX_%.8lX at the end to adjust to the page size\n", bit64_as_2_bit32(too_much_at_end));
			cut_out_memory(&hv_range, BASE_PAGE_SIZE - too_much_at_end, 0);
		}
		while (hv_range.size) {
			uint64_t core_size = (for_core == 0) ? MEM_KERNEL_BSP_CORE_SIZE : MEM_KERNEL_APP_CORE_SIZE;
			if (hv_range.size < core_size) {
				printf("Current range has 0x%lX_%.8lX bytes remaining, which is not enough for another core\n", bit64_as_2_bit32(hv_range.size));
				break;
			}
			core_data.n_data = for_core + 1;
			//ranges.n_physical_mem_ranges = for_core + 1;
			core_data.per_core[for_core].phys_mem.start = hv_range.start;
			core_data.per_core[for_core].phys_mem.size = core_size;
			core_data.per_core[for_core].phys_mem.bytes_free = core_size;
			hv_range.start += core_size;
			hv_range.size -= core_size;
			printf("Physical Memory for core %lu - start: 0x%lX_%.8lX; end: 0x%lX_%.8lX; free: 0x%lX_%.8lX\n",
					core_data.n_data - 1,
					bit64_as_2_bit32(core_data.per_core[for_core].phys_mem.start),
					bit64_as_2_bit32(core_data.per_core[for_core].phys_mem.start + core_data.per_core[for_core].phys_mem.size),
					bit64_as_2_bit32(core_data.per_core[for_core].phys_mem.bytes_free));
			for_core++;
			if (for_core >= MAX_PHYSICAL_CORES) {
				break;
			}
		}
	}
	// Core 0 needs to set his memory also
	assert(core_data.n_data >= 1);

	mem_range = core_data.per_core[0].phys_mem;
	phys_mem_sanity_check();
	create_kernel_mappings_in_boot_l1(&mem_range, NULL);
	init_offset(mem_range.start, NULL);
	*first_core_range = &mem_range;

	return core_data;
}

void init_phys_mem_app_core(struct physical_mem_range *bsp_range, struct physical_mem_range *range) {
	bsp_mem_range = *bsp_range;
	mem_range = *range;
	phys_mem_sanity_check();
	create_kernel_mappings_in_boot_l1(bsp_range, range);
	init_offset(mem_range.start, bsp_range);
}


lpaddr_t tilepro_alloc_phys(unsigned int size)
{
	uint32_t page_size = BASE_PAGE_SIZE;
    uint32_t npages = (size + page_size - 1) / page_size;
    uint32_t bytes_to_alloc = npages * page_size;

	if (mem_range.bytes_free >= bytes_to_alloc)
	{
		lpaddr_t addr = mem_range.start + mem_range.size - mem_range.bytes_free;
		mem_range.bytes_free -= bytes_to_alloc;
		lpaddr_t end = addr + bytes_to_alloc;
		printf("Allocated physical - start: 0x%lX_%.8lX; size: 0x%.8" PRIX32 "; requested: 0x%.8" PRIX16
				"; end: 0x%lX_%.8lX\n", (uint32_t)(addr >> 32), (uint32_t)addr, bytes_to_alloc, size, (uint32_t)(end >> 32), (uint32_t)end);
		return addr;
	}
    tilepro_panic("Out of memory!");
    halt();
}
