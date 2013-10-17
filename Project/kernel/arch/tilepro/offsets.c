/*
 * offsets.c
 *
 *  Created on: Jan 28, 2013
 *      Author: robert
 */

#include <offsets.h>

static int offset_set = 0;
static struct physical_mem_range *bsp_mem_range;
static lpaddr_t mem_core_offset;

static inline lvaddr_t get_virtual_offset(void) {
	lpaddr_t app_core_offset = bsp_mem_range ? bsp_mem_range->size : 0;
	lvaddr_t virtual_offset = (lvaddr_t)(MEM_KERNEL_START_V + app_core_offset);
	return virtual_offset;
}

lvaddr_t local_phys_to_mem(lpaddr_t addr) {
	if (!addr) {
		return 0;
	}
	if (addr >= MEM_BF_INTERNAL_LARGE_P && addr < MEM_BF_INTERNAL_LARGE_P + HV_PAGE_SIZE_LARGE) {
		return addr - MEM_BF_INTERNAL_LARGE_P + MEM_BF_INTERNAL_LARGE_V;
	}
	if (bsp_mem_range && addr >= bsp_mem_range->start && addr < bsp_mem_range->start + bsp_mem_range->size) {
		lvaddr_t result = (lvaddr_t)(addr + (lvaddr_t)(MEM_KERNEL_START_V) - bsp_mem_range->start);
		return result;
	}
	assert(addr >= mem_core_offset);
	lvaddr_t result = (lvaddr_t)(addr + get_virtual_offset() - mem_core_offset);
	// printf("local_phys_to_mem(%.8" PRIx32 "_%.8" PRIx32 "): %.8" PRIxLVADDR "\n", (uint32_t)(addr >> 32), (uint32_t)addr, result);
	return result;
}

lpaddr_t mem_to_local_phys(lvaddr_t addr) {
	if (bsp_mem_range && addr >= MEM_KERNEL_START_V && addr < MEM_KERNEL_START_V + bsp_mem_range->size) {
		lpaddr_t result = (lpaddr_t)(addr - MEM_KERNEL_START_V + bsp_mem_range->start);
		return result;
	}
	assert(addr >= get_virtual_offset());
	lpaddr_t result = (lpaddr_t)(addr - get_virtual_offset() + mem_core_offset);
	// printf("mem_to_local_phys(%.8" PRIxLVADDR "): %.8lX_%.8lX\n", addr, (uint32_t)(result >> 32), (uint32_t)result);
	return result;
}

void init_offset(uint64_t core_offset, struct physical_mem_range *bsp_range) {
	if (offset_set) {
		tilepro_panic("Offset already set, no cheating here!\n");
	}
	offset_set = 1;
	bsp_mem_range = bsp_range;
	mem_core_offset = core_offset;

}
