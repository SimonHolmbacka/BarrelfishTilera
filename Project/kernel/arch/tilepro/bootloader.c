/*
 * loader.c
 *
 *  Created on: Dec 11, 2012
 *      Author: robert
 */

#include <hv/hypervisor.h>
#include <stdarg.h>
#include "loader_libc.h"
#include "mem_loc.h"
#include "bootloader_integrate.h"
#include <elf/elf.h>

#include "pte.h"


#define EM_TILEPRO	0xbc

extern char kernel_elf_image[];

extern char loader_l1[];
extern char loader_l2[];

static void print_page(union Tile_PTE pte, uint32_t va, int is_large)
{
	if (!pte.pte.present) {
		loader_print("NOT_PRESENT ");
	}
	if (is_large) {
		if (!pte.pte.page) {
			loader_print("LINK_TO_L2 ");
		}
	} else {
		if (pte.pte.page) {
			loader_print("PAGE_BIT_SET_BUT_IN_L2 ");
		}
	}
	loader_print_string_xint32("pte: ", va, "->");
	uint32_t pa = HV_PFN_TO_CPA(pte.pte.pfn);
	loader_print_string_xint32("", pa, " ");
	if (is_large) {
		loader_print("large ");
	}
	loader_print((pte.pte.readable) ?"r" : "-");
	loader_print((pte.pte.writable) ?"w" : "-");
	loader_print((pte.pte.executable) ?"x " : "- ");
	if (pte.pte.mode == 1) {
		loader_print("UNCACHED ");
	} else if (pte.pte.mode == 2) {
		loader_print("CACHE_NO_L3 ");
	} else if (pte.pte.mode == 3) {
		loader_print("CACHE_TILE_L3 ");
	} else if (pte.pte.mode == 4) {
		loader_print("CACHE_HASH_L3 ");
	} else if (pte.pte.mode == 2) {
		loader_print("CACHE_NO_L3 ");
	} else if (pte.pte.mode == 2) {
		loader_print("MMIO ");
	} else {
		loader_print("UNKNOWN_MODE ");
	}
	if (pte.pte.global) {
		loader_print("global ");
	}
	if (pte.pte.user) {
		loader_print("user ");
	}
	if (pte.pte.accessed) {
		loader_print("accessed ");
	}
	if (pte.pte.dirty) {
		loader_print("dirty ");
	}
	loader_print("\n");
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

static inline union Tile_PTE* get_linked_l2_table_from_l1_entry(union Tile_PTE l1_entry) {
	assert(l1_entry.pte.present);
	assert(!l1_entry.pte.page);
	lpaddr_t l2_paddr = HV_PFN_TO_CPA(l1_entry.pte.pfn) + HV_PTFN_TO_CPA(l1_entry.pte.ptfn_lower_part);
	lvaddr_t l2_vaddr;
	if (l2_paddr >= 0x01000000 && l2_paddr < 0x02000000) {
		l2_vaddr = l2_paddr - 0x01000000;
	} else {
		loader_print_string_xint32("Can't get virtual address for physical address: ", l2_paddr, "\n");
		assert(0);
	}
	union Tile_PTE *l2_table = (union Tile_PTE*)l2_vaddr;
	return l2_table;
}

void print_page_mappings(union Tile_PTE l1[HV_L1_ENTRIES])
{
	loader_print_string_xint32("Printing page table located at ", (uint32_t)l1, ":\n");
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

static int get_file_inode(char* file_name, HV_FS_StatInfo* file_info) {
	int inode = hv_fs_findfile((HV_VirtAddr) file_name);
	if (inode == HV_ENOENT) {
		panic("ERROR! File not found!");
	} else if (inode == HV_EFAULT) {
		panic("ERROR! Bad filename pointer!");
	} else {
		*file_info = hv_fs_fstat(inode);
	}
	return inode;
}

static void read_file(int inode, HV_FS_StatInfo file_info, char* buffer) {
	int err = hv_fs_pread(inode, (HV_VirtAddr) buffer, file_info.size, 0);
	if (err == HV_EBADF) {
		panic("ERROR! Reading file on a bad node!");
	} else if (err == HV_EFAULT) {
		loader_print_string_xint32("ERROR! Reading file to a bad buffer: ", (uint32_t) buffer, "\n");
		panic("");
	}
	assert(file_info.size == err);
}

errval_t alloc(void *state, genvaddr_t base, size_t size, uint32_t flags,
		void **ret) {
	// Text sections which are the same on every processor
	// are simply overwritten with the same data on every core startup.
	if (base == 0) {
		assert(size == 0);
		// Basically a 0, but that one would not be accepted.
		// This area is not mapped and should crash if something would be really written.
		*ret = (void*)1;
	} else if (base == MEM_LINK_DATA_START_V) {
		assert(size <= MEM_LINK_DATA_PER_CORE_SIZE);
		*ret = (void*)MEM_LINK_DATA_START_V;
	} else if (base == MEM_INTR_PL1_V) {
		assert(size <= MEM_INTR_PL1_SIZE);
		*ret = (void*)MEM_INTR_PL1_V;
	} else if (base == MEM_LINK_TEXT_START_V) {
		assert(size <= MEM_LINK_TEXT_END_V - MEM_LINK_TEXT_START_V);
		*ret = (void*)MEM_LINK_TEXT_START_V;
	} else {
		loader_print_string_xint32("ERROR! Unknown address: ", (uint32_t) base, "\n");
		panic();
	}
	loader_print_string_xint32("base: ", base, "; ");
	loader_print_string_xint32("size: ", size, "; ");
	loader_print_string_xint32("flags: ", flags, "; ");
	loader_print_string_xint32("ret: ", (uint32_t)*ret, "\n");
	return 0;
}

void first_core_init(int inode, HV_FS_StatInfo file_info) {
	read_file(inode, file_info, kernel_elf_image);
}

static inline uint32_t page_table_va_to_pa(uint32_t page_table_va) {
	assert(page_table_va >= MEM_PER_CORE_PAGE_TABLE_V);
	assert(page_table_va < MEM_PER_CORE_PAGE_TABLE_V + (256 * (HV_L1_SIZE + HV_L2_SIZE)));
	return page_table_va - MEM_BF_INT_START_V + MEM_BF_INT_START_P;
}

static void install_per_core_page_table(const unsigned int cpu_id) {
	size_t page_table_size = 1 << 11;
	union Tile_PTE *new_l2 = (void*)MEM_L2_PAGE_TABLE(cpu_id);
	union Tile_PTE *new_l1 = (void*)MEM_L1_PAGE_TABLE(cpu_id);
	memcpy(new_l2, loader_l2, HV_L2_SIZE);
	memcpy(new_l1, loader_l1, HV_L1_SIZE);

	uint32_t l2_vaddr = (uint32_t)new_l2;
	uint32_t l2_paddr = page_table_va_to_pa(l2_vaddr);
	new_l1[0].pte.ptfn_lower_part = HV_CPA_TO_PTFN(l2_paddr) & 0x1F;
	new_l1[0].pte.pfn = HV_CPA_TO_PFN(l2_paddr);
	// map the data section per core
	uint32_t pa_base = MEM_LINK_DATA_PER_CORE_START_P + cpu_id * MEM_LINK_DATA_PER_CORE_SIZE;
	for (int i = 0x0; i <= 0xF; i++) {
		uint32_t pa = pa_base + i * HV_PAGE_SIZE_SMALL;
		union Tile_PTE pte;
		pte.raw = 0;
		pte.pte.present = 1;
		pte.pte.global = 1;
		pte.pte.accessed = 1;
		pte.pte.dirty = 1;
		pte.pte.mode = HV_PTE_MODE_CACHE_NO_L3;
		pte.pte.readable = 1;
		pte.pte.writable = 1;
		pte.pte.pfn = HV_CPA_TO_PFN(pa);
		new_l2[0xF0+i] = pte;
	}
	HV_PTE access;
	access.val = HV_PTE_MODE_CACHE_NO_L3 << HV_PTE_INDEX_MODE;
	HV_ASID asid = hv_inquire_asid(0).start;
	__hv32 flags = 0;
	uint64_t page_table_address = page_table_va_to_pa((uint32_t)new_l1);
	hv_install_context(page_table_address, access, asid, flags);
	hv_flush_all(0);
	loader_print("Installed page table!\n");
}

void boot_loader(void);
void boot_loader(void) {
	HV_Topology topo = hv_inquire_topology();
	unsigned int cpu_id = topo.coord.y * topo.width + topo.coord.x;
	loader_print_string_uint32("cpu_id: ", cpu_id, "\n");

	HV_FS_StatInfo file_info;
	//TILEmpower boot changes:
	//int inode = get_file_inode("tmp/sd_boot/cpu", &file_info);
	int inode = get_file_inode("./tilepro/sbin/cpu", &file_info);

	if (!cpu_id) {
		first_core_init(inode, file_info);
	}
	install_per_core_page_table(cpu_id);

	uint32_t entry_point;

	int ret = elf32_load(EM_TILEPRO, alloc, NULL, (lvaddr_t) kernel_elf_image,
			file_info.size, (genvaddr_t*) &entry_point,
			NULL, NULL, NULL);
	loader_print_string_xint32("entry_point: ", entry_point, "\n");

	__asm volatile("jr %[entry_point] \n\t"
			:
			: [entry_point] "r" (entry_point));
}
