/*
 * page_table.h
 *
 *  Created on: Sep 26, 2012
 *      Author: jackie
 */

#ifndef PAGE_TABLE_H_
#define PAGE_TABLE_H_

#include <hv/hypervisor.h>
#include <pte.h>
#include <barrelfish_kpi/registers_arch.h>
#include <barrelfish_kpi/tilepro_core_data.h>


/*
 * Write a PTE to the L1 page table
 *
 * @param va the virtual address of the memory, to which this PTE points. Needs not to be aligned.
 * @param pte the entry to write
 */
void write_l1_pte(lvaddr_t va, union Tile_PTE pte);

/*
 * Write a PTE to the L2 page table, assuming that the L1 page table is already mapped.
 *
 * @param va the virtual address of the memory, to which this PTE points. Needs not to be aligned.
 * @param pte the entry to write
 */
void write_l2_pte(lvaddr_t va, union Tile_PTE pte);

/*
 * Creates a new PTE for the L1 page table, which just links to an L2 table
 *
 * @param pa the physical address of the l2 page table.
 * @return a new entry
 */
union Tile_PTE create_l1_link_to_l2_pte(lpaddr_t addr_of_l2_table);

/*
 * Creates a new PTE for data, with default global settings
 *
 * @param pa the physical address to map to. Needs to be aligned when using in an L1 table.
 * @param is_page true if "page" should be set. Needed for L1 PTEs, which are describe a large page.
 * @return a new entry
 */
union Tile_PTE create_global_data_pte(lpaddr_t pa, int is_page);

/*
 * Creates a new PTE for code, with default global settings
 *
 * @param pa the physical address to map to. Needs to be aligned when using in an L1 table.
 * @param is_page true if "page" should be set. Needed for L1 PTEs, which are describe a large page.
 * @return a new entry
 */
union Tile_PTE create_global_code_pte(lpaddr_t pa, int is_page);

/**
 * Creates a new PTE for accessing physical memory. The mode is set to HV_PTE_MODE_CACHE_NO_L3
 *
 * @return a new entry
 */
union Tile_PTE create_access_pte(void);

/*
 * Creates a new PTE for code, with default settings, for use in a L2 page table
 *
 * @param pa the physical address to map to.
 * @param elf_flags the flags, according to elf/elf.h
 * @return a new entry
 */
union Tile_PTE create_pte_according_to_elf_flags(lpaddr_t pa, uint32_t elf_flags);

/**
 * Gets the physical address of the L1 page table.
 *
 * @return the physical address
 */
lpaddr_t get_l1_page_table_pa(void);

/*
 * print out L1 page table
 */
void print_boot_l1_page_table(void);

void print_page_mappings(union Tile_PTE l1[HV_L1_ENTRIES]);


/**
 * Creates the mappings for the kernel memory (upper 2GB) for this core in the boot L1
 *
 * @param bsp_range The physical memory range for the BSP core
 * @param range The physical memory range for this core (or NULL, if on bsp core)
 */
void create_kernel_mappings_in_boot_l1(struct physical_mem_range *bsp_range, struct physical_mem_range *range);

/*
 * this function is to copy all statically-mapped entries from kernel's L1 page table into a L1 page table
 *
 * @param l1 the L1 page table, which should get the mappings
 */
void copy_kernel_mappings_to_l1_page_table(union Tile_PTE* l1);

/**
 * The interrupt service routine.
 *
 * @param vecnum, the number of the interrupt
 * @param vecname, the name of the interrupt
 * @param unmapped_va, the virtual address, which was not mapped
 * @return TODO define a contract for the return value
 */
int page_fault(int vecnum, char vecname[], uint32_t unmapped_va, arch_registers_state_t* save_area);


#endif /* PAGE_TABLE_H_ */
