/*
 * hv_test.ld.S
 *
 *  Created on: Aug 8, 2012
 *      Author: robert
 */

#include <hv/hypervisor.h>
#include "../../include/arch/tilepro/mem_loc.h"

OUTPUT_ARCH(tile)
ENTRY(start_kernel)

PHDRS
{
	intr_pl1 PT_LOAD;
	text PT_LOAD;
	data PT_LOAD;
}

// Sections of the linker script ordered by physical address
SECTIONS
{
	// The interrupt vector of protection level 1
	.intr_pl1 (MEM_INTR_PL1_V) :
	{
		*(.intr_pl1)
	} :intr_pl1 =0

	.hvglue (MEM_HVGLUE_V) (NOLOAD) :
	{
		*(.hvglue)
	} :data

	// Text starts after .hvglue
	.text (MEM_LINK_TEXT_START_V) :
	{
		*(.text)
	} :text
	_end_text = .;

	.rodata (MEM_LINK_DATA_START_V) :
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
