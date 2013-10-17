/*
 * paging.c
 *
 *  Created on: Sep 3, 2012
 *      Author: robert
 */

#include <kernel.h>
#include <capabilities.h>
#include <paging_kernel_arch.h>
#include <string.h>
#include <barrelfish_kpi/paging_arch.h>
#include <page_table.h>
#include <virt_mem.h>
#include <cap_predicates.h>
#include "mem_loc.h"

inline static int aligned(uintptr_t address, uintptr_t bytes)
{
    return (address & (bytes - 1)) == 0;
}

static void announce_new_process(uint8_t cpu_id, uint8_t asid)
{
	// Tell the simulator that we have a new "process".
	// Maybe there is a way to debug only this process.
	HV_Context context = hv_inquire_context();
	uint8_t parent_asid = context.asid;
	uint16_t parent_id = (cpu_id << 8) | parent_asid;
	__insn_mtspr(SPR_SIM_CONTROL, SIM_CONTROL_OS_FORK_PARENT | (parent_id << _SIM_CONTROL_OPERATOR_BITS));
	uint16_t id = (cpu_id << 8) | asid;
	__insn_mtspr(SPR_SIM_CONTROL, SIM_CONTROL_OS_FORK | (id << _SIM_CONTROL_OPERATOR_BITS));
}

void paging_context_switch(lpaddr_t addr, struct dcb* dcb)
{
	//printf(__FILE__ ": paging_context_switch(0x%.8lX) is called\n", addr);
	//printf("context switch...\n");
	union Tile_PTE acc = create_access_pte();
	HV_PTE access;
	access.val = acc.raw;
	uint8_t asid = dispatcher_tilepro_get_asid(dcb->disp);
	uint8_t cpu_id = __insn_mfspr(SPR_SYSTEM_SAVE_1_0);
	if(!asid) {  // if this is a new dispatcher
		//printf("new dispatcher to be invoked...\n");
		// Copy the kernel mappings once to each new process.
		copy_kernel_mappings_to_l1_page_table((union Tile_PTE*)local_phys_to_mem(addr));
		asid = get_next_asid();
		dispatcher_tilepro_start_in_chan_count(dcb->disp);
		dispatcher_tilepro_set_asid(dcb->disp, asid);
		dispatcher_tilepro_set_tile_width(dcb->disp, (uint8_t)hv_inquire_topology().width);
		dispatcher_tilepro_set_tile_height(dcb->disp, (uint8_t)hv_inquire_topology().height);
		announce_new_process(cpu_id, asid);
	}
	uint16_t id = (cpu_id << 8) | asid;
	__insn_mtspr(SPR_SIM_CONTROL, SIM_CONTROL_OS_SWITCH | (id << _SIM_CONTROL_OPERATOR_BITS));
	//print_page_mappings((union Tile_PTE*)local_phys_to_mem(addr));
	//printf("current ASID : %d\n", asid);
	//printf("current dcb dispatcher : 0x%.8lX\n", (lvaddr_t)dcb->disp);
	uint32_t flags = 0;
	int err = hv_install_context(addr, access, asid, flags);
	assert(err == 0);
	// Due to the use of ASIDs, we don't need to flush.
	//hv_flush_all(1);
}

lvaddr_t paging_map_device(lpaddr_t base, size_t size)
{
	assert(!"implement me");
}

void paging_dump_tables(struct dcb *dispatcher)
{
    lvaddr_t l1 = local_phys_to_mem(dispatcher->vspace);
    print_page_mappings((union Tile_PTE*)l1);
}

static inline struct cte *cte_for_cap(struct capability *cap)
{
    return (struct cte*)(cap - offsetof(struct cte, cap));
}

/*
 * set up the mapping bewteen l1 and l2 page tables
 */
static errval_t caps_map_l1(struct capability *dest,
		                       cslot_t slot,
		                       struct capability *src,
		                       uint64_t flags,
		                       uintptr_t offset,
		                       uintptr_t pte_count)
{
	// first do some checks
	if(slot >= LARGE_PAGE_TABLE_ENTRIES) {
		return SYS_ERR_VNODE_SLOT_INVALID;
	}
	if(pte_count != 1) {
		printf("pte_count = %zu\n", (size_t)pte_count);
		panic("oops: pte_count");
		return SYS_ERR_VM_MAP_SIZE;
	}
	if(slot >= LARGE_PAGE_TABLE_BASE(MEM_KERNEL_START_V)) {  // Kernel mapped here
		return SYS_ERR_VNODE_SLOT_RESERVED;
	}
	if(src->type != ObjType_VNode_tilepro_l2) {  // Right mapping
		return SYS_ERR_WRONG_MAPPING;
	}

	// Destination
	lpaddr_t dest_lpaddr = gen_phys_to_local_phys(get_address(dest));
	lvaddr_t dest_lvaddr = local_phys_to_mem(dest_lpaddr);

	// Source
	genpaddr_t src_gpaddr = get_address(src);
	lpaddr_t   src_lpaddr = gen_phys_to_local_phys(src_gpaddr);

    assert(offset == 0);
    assert(aligned(src_lpaddr, 1u << BASE_PAGE_BITS));
    //assert((src_lpaddr < dest_lpaddr) || (src_lpaddr >= dest_lpaddr + 16384));

    struct cte *src_cte = cte_for_cap(src);
	src_cte->mapping_info.pte_count = pte_count;
	src_cte->mapping_info.pte = dest_lpaddr + slot;
	src_cte->mapping_info.offset = 0;

	// create l1 page table entry to link to corresponding l2 page talbe
	union Tile_PTE l1_entry = create_l1_link_to_l2_pte(src_lpaddr);

	// map l2 entry to l1 entry
	union Tile_PTE *l1 = (union Tile_PTE*)dest_lvaddr;
	l1[slot] = l1_entry;

//	printf("l1 base address : [va]0x%.8lX; [pa]0x%.8lX\n", dest_lvaddr, dest_lpaddr);
//	printf("l2 base address : [va]0x%.8lX; [pa]0x%.8lX\n", src_lvaddr, src_lpaddr);
//	printf("slot : %lu\n", slot);
//	print_page_mappings(l1);

	return SYS_ERR_OK;
}

/*
 * set up the mapping between l2 page table and the memory address space
 */
static errval_t caps_map_l2(struct capability *dest,
		                       cslot_t slot,
		                       struct capability *src,
		                       uint64_t pte_flags,
		                       uintptr_t offset,
		                       uintptr_t pte_count)
{
	// first do some checks
	if(slot >= BASE_PAGE_TABLE_ENTRIES) {
		return SYS_ERR_VNODE_SLOT_INVALID;
	}

	if(src->type != ObjType_Frame && src->type != ObjType_DevFrame) {
		panic("oops");
		return SYS_ERR_WRONG_MAPPING;
	}

	// check offset within frame
	if((offset + BASE_PAGE_SIZE) > get_size(src)) {
		panic("oops");
		return SYS_ERR_FRAME_OFFSET_INVALID;
	}

	// check mapping does not overlap leaf page table
	if (slot + pte_count > BASE_PAGE_TABLE_ENTRIES) {
		return SYS_ERR_VM_MAP_SIZE;
	}

	// Destination
	lpaddr_t dest_lpaddr = gen_phys_to_local_phys(get_address(dest));
	lvaddr_t dest_lvaddr = local_phys_to_mem(dest_lpaddr);


    lpaddr_t src_lpaddr = gen_phys_to_local_phys(get_address(src) + offset);
    if((src_lpaddr & (BASE_PAGE_SIZE - 1))) {
        panic("Invalid target");
    }

    struct cte *src_cte = cte_for_cap(src);
    src_cte->mapping_info.pte_count = pte_count;
    src_cte->mapping_info.pte = dest_lpaddr;
    src_cte->mapping_info.offset = offset;

    // then we fill the mapping into page table one by one
    for(int i = 0; i < pte_count; i++) {
    	union Tile_PTE entry;
		entry.raw = 0;
		entry.pte.present = 1;
		entry.pte.user = 1;
		entry.pte.accessed = 1;
		entry.pte.dirty = 1;
		entry.pte.pfn = HV_CPA_TO_PFN(src_lpaddr + i * BASE_PAGE_SIZE);
		entry.raw |= pte_flags;

		// map the address to l2 page table
		union Tile_PTE *l2 = (union Tile_PTE*)dest_lvaddr;
		if(l2[slot+i].pte.present) {
			tilepro_panic("Remapping valid page.");
		} else {
			l2[slot + i] = entry;
		}
    }

	return SYS_ERR_OK;
}

/// Create page mappings
errval_t caps_copy_to_vnode(struct cte *dest_vnode_cte, cslot_t dest_slot, struct cte *src_cte, uint64_t flags, uintptr_t offset, uintptr_t pte_count)
{
	struct capability *src_cap  = &src_cte->cap;
	struct capability *dest_cap = &dest_vnode_cte->cap;

	if(dest_cap->type == ObjType_VNode_tilepro_l1) {
		return caps_map_l1(dest_cap, dest_slot, src_cap, flags, offset, pte_count);
	} else if(dest_cap->type == ObjType_VNode_tilepro_l2) {
		return caps_map_l2(dest_cap, dest_slot, src_cap, flags, offset, pte_count);
	} else {
		tilepro_panic("ObjType wrong!\n");
	}
	return SYS_ERR_OK;
}

size_t do_unmap(lvaddr_t pt, cslot_t slot, size_t num_pages)
{
    size_t unmapped_pages = 0;
    union Tile_PTE *ptentry = (union Tile_PTE *)pt + slot;
    for(int i = 0; i < num_pages; i++) {
    	ptentry++->raw = 0;
        unmapped_pages++;
    }
    return unmapped_pages;
}

static inline void read_pt_entry(struct capability *pgtable, size_t slot, genpaddr_t *mapped_addr, lpaddr_t *pte, void **entry)
{
    assert(type_is_vnode(pgtable->type));

    genpaddr_t paddr;
    lpaddr_t pte_;
    void *entry_;

    genpaddr_t gp = get_address(pgtable);
    lpaddr_t lp = gen_phys_to_local_phys(gp);
    lvaddr_t lv = local_phys_to_mem(lp);

    // get paddr
    switch(pgtable->type) {
    case ObjType_VNode_tilepro_l1: {
    	union Tile_PTE *e = (union Tile_PTE*)lv + slot;
		paddr = HV_PFN_TO_CPA(e->pte.pfn) + HV_PTFN_TO_CPA(e->pte.ptfn_lower_part);
		entry_ = e;
		pte_ = lp + slot * sizeof(union Tile_PTE);
		break;
    }
	case ObjType_VNode_tilepro_l2: {
		union Tile_PTE *e = (union Tile_PTE*)lv + slot;
		paddr = HV_PFN_TO_CPA(e->pte.pfn);
		entry_ = e;
		pte_ = lp + slot * sizeof(union Tile_PTE);
		break;
	}
	default:
		assert(!"Should not get here");
		break;
    }

    if(mapped_addr) {
        *mapped_addr = paddr;
    }
    if(pte) {
        *pte = pte_;
    }
    if(entry) {
    	*entry = entry_;
    }
}

/*
 * Unmap the page mappings.
 * TODO: this function calls compile_vaddr(), which returns the starting virtual address of PTE,
 * but in tilepro, we could not handle to return a right address, because our setting in vnode_objbits() is strange (see it for explanation),
 * so we simply flush all TLB entries, but time-consuming.
 */
errval_t page_mappings_unmap(struct capability *pgtable, struct cte *mapping, size_t slot, size_t num_pages)
{
	//paging_dump_tables(dcb_current);
    assert(type_is_vnode(pgtable->type));
    //errval_t err;
    debug(SUBSYS_PAGING, "page_mappings_unmap(%zd pages)\n", num_pages);
//    printf("num_pages = %zu\n", num_pages);
//    printf("slot = %zu\n", slot);

    // get page table entry data
    genpaddr_t paddr;
    lpaddr_t pte;
    read_pt_entry(pgtable, slot, &paddr, &pte, NULL);
    lvaddr_t pt = local_phys_to_mem(gen_phys_to_local_phys(get_address(pgtable)));
//    printf("mapped_pa = 0x_%.8lX_%.8lX\n", (uint32_t)(paddr >> 32), (uint32_t)paddr);
//    printf("page_table_va = 0x_%.8lX\n", (uint32_t)pt);
//    printf("pte_pa = 0x_%.8lX\n", (uint32_t)pte);
//    printf("pte_va = 0x_%.8lX\n", (uint32_t)local_phys_to_mem((uint32_t)pte));

    // get virtual address of first page
    // TODO: error checking
//    genvaddr_t vaddr;
//    struct cte *leaf_pt = cte_for_cap(pgtable);
//    err = compile_vaddr(leaf_pt, slot, &vaddr);
//    if(err_is_fail(err)) {
//    	if(err_no(err) == SYS_ERR_VNODE_NOT_INSTALLED) {
//    		debug(SUBSYS_PAGING, "couldn't reconstruct virtual address\n");
//    	} else {
//    		return err;
//    	}
//    }
//    printf("vaddr = 0x_%.8lX_%.8lX\n", (uint32_t)(vaddr >> 32), (uint32_t)vaddr);

    if(num_pages != mapping->mapping_info.pte_count) {
    	printf("num_pages = %zu, mapping = %zu\n", num_pages, mapping->mapping_info.pte_count);
        // want to unmap a different amount of pages than was mapped
        return SYS_ERR_VM_MAP_SIZE;
    }

    size_t unmapped_pages = do_unmap(pt, slot, num_pages);
    assert(unmapped_pages == num_pages);

    // flush TLB for unmapped pages
    // TODO: heuristic that decides if selective or full flush is more efficient?
//    if((num_pages > 1) && err_is_ok(err)) {
//    	do_full_tlb_flush();
//    } else {
//    	do_one_tlb_flush(vaddr);
//    }

    do_full_tlb_flush();

    // update mapping info
    memset(&mapping->mapping_info, 0, sizeof(struct mapping_info));

    return SYS_ERR_OK;
}
