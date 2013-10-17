/*
 * page_table.c
 *
 *  Created on: Sep 26, 2012
 *      Author: jackie
 */

#include <hv/hypervisor.h>
#include <arch/spr_def.h>

#include <assert.h>
#include <stdio.h>
#include <offsets.h>
#include <exec.h>
#include <elf/elf.h>
#include <dispatch.h>
#include <barrelfish_kpi/tilepro_core_data.h>

#include <page_table.h>
#include <virt_mem.h>
#include <phys_mem.h>

#include <string.h>
#include <stdarg.h>


static inline union Tile_PTE* get_linked_l2_table_from_l1_entry(union Tile_PTE l1_entry) {
	assert(l1_entry.pte.present);
	assert(!l1_entry.pte.page);
	lpaddr_t l2_paddr = HV_PFN_TO_CPA(l1_entry.pte.pfn) + HV_PTFN_TO_CPA(l1_entry.pte.ptfn_lower_part);
	lvaddr_t l2_vaddr = local_phys_to_mem(l2_paddr);
	union Tile_PTE *l2_table = (union Tile_PTE*)l2_vaddr;
	for (int i = 0; i < HV_L2_ENTRIES; i++) {
		union Tile_PTE read_by_conv = l2_table[i];
		HV_PhysAddr addr = l2_paddr+i*sizeof(union Tile_PTE);
		HV_PTE access = {.val= create_access_pte().raw};
		union Tile_PTE read_by_hv = (union Tile_PTE)hv_physaddr_read64(addr, access);
		assert(read_by_conv.wordwise.most == read_by_hv.wordwise.most);
		assert(read_by_conv.wordwise.least == read_by_hv.wordwise.least);
	}
	return l2_table;
}

static inline uint32_t page_align(uint32_t value, uint32_t page_size) {
	return value - (value % page_size);
}

union Tile_PTE create_l1_link_to_l2_pte(lpaddr_t addr_of_l2_table)
{
	union Tile_PTE pte;
	pte.raw = 0;
	pte.pte.present = 1;
// According to the hypervisor.h these 3 entries
// "determines how the level-2 page table is accessed."
//	pte.pte.nc
//	pte.pte.no_alloc_l1
//	pte.pte.no_alloc_l2
	pte.pte.mode = HV_PTE_MODE_CACHE_NO_L3;
	pte.pte.ptfn_lower_part = HV_CPA_TO_PTFN(addr_of_l2_table) & 0x1F;
	pte.pte.pfn = HV_CPA_TO_PFN(addr_of_l2_table);
	return pte;
}

union Tile_PTE create_global_data_pte(lpaddr_t pa, int is_page)
{
	union Tile_PTE pte;
	pte.raw = 0;
	pte.pte.present = 1;
	if (is_page)
	{
		pte.pte.page = 1;
	}
	pte.pte.global = 1;
	pte.pte.accessed = 1;
	pte.pte.dirty = 1;
	pte.pte.mode = HV_PTE_MODE_CACHE_NO_L3;
	pte.pte.readable = 1;
	pte.pte.writable = 1;
	pte.pte.pfn = HV_CPA_TO_PFN(pa);

	return pte;
}

union Tile_PTE create_global_code_pte(lpaddr_t pa, int is_page)
{
	union Tile_PTE pte;
	pte.raw = 0;
	pte.pte.present = 1;
	if (is_page)
	{
		pte.pte.page = 1;
	}
	pte.pte.global = 1;
	pte.pte.accessed = 1;
	pte.pte.mode = HV_PTE_MODE_CACHE_NO_L3;
	pte.pte.executable = 1;
	pte.pte.pfn = HV_CPA_TO_PFN(pa);

	return pte;
}

union Tile_PTE create_access_pte(void)
{
	union Tile_PTE pte;
	pte.raw = 0;
	pte.pte.mode = HV_PTE_MODE_CACHE_NO_L3;
	return pte;
}

union Tile_PTE create_pte_according_to_elf_flags(lpaddr_t pa, uint32_t elf_flags)
{
	union Tile_PTE pte;
	pte.raw = 0;
	pte.pte.present = 1;
	pte.pte.user = 1;
	pte.pte.mode = HV_PTE_MODE_CACHE_NO_L3;
	//pte.pte.readable = elf_flags & PF_R ? 1 : 0;
	// pte.pte.writable = elf_flags & PF_W ? 1 : 0;
	// To be able to copy the bytes to that position we need write access.
	// TODO We should be able to set the HV_PTE_CLIENT0 bit if the elf flags
	// tell not writable and would be able to remove this bits after writing
	// the program to that address.
	pte.pte.writable = 1;
	pte.pte.readable = 1;

	// These are test entries, until the mapping works
	pte.pte.accessed = 1;
	pte.pte.dirty = 1;
	// Test end

	pte.pte.executable = elf_flags & PF_X ? 1 : 0;
	pte.pte.pfn = HV_CPA_TO_PFN(pa);

	return pte;
}

static void print_page(union Tile_PTE pte, uint32_t va, int is_large)
{
	if (!pte.pte.present) {
		printf("NOT_PRESENT ");
	}
	if (is_large) {
		if (!pte.pte.page) {
			printf("LINK_TO_L2 ");
		}
	} else {
		if (pte.pte.page) {
			printf("PAGE_BIT_SET_BUT_IN_L2 ");
		}
	}
	printf("pte: 0x%.8lX->", va);
	uint32_t pa = HV_PFN_TO_CPA(pte.pte.pfn);
	printf("0x%.8lX ", pa);
	if (is_large) {
		printf("large ");
	}
	printf((pte.pte.readable) ?"r" : "-");
	printf((pte.pte.writable) ?"w" : "-");
	printf((pte.pte.executable) ?"x " : "- ");
	if (pte.pte.mode == 1) {
		printf("UNCACHED ");
	} else if (pte.pte.mode == 2) {
		printf("CACHE_NO_L3 ");
	} else if (pte.pte.mode == 3) {
		printf("CACHE_TILE_L3 ");
	} else if (pte.pte.mode == 4) {
		printf("CACHE_HASH_L3 ");
	} else if (pte.pte.mode == 2) {
		printf("CACHE_NO_L3 ");
	} else if (pte.pte.mode == 2) {
		printf("MMIO ");
	} else {
		printf("UNKNOWN_MODE ");
	}
	if (pte.pte.global) {
		printf("global ");
	}
	if (pte.pte.user) {
		printf("user ");
	}
	if (pte.pte.accessed) {
		printf("accessed ");
	}
	if (pte.pte.dirty) {
		printf("dirty ");
	}
	printf("\n");
}

static void print_l2_page_mappings(union Tile_PTE l2[HV_L2_ENTRIES], uint32_t vbase)
{
	for (int i = 0; i < HV_L2_ENTRIES; ++i) {
		union Tile_PTE pte = l2[i];
		if (!pte.pte.present)
		{
			continue;
		}
		uint32_t va = vbase + (i << HV_LOG2_PAGE_SIZE_SMALL);
		print_page(pte, va, 0);
	}
}

void print_page_mappings(union Tile_PTE l1[HV_L1_ENTRIES])
{
	printf("Printing page table located at 0x%.8lX:\n", (uint32_t)l1);
	for (int i = 0; i < HV_L1_ENTRIES; ++i) {
		union Tile_PTE pte = l1[i];
		if (!pte.pte.present)
		{
			continue;
		}
		uint32_t vbase = i << HV_LOG2_PAGE_SIZE_LARGE;
		if (pte.pte.page) {
			print_page(pte, vbase, 1);
		} else {
			union Tile_PTE *l2_table = get_linked_l2_table_from_l1_entry(pte);
			print_l2_page_mappings(l2_table, vbase);
		}
	}
}

static inline union Tile_PTE* get_boot_l1(void) {
	uint32_t cpu_id = __insn_mfspr(SPR_SYSTEM_SAVE_1_0);
	uint32_t boot_l1 = MEM_L1_PAGE_TABLE(cpu_id);
	return (union Tile_PTE*)boot_l1;
}

void create_kernel_mappings_in_boot_l1(struct physical_mem_range *bsp_range, struct physical_mem_range *range) {
	union Tile_PTE *boot_l1 = get_boot_l1();
	uint32_t number_of_bsp_pages = bsp_range->size / LARGE_PAGE_SIZE;
	for (int i = 0; i < number_of_bsp_pages; i++) {
		int index = HV_L1_INDEX(MEM_KERNEL_START_V) + i;
		boot_l1[index] = create_global_data_pte(bsp_range->start + i * LARGE_PAGE_SIZE, 1);
	}
	if (range != NULL) {
		uint32_t number_of_pages = range->size / LARGE_PAGE_SIZE;
		for (int i = 0; i < number_of_pages; i++) {
			int index = HV_L1_INDEX(MEM_KERNEL_START_V) + number_of_bsp_pages + i;
			boot_l1[index] = create_global_data_pte(range->start + i * LARGE_PAGE_SIZE, 1);
		}
	}
}

void copy_kernel_mappings_to_l1_page_table(union Tile_PTE* l1)
{
	// sanity test first
	assert(l1[HV_L1_INDEX(MEM_BF_INTERNAL_LARGE_V)].raw == 0);
	assert(l1[HV_L1_INDEX(MEM_KERNEL_STACK_MEM_START_V)].raw == 0);
	assert(l1[HV_L1_INDEX(MEM_WHOLE_TEXT_PAGE_V)].raw == 0);

	// copy kernel entries from boot-up L1 page table to new page table
	union Tile_PTE *boot_l1 = get_boot_l1();
	// This entry is just copied, so all copies link to the same l2 page table.
	l1[HV_L1_INDEX(MEM_BF_INTERNAL_LARGE_V)] = boot_l1[HV_L1_INDEX(MEM_BF_INTERNAL_LARGE_V)];
	l1[HV_L1_INDEX(MEM_KERNEL_STACK_MEM_START_V)] = boot_l1[HV_L1_INDEX(MEM_KERNEL_STACK_MEM_START_V)];
	l1[HV_L1_INDEX(MEM_WHOLE_TEXT_PAGE_V)] = boot_l1[HV_L1_INDEX(MEM_WHOLE_TEXT_PAGE_V)];

	// copy kernel space mappings
	lvaddr_t va;
	for(va = MEM_KERNEL_START_V ; va < MEM_KERNEL_END_V; va += HV_PAGE_SIZE_LARGE) {
		l1[HV_L1_INDEX(va)] = boot_l1[HV_L1_INDEX(va)];
	}
}

/*
 * This function is to handle user page fault,
 * filling the missing page table mapping to the corresponding page table.
 * This function is called from exceptions.S.
 */
int page_fault(int vecnum, char vecname[], uint32_t unmapped_va, arch_registers_state_t* save_area)
{
	// release ICS, this will allow higher level interrupts to occur
	__insn_mtspr(SPR_INTERRUPT_CRITICAL_SECTION, 0);

	// get the PC when this page fault occurs, we use it to restore program later
	uint32_t last_pc = __insn_mfspr(SPR_EX_CONTEXT_1_0);

	// print the fault address and dump all mappings
	printf("page_fault(%d) %s; pc: 0x%.8lX; fault_addr: 0x%.8lX\n", vecnum, vecname, last_pc, unmapped_va);
	//print_page_mappings((union Tile_PTE*)local_phys_to_mem(hv_inquire_context().page_table));

	__asm volatile("ill");

#if 0
	// get current page table, and print the fault page mapping
	HV_Context context = hv_inquire_context();
	lvaddr_t pt_addr = local_phys_to_mem(context.page_table);
	union Tile_PTE *l1_table = (union Tile_PTE*)pt_addr;
	union Tile_PTE entry = l1_table[HV_L1_INDEX(unmapped_va)];
	int is_large = 1;
	if(entry.pte.present && !entry.pte.page) {
		print_page(entry, page_align(unmapped_va, LARGE_PAGE_SIZE), 1);
		union Tile_PTE *l2_table = get_linked_l2_table_from_l1_entry(entry);
		entry = l2_table[HV_L2_INDEX(unmapped_va)];
		is_large = 0;
	}
	uint32_t page_size = is_large ? LARGE_PAGE_SIZE : BASE_PAGE_SIZE;
	uint32_t aligned_unmapped_va = page_align(unmapped_va, page_size);
	print_page(entry, aligned_unmapped_va, is_large);

	//TODO: page fault will be handled by Barrelfish lib,
	// which provides an interface in arch-specific entry.S

	struct dispatcher_shared_tilepro *disp_tilepro = get_dispatcher_shared_tilepro(dcb_current->disp);
	disp_tilepro->d.disabled = dispatcher_is_disabled_ip(dcb_current->disp, save_area->pc);

	struct dispatcher_shared_generic *disp_generic = get_dispatcher_shared_generic(dcb_current->disp);

	assert(dcb_current->disp_cte.cap.type == ObjType_Frame);

//	printf("dcb_current : %lX\n", (uint32_t)dcb_current);
//	printf("dcb_current->disp : %lX\n", (uint32_t)dcb_current->disp);
//	printf("disp_generic : %lX\n", (uint32_t)disp_generic);
//	printf("disp_tilepro : %lX\n", (uint32_t)disp_tilepro);
//	printf("save_area : %lX\n", (uint32_t)save_area);
//	arch_registers_state_t* r = &disp_tilepro->enabled_save_area;
//	printf("disp_tilepro->enabled_save_area : %lX\n", (uint32_t)r);
//	r = &disp_tilepro->trap_save_area;
//	printf("disp_tilepro->trap_save_area : %lX\n", (uint32_t)r);

	// needs to decide the current dispatcher is disabled or enbaled,
	// and then set up the dispatcher's entry point
	lvaddr_t disp_entry;
	if(disp_tilepro->d.disabled) {
		assert(save_area == &disp_tilepro->trap_save_area);
		disp_entry = disp_tilepro->d.dispatcher_pagefault_disabled;
		dcb_current->faults_taken++;  // XXX: is it important?
	} else {
		assert(save_area == &disp_tilepro->enabled_save_area);
		disp_entry = disp_tilepro->d.dispatcher_pagefault;
	}

	if(dcb_current->faults_taken > 2) {  // XXX: this special part, we need to check later
		printf("WARNING: page fault interrupt subroutine has too many faults, making domain unrunnable!\n");
		dcb_current->faults_taken = 0;  // just in case it gets restarted
		scheduler_remove(dcb_current);
		dispatch(schedule());
		tilepro_panic("page_fault user handler must not reach here!\n");
	} else {
		// we upcall the dispatcher
		struct registers_tilepro resume_area;

		// these four variables are the arguments of Barrelfish's page fault function
		resume_area.r0 = disp_generic->udisp;
		resume_area.r1 = aligned_unmapped_va;
		resume_area.r2 = 0;
		resume_area.r3 = save_area->pc;

		resume_area.pc = disp_entry;  // Barrelfish's page fault function entry
		resume_area.tp = disp_generic->udisp;
		resume_area.lr = save_area->lr;

		// SP will be set up in entry.S later

		// Upcall user to save area
		disp_tilepro->d.disabled = true;
		resume(&resume_area);
		tilepro_panic("page_fault user handler must not reach here!\n");
	}
#endif

	// fake, program won't reach here
	return 0;

	// Create a GPV, we can't write into the system save of PL3
	// PL0 is user, PL1 supervisor, PL2 hypervisor & PL3 hypervisor-debugger
	//__insn_mtspr(SPR_SYSTEM_SAVE_3_0, 0);

	//tilepro_panic("Should not reach this statement!\n");
	//__asm volatile("ill");

	//halt();
}
