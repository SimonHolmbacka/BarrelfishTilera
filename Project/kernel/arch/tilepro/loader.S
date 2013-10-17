/*
 * bootloader.S
 *
 *  Created on: Dec 11, 2012
 *      Author: robert
 */

#include "hv/hypervisor.h"
#include "pte.h"
#include "mem_loc.h"

.macro	console_print label
	moveli  r0, lo16(\label)
	auli    r0, r0, ha16(\label)
	moveli r1, .L\()\label - \label
	jal    hv_console_write
.endm

.macro	before_mmu_console_print label
	moveli r0, lo16(MEM_LINK_BOOTLOADER_DATA_VA_TO_PA(\label))
	auli r0, r0, ha16(MEM_LINK_BOOTLOADER_DATA_VA_TO_PA(\label))
	// difference should be the same
	moveli r1, .L\()\label - \label
	jal    hv_console_write
.endm

.section .rodata, "a"

first_instr_msg:
	.ascii  "Bootloader starting.\n"
.Lfirst_instr_msg:

hv_init_msg:
	.ascii  "Called hv_init.\n"
.Lhv_init_msg:

wrong_asid_msg:
	.ascii  "Got no valid ASID range.\n"
.Lwrong_asid_msg:

right_asid_msg:
	.ascii  "Got a valid ASID range.\n"
.Lright_asid_msg:

context_success_msg:
	.ascii  "Successfully installed boot-up context.\n"
.Lcontext_success_msg:

too_much_tiles:
	.ascii  "Too much tiles loaded, don't have enough stack space.\n"
.Ltoo_much_tiles:

.text
.global _start
_start:
	.align 8

	before_mmu_console_print first_instr_msg

	movei r0, _HV_VERSION
	movei r1, TILE_CHIP /* TilePro chip version */
	movei r2, TILE_CHIP_REV /* TilePro chip revision */
	/* void hv_init(HV_VersionNumber (32bit) interface_version_number,
             int chip_num, int chip_rev_num); */
	jal hv_init

	before_mmu_console_print hv_init_msg

	move r0, zero
	/* HV_ASIDRange (64bit) hv_inquire_asid(int idx); */
	jal hv_inquire_asid

	// Check if HV_ASIDRange.size > 0, otherwise result is invalid
	bgzt r1, 1f
	before_mmu_console_print wrong_asid_msg
	jal hv_halt
1:
	// The start of the inquired asid range is used as asid for this table
	move r4, r0
	before_mmu_console_print right_asid_msg
    // Page table described in CPA - 64 bit
	moveli r0, lo16(MEM_LINK_BOOTLOADER_DATA_VA_TO_PA(loader_l1))
	auli r0, r0, ha16(MEM_LINK_BOOTLOADER_DATA_VA_TO_PA(loader_l1))
	move r1, zero
	// Don't use the L3
	move r2, zero
	auli r2, zero, hi16(HV_PTE_MODE_CACHE_NO_L3 << HV_PTE_INDEX_MODE)
	move r3, zero
	// No flags
	move r5, zero
	// Set the link adress, which will jump to after hv_install_context finishes.
	// We need this, because hv_install_context does address translations,
	// so the link register won't fit afterwards.
	// We link to the correct virtual address and after the setup,
	// the virtual address will work fine.
	moveli lr, lo16(2f)
	auli lr, lr, ha16(2f)
	/* int hv_install_context(HV_PhysAddr (64bit) page_table, HV_PTE (64bit) access, HV_ASID (32bit) asid,
                       __hv32 flags); */
	j hv_install_context
2:
	// Check if successful
	bzt r0, 3f
	jal hv_halt
3:
	console_print context_success_msg

	moveli sp, lo16(MEM_KERNEL_STACK_POINTER_V)
	auli sp, sp, ha16(MEM_KERNEL_STACK_POINTER_V)
	addi sp, sp, -C_ABI_SAVE_AREA_SIZE

	/* HV_Topology (128bit; x, y, width, height) hv_inquire_topology(void); */
	jal hv_inquire_topology

	// r0 = y * width + x; r0 = r1 * r2 + r0
	// multiply accumulate low unsigned low unsigned half word
	// y & width are positive and should be < 65536
	mullla_uu r0, r1, r2

	// cpu number is saved	 in this special register
	mtspr SYSTEM_SAVE_1_0, r0

	// Load MAX_TILES into r1
	moveli r1, lo16(MAX_TILES)
	auli r1, r1, ha16(MAX_TILES)

	// r1 = MAX_TILES - tile number
	sub r1, r1, r0
	// Check if successful
	// branch greater zero taken
	bgzt r1, 4f
	console_print too_much_tiles
	jal hv_halt
4:

	// Shift by log2 of the stack size,
	// which means multiply with the stack size. That is the stack space for one tile.
	// Due to the multiplying with the stack number, every tile gets this space.
	shli r0, r0, PER_TILE_KERNEL_STACK_LOG2

	// Grow back on the stack
	sub sp, sp, r0

	movei lr, 0

	/* void boot_loader() */
	j boot_loader

//.section .rodata, "a"
.data
.align HV_L2_SIZE

#define TIMES_16(entry, x) \
	entry(x##0) \
	entry(x##1) \
	entry(x##2) \
	entry(x##3) \
	entry(x##4) \
	entry(x##5) \
	entry(x##6) \
	entry(x##7) \
	entry(x##8) \
	entry(x##9) \
	entry(x##A) \
	entry(x##B) \
	entry(x##C) \
	entry(x##D) \
	entry(x##E) \
	entry(x##F)

#define MAP_LOADER_DATA(x) PAGE_TABLE_ENTRY_64K(loader_l2, MEM_LINK_BOOTLOADER_DATA_V + 0x##x##0000, \
	MEM_LINK_BOOTLOADER_DATA_P + 0x##x##0000, HV_PTE_READABLE | HV_PTE_WRITABLE, 0)
#define MAP_PER_CORE_PAGE_TABLES(x)	PAGE_TABLE_ENTRY_64K(loader_l2, MEM_PER_CORE_PAGE_TABLE_V + 0x##x##0000, \
	MEM_PER_CORE_PAGE_TABLE_P + 0x##x##0000, HV_PTE_READABLE | HV_PTE_WRITABLE, 0)
.global loader_l2
loader_l2:
		// DISPATCHER memory & Co. is mapped dynamically into kernel space

		PAGE_TABLE_ENTRY_64K(loader_l2, MEM_BF_INT_MULTIBOOT_DATA_V, MEM_BF_INT_MULTIBOOT_DATA_P, HV_PTE_READABLE | HV_PTE_WRITABLE, HV_PTE_USER)

		TIMES_16(MAP_LOADER_DATA, )
		MAP_LOADER_DATA(10)

		// Have 16 entries for a section, where the l1 + l2 table are placed, calculated by cpu id.
		// One table is 2^11. 2^11 * 256 = 512k = 8 pages. So 16 pages for 256 L1 and L2 tables.
		TIMES_16(MAP_PER_CORE_PAGE_TABLES, )

		// rodata + data is mapped dynamically in bootloader.c
.org loader_l2 + (HV_L2_SIZE)

// No alignment here because HV_L1 & HV_L2 have the same size. // .align HV_L1_SIZE
// We need to have a fixed difference between loader_l1 & loader_l2, to be able to map to that.
// If they would differ, .size would be better than .align

.global loader_l1
loader_l1:
		// BF internal + rodata
		PAGE_TABLE_ENTRY_LINK_TO_L2(loader_l1, MEM_BF_INTERNAL_LARGE_V, MEM_LINK_BOOTLOADER_DATA_VA_TO_PA(MEM_BOOTLOADER_L2_V))

		// Kernel memory is mapped dynamically per core

		// map stacks section
		PAGE_TABLE_ENTRY_16M(loader_l1, MEM_KERNEL_STACK_MEM_START_V, MEM_KERNEL_STACK_MEM_START_P, HV_PTE_READABLE | HV_PTE_WRITABLE, 0)
		// map text section(s)
		PAGE_TABLE_ENTRY_16M(loader_l1, MEM_WHOLE_TEXT_PAGE_V, MEM_WHOLE_TEXT_PAGE_P, HV_PTE_READABLE | HV_PTE_WRITABLE | HV_PTE_EXECUTABLE, 0)
.org loader_l1 + HV_L1_SIZE

.section .bss
.global kernel_elf_image
kernel_elf_image:
	// 1MB space for the kernel image - which is put here once
.org kernel_elf_image + (1 << 20)

// Here we assume that the MEM_BOOTLOADER_L2_V is equal to loader_l2. We need this to point from loader_l1 to loader_l2.
// We can't point with symbols, because there are calculations (shifts) needed in assembler, but the address is set at linking time.
// This check variable sets the difference and is tested to be 00000000 in the build script
mem_l2_check = loader_l2 - MEM_BOOTLOADER_L2_V

.end
