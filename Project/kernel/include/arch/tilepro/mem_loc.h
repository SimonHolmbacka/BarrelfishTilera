/*
 * memory.h
 *
 *  Created on: Aug 8, 2012
 *      Author: robert
 */

#ifndef ARCH_TILEPRO_MEM_LOC_H_
#define ARCH_TILEPRO_MEM_LOC_H_

#include <hv/hypervisor.h>
#include <arch/abi.h>


/****************************
* Kernel space + user space *
* **************************/

// This is the dynamically allocated memory - the first parts
// are even allocated by the kernel.

// the kernel space is 2GB
#define MEM_KERNEL_START_V 0x80000000
#define MEM_KERNEL_START_P 0x13000000

#define MEM_KERNEL_SIZE 0x0A000000

#define MEM_KERNEL_BSP_CORE_SIZE 0x10000000
#define MEM_KERNEL_APP_CORE_SIZE 0x04000000

#define MEM_KERNEL_END_V 0xFC000000

// the virtual user space is around 2GB
#define MEM_DYN_USER_START_V 0x01000000
#define MEM_DYN_USER_END_V (MEM_KERNEL_START_V)

/*******************
* Stack definition *
* *****************/

#define MEM_KERNEL_STACK_MEM_START_V 0xFC000000
#define MEM_KERNEL_STACK_MEM_START_P 0x02000000

#define MEM_KERNEL_STACK_SIZE (HV_PAGE_SIZE_LARGE)

#define MEM_KERNEL_STACK_POINTER_V ((MEM_KERNEL_STACK_MEM_START_V) + (MEM_KERNEL_STACK_SIZE))

#define PER_TILE_KERNEL_STACK_LOG2 16
#define PER_TILE_KERNEL_STACK (1 << (PER_TILE_KERNEL_STACK_LOG2))

#define MAX_TILES ((MEM_KERNEL_STACK_SIZE) / (PER_TILE_KERNEL_STACK))

// user process stack top, we put it at the end of user space section, growing backwards
#define MEM_USER_STACK_TOP (MEM_DYN_USER_END_V)
#define MEM_USER_STACK_SIZE 0x00100000 // 1M

/************
* Data part *
************/

// .rodata, .data & .bss get linked in starting at this address
#define MEM_LINK_DATA_START_V 0x00F00000
#define MEM_LINK_DATA_PER_CORE_START_P 0x03000000

#define MEM_LINK_DATA_PER_CORE_SIZE 0x100000 // 1 MB

// The maximum amount address of data, to be linked in here.
// Should be checked at startup, so it is not exceeded,
// because it is put in the boot-up page table statically.
#define MEM_LINK_DATA_END_V 0x01000000
#define MEM_LINK_DATA_PER_CORE_END_P 0x13000000

/*****************************************
* Code part including interrupt vectors, *
* hvglue and text section,               *
* totally 1 large page size              *
*****************************************/

#define MEM_WHOLE_TEXT_PAGE_V 0xFD000000
#define MEM_WHOLE_TEXT_PAGE_P 0x00000000

#define MEM_INTR_PL1_V	  0xFD000000
#define MEM_INTR_PL1_P 	  0x00000000
#define MEM_INTR_PL1_SIZE (HV_GLUE_START_CPA)

// start of hv_glue, directly after the PL 1 in the same huge page
#define MEM_HVGLUE_V 	((MEM_INTR_PL1_V) + (MEM_INTR_PL1_SIZE))
#define MEM_HVGLUE_P 	(HV_GLUE_START_CPA)  // defined in hypervisor
#define MEM_HVGLUE_SIZE (HV_GLUE_RESERVED_SIZE)

// end of hv_glue, text area starts after that
#define MEM_LINK_BOOTLOADER_TEXT_V 0xFD020000
#define MEM_LINK_BOOTLOADER_TEXT_P 0x00020000

// So, the text section lies in the first huge page. We had a problem
// booting (and calling hv_init) when it wasn't there.
#define MEM_LINK_TEXT_START_V 0xFD030000
#define MEM_LINK_TEXT_START_P 0x00030000

// The maximum amount address of code, to be linked in here.
// Should be checked at startup, so it is not exceeded,
// because it is put in the page table statically.
#define MEM_LINK_TEXT_END_V 0xFE000000
#define MEM_LINK_TEXT_END_P 0x01000000

#define MEM_LINK_TEXT_VA_TO_PA(MEM) ((MEM) - (MEM_LINK_TEXT_START_V) + (MEM_LINK_TEXT_START_P))
#define MEM_LINK_TEXT_PA_TO_VA(MEM) ((MEM) - (MEM_LINK_TEXT_START_P) + (MEM_LINK_TEXT_START_V))

/***********************
* Barrelfish internal  *
* including bootloader *
***********************/

/*
 * one large page for bootinfo, arguments and dispatcher data.
 */
#define MEM_BF_INTERNAL_LARGE_V 0x00000000
#define MEM_BF_INTERNAL_LARGE_P 0x01000000


#define MEM_BF_INT_START_V 0x00010000
#define MEM_BF_INT_START_P 0x01010000

#define MEM_BF_INT_BARRELFISH_PART_END_V ((MEM_BF_INT_START_V) + (1 << 18) + (1 << 17) + (1 << 21))
#define MEM_BF_INT_MULTIBOOT_DATA_V (MEM_BF_INT_BARRELFISH_PART_END_V)
#define MEM_BF_INT_MULTIBOOT_DATA_P ((MEM_BF_INT_MULTIBOOT_DATA_V) - (MEM_BF_INTERNAL_LARGE_V) + (MEM_BF_INTERNAL_LARGE_P))

#define MEM_LINK_BOOTLOADER_DATA_V 0x00A00000
#define MEM_LINK_BOOTLOADER_DATA_P 0x01A00000

#define MEM_LINK_BOOTLOADER_DATA_VA_TO_PA(MEM) ((MEM) - (MEM_LINK_BOOTLOADER_DATA_V) + (MEM_LINK_BOOTLOADER_DATA_P))

#define MEM_PER_CORE_PAGE_TABLE_V 0x00C00000
#define MEM_PER_CORE_PAGE_TABLE_P ((MEM_BF_INTERNAL_LARGE_P) + (MEM_PER_CORE_PAGE_TABLE_V))

#define MEM_L2_PAGE_TABLE(cpu_id) ((MEM_PER_CORE_PAGE_TABLE_V) + cpu_id * (HV_L2_SIZE + HV_L1_SIZE))
#define MEM_L1_PAGE_TABLE(cpu_id) ((MEM_PER_CORE_PAGE_TABLE_V) + cpu_id * (HV_L2_SIZE + HV_L1_SIZE) + HV_L2_SIZE)

#define MEM_BOOTLOADER_L2_V ((MEM_LINK_BOOTLOADER_DATA_V) + 0x800)


#endif /* ARCH_TILEPRO_MEM_LOC_H_ */
