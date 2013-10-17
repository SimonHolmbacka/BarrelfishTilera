/*
 * offsets.h
 *
 *  Created on: Aug 17, 2012
 *      Author: robert
 */

#ifndef ARCH_TILEPRO_OFFSETS_H_
#define ARCH_TILEPRO_OFFSETS_H_

#include <stdio.h>

#include <kernel.h>

#include <misc.h>
#include <mem_loc.h>
#include <arch/spr_def.h>
#include <barrelfish_kpi/tilepro_core_data.h>


#define GEN_ADDR(bits)          (((genpaddr_t)1) << bits)

/**
 * Absolute size of virtual address space.
 */
#define VADDR_SPACE_SIZE        GEN_ADDR(32);

/**
 * Absolute size of physical address space.
 */
#define PADDR_SPACE_SIZE        GEN_ADDR(36)

/**
 * Maximum physical address space mappable by the kernel.  Adjust this
 * for a bigger physical address space.
 */
#define PADDR_SPACE_LIMIT       GEN_ADDR(36)

lvaddr_t local_phys_to_mem(lpaddr_t addr);

lpaddr_t mem_to_local_phys(lvaddr_t addr);

static inline lpaddr_t gen_phys_to_local_phys(genpaddr_t addr) {
    return (lpaddr_t)addr;
}

static inline genpaddr_t local_phys_to_gen_phys(lpaddr_t addr) {
	return (genpaddr_t)addr;
}

void init_offset(uint64_t core_offset, struct physical_mem_range *bsp_range);

#endif /* ARCH_TILEPRO_OFFSETS_H_ */
