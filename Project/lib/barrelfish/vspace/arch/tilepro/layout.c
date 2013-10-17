/**
 * \file
 * \brief
 */

/*
 * Copyright (c) 2010, ETH Zurich.
 * All rights reserved.
 *
 * This file is distributed under the terms in the attached LICENSE file.
 * If you do not find this file, copies can be found by writing to:
 * ETH Zurich D-INFK, Haldeneggsteig 4, CH-8092 Zurich. Attn: Systems Group.
 */

#include <barrelfish/barrelfish.h>
#include "../../vspace_internal.h"

/**
 * \brief Initialize the tilepro vspace layout
 * vspace represents the page table of tilepro arch.
 */
errval_t vspace_layout_init(struct vspace_layout *l)
{
	l->offset = 0;
	l->granularity = BASE_PAGE_SIZE;
	// 4GB virtual space
	l->size = ((genvaddr_t)LARGE_PAGE_TABLE_ENTRIES * BASE_PAGE_TABLE_ENTRIES * BASE_PAGE_SIZE);
	return SYS_ERR_OK;
}
