/*
 * hv_test.ld.S
 *
 *  Created on: Aug 8, 2012
 *      Author: robert
 */

#include <hv/hypervisor.h>
#include "../../include/arch/tilepro/mem_loc.h"

OUTPUT_ARCH(tile)
ENTRY(_start)

PHDRS
{
	text PT_LOAD;
	data PT_LOAD;
}

// Sections of the linker script ordered by physical address
SECTIONS
{
	.hvglue (MEM_HVGLUE_V) (NOLOAD) : AT (MEM_HVGLUE_P)
	{
		*(.hvglue)
	} :data

	// Text starts after .hvglue
	.text (MEM_LINK_BOOTLOADER_TEXT_V) : AT (MEM_LINK_BOOTLOADER_TEXT_P)
	{
		*(.text)
	} :text
	_end_text = .;

	.rodata (MEM_LINK_BOOTLOADER_DATA_V) : AT (MEM_LINK_BOOTLOADER_DATA_P)
	{
		*(.rodata)
	} :data

	// .data starts after .rodata
	.data :
	{
		*(.data)
	} :data
	_end_data = .;
}
