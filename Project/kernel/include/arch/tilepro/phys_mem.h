/*
 * memory.h
 *
 *  Created on: Sep 18, 2012
 *      Author: robert
 */

#ifndef ARCH_TILEPRO_PHYS_MEMORY_H_
#define ARCH_TILEPRO_PHYS_MEMORY_H_

#include <hv/hypervisor.h>
#include <barrelfish_kpi/paging_arch.h>
#include <target/tilepro/barrelfish_kpi/paging_target.h>
#include <barrelfish_kpi/tilepro_core_data.h>


struct tilepro_core_data phys_mem;

struct tilepro_core_data init_phys_mem_bsp_core(struct physical_mem_range **first_core_range);

void init_phys_mem_app_core(struct physical_mem_range *bsp_range, struct physical_mem_range *range);

lpaddr_t tilepro_alloc_phys(unsigned int size);

#endif /* ARCH_TILEPRO_PHYS_MEMORY_H_ */
