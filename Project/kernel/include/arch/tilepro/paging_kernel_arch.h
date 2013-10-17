/*
 * paging_kernel_arch.h
 *
 *  Created on: Aug 22, 2012
 *      Author: robert
 */

#ifndef KERNEL_ARCH_TILEPRO_PAGING_H
#define KERNEL_ARCH_TILEPRO_PAGING_H

#include <dispatch.h>
#include <barrelfish_kpi/paging_arch.h>
#include <page_table.h>

void paging_context_switch(lpaddr_t table_addr, struct dcb* dcb);

lvaddr_t paging_map_device(lpaddr_t base, size_t size);

static inline size_t get_pte_size(void)
{
	return sizeof(union Tile_PTE);
}

static inline bool is_root_pt(enum objtype type)
{
    return type == ObjType_VNode_tilepro_l1;
}

static inline void do_one_tlb_flush(genvaddr_t vaddr)
{
	assert(vaddr < ((genvaddr_t)1)<<32);
	lvaddr_t vaddr32 = (lvaddr_t)vaddr;

	int err = hv_flush_page(vaddr32, BASE_PAGE_SIZE);
	assert(err == 0);
}

static inline void do_selective_tlb_flush(genvaddr_t vaddr, genvaddr_t vend)
{
    assert(vaddr < ((genvaddr_t)1)<<32);
    assert(vend < ((genvaddr_t)1)<<32);
    uint32_t vaddr32 = (uint32_t)vaddr;
    uint32_t vend32 = (uint32_t)vend;
    uint32_t size = vend32 - vaddr32;

    int err = hv_flush_pages(vaddr32, BASE_PAGE_SIZE, size);
    assert(err == 0);
}

static inline void do_full_tlb_flush(void)
{
	int err = hv_flush_all(1);
	assert(err == 0);
}

#endif /* KERNEL_ARCH_TILEPRO_PAGING_H */
