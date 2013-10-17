/**
 * \file
 * \brief Paging definitions for tilepro.
 */

/*
 * Copyright (c) 2010, ETH Zurich.
 * All rights reserved.
 *
 * This file is distributed under the terms in the attached LICENSE file.
 * If you do not find this file, copies can be found by writing to:
 * ETH Zurich D-INFK, Haldeneggsteig 4, CH-8092 Zurich. Attn: Systems Group.
 */

#ifndef TARGET_TILEPRO_BARRELFISH_KPI_PAGING_TARGET_H
#define TARGET_TILEPRO_BARRELFISH_KPI_PAGING_TARGET_H

#include <hv/hypervisor.h>

typedef uint64_t paging_tilepro_flags_t;

/* Default page size is 64K */
#define BASE_PAGE_BITS          16
#define BASE_PAGE_SIZE          (1u << BASE_PAGE_BITS)
#define BASE_PAGE_MASK          (BASE_PAGE_SIZE - 1)
#define BASE_PAGE_OFFSET(a)     ((a) & BASE_PAGE_MASK)

/* 16MB large pages */
#define LARGE_PAGE_BITS         24
#define LARGE_PAGE_SIZE         (1u << LARGE_PAGE_BITS)
#define LARGE_PAGE_MASK         (LARGE_PAGE_SIZE - 1)
#define LARGE_PAGE_OFFSET(a)    ((a) & LARGE_PAGE_MASK)

/* page table size */
#define BASE_PAGE_TABLE_BITS	(HV_LOG2_L2_SIZE)
#define BASE_PAGE_TABLE_SIZE	(HV_L2_SIZE)
#define LARGE_PAGE_TABLE_BITS	(HV_LOG2_L1_SIZE)
#define LARGE_PAGE_TABLE_SIZE	(HV_L1_SIZE)

/* page table entries */
#define BASE_PAGE_TABLE_ENTRIES		(HV_L2_ENTRIES)
#define LARGE_PAGE_TABLE_ENTRIES	(HV_L1_ENTRIES)

/* page table mask */
#define BASE_PAGE_TABLE_MASK	(BASE_PAGE_TABLE_ENTRIES - 1)
#define LARGE_PAGE_TABLE_MASK 	(LARGE_PAGE_TABLE_ENTRIES - 1)

/*
 * MACRO to calculate page table base address
 */
#define BASE_PAGE_TABLE_BASE(va)	(((uint32_t)(va) >> (BASE_PAGE_BITS)) & (BASE_PAGE_TABLE_MASK))
#define LARGE_PAGE_TABLE_BASE(va) 	(((uint32_t)(va) >> (LARGE_PAGE_BITS)) & (LARGE_PAGE_TABLE_MASK))

#define TILEPRO_L1_OFFSET(addr)       ((((uintptr_t)addr) >> 24) & 0xff)
#define TILEPRO_L2_OFFSET(addr)       ((((uintptr_t)addr) >> 16) & 0xff)
#define TILEPRO_PAGE_OFFSET(addr)     ((uintptr_t)addr & 0xfff)

#define TILEPRO_L1_MAX_ENTRIES              256u // 2 ^ (32 - LARGE_PAGE_BITS) = 2 ^ (32-24) = 2 ^ 8
#define TILEPRO_L2_MAX_ENTRIES              256u // 2 ^ (LARGE_PAGE_BITS - BASE_PAGE_BITS) = 2 ^ (24-16) = 2 ^ 8

/**
 * Bits within the various page directories and tables.
 */
#define TILEPRO_PTE_PRESENT			(((paging_tilepro_flags_t)1) << HV_PTE_INDEX_PRESENT)
#define TILEPRO_PTE_PAGE			(((paging_tilepro_flags_t)1) << HV_PTE_INDEX_PAGE)
#define TILEPRO_PTE_GLOBAL			(((paging_tilepro_flags_t)1) << HV_PTE_INDEX_GLOBAL)
#define TILEPRO_PTE_USER			(((paging_tilepro_flags_t)1) << HV_PTE_INDEX_USER)
#define TILEPRO_PTE_ACCESSED		(((paging_tilepro_flags_t)1) << HV_PTE_INDEX_ACCESSED)
#define TILEPRO_PTE_DIRTY			(((paging_tilepro_flags_t)1) << HV_PTE_INDEX_DIRTY)
#define TILEPRO_PTE_UNCACHED		(((paging_tilepro_flags_t)1) << HV_PTE_INDEX_MODE)
#define TILEPRO_PTE_NO_L1_CACHE		(((paging_tilepro_flags_t)1) << HV_PTE_INDEX_NO_ALLOC_L1)
#define TILEPRO_PTE_NO_L2_CACHE		(((paging_tilepro_flags_t)1) << HV_PTE_INDEX_NO_ALLOC_L2)
#define TILEPRO_PTE_NO_L3_CACHE		(((paging_tilepro_flags_t)2) << HV_PTE_INDEX_MODE)

#define TILEPRO_PTE_READABLE		(((paging_tilepro_flags_t)1) << HV_PTE_INDEX_READABLE)
#define TILEPRO_PTE_WRITABLE		(((paging_tilepro_flags_t)1) << HV_PTE_INDEX_WRITABLE)
#define TILEPRO_PTE_EXECUTABLE		(((paging_tilepro_flags_t)1) << HV_PTE_INDEX_EXECUTABLE)

#define PTABLE_ENTRY_SIZE   sizeof(union Tile_PTE)

#endif /* TARGET_TILEPRO_BARRELFISH_KPI_PAGING_TARGET_H */
