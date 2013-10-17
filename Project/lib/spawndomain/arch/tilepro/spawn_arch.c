/**
 * \file
 * \brief functionality to spawn domains
 */

/*
 * Copyright (c) 2007, 2008, 2009, 2010, ETH Zurich.
 * All rights reserved.
 *
 * This file is distributed under the terms in the attached LICENSE file.
 * If you do not find this file, copies can be found by writing to:
 * ETH Zurich D-INFK, Haldeneggsteig 4, CH-8092 Zurich. Attn: Systems Group.
 */

#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include <barrelfish/barrelfish.h>
#include <spawndomain/spawndomain.h>
#include <barrelfish/dispatcher_arch.h>
#include <elf/elf.h>
#include "spawn.h"
#include "../../arch.h"

#include <asmoffsets.h>

#if defined(__tilepro__)
#define EM_HOST EM_TILEPRO  // XXX: we add EM_TILEPRO in elf.h
#else
#error "Unexpected architecture."
#endif

/**
 * \brief Convert elf flags to vregion flags
 */
static vregion_flags_t elf_to_vregion_flags(uint32_t flags)
{
	vregion_flags_t vregion_flags = 0;

    if(flags & PF_R) {
    	vregion_flags |= VREGION_FLAGS_READ;
    }
    if(flags & PF_W) {
        vregion_flags |= VREGION_FLAGS_WRITE;
    }
    if(flags & PF_X) {
        vregion_flags |= VREGION_FLAGS_EXECUTE;
    }

    return vregion_flags;
}

/*
 * memory allocation function used for elf loading.
 */
static errval_t elf_allocate(void *state, genvaddr_t base, size_t size, uint32_t flags, void **retbase)
{
	errval_t err;

	struct spawninfo *si = state;

	// Increase size by space wasted on first page due to page-alignment
	size_t base_offset = BASE_PAGE_OFFSET(base);
	size += base_offset;
	base -= base_offset;
	// Page-align
	size = ROUND_UP(size, BASE_PAGE_SIZE);

	cslot_t vspace_slot = si->elfload_slot;
	cslot_t spawn_vspace_slot = si->elfload_slot;

	// Allocate the frames
	size_t sz = 0;
	for(lpaddr_t offset = 0; offset < size; offset += sz) {
		sz = 1UL << log2floor(size - offset);
		struct capref frame = {
				.cnode = si->segcn,
				.slot  = si->elfload_slot++,
		};
		err = frame_create(frame, sz, NULL);
		if(err_is_fail(err)) {
			return err_push(err, LIB_ERR_FRAME_CREATE);
		}
	}

	/* Map into my vspace */

	struct memobj *memobj = malloc(sizeof(struct memobj_anon));
	if(!memobj) {
		return LIB_ERR_MALLOC_FAIL;
	}
	struct vregion *vregion = malloc(sizeof(struct vregion));
	if(!vregion) {
		return LIB_ERR_MALLOC_FAIL;
	}

	// Create the objects, initialize memobj.
	err = memobj_create_anon((struct memobj_anon*)memobj, size, 0);
	if(err_is_fail(err)) {
		return err_push(err, LIB_ERR_MEMOBJ_CREATE_ANON);
	}

	// set up vregion
	err = vregion_map(vregion, get_current_vspace(), memobj, 0, size, VREGION_FLAGS_READ_WRITE);
	if(err_is_fail(err)) {
		return err_push(err, LIB_ERR_VSPACE_MAP);
	}

	for(lvaddr_t offset = 0; offset < size; offset += sz) {
		sz = 1UL << log2floor(size - offset);
		struct capref frame = {
				.cnode = si->segcn,
				.slot  = vspace_slot++,
		};
		genvaddr_t genvaddr = vspace_lvaddr_to_genvaddr(offset);
		err = memobj->f.fill(memobj, genvaddr, frame, sz);
		if(err_is_fail(err)) {
			return err_push(err, LIB_ERR_MEMOBJ_FILL);
		}
		err = memobj->f.pagefault(memobj, vregion, offset, 0);
		if(err_is_fail(err)) {
			DEBUG_ERR(err, "lib_err_memobj_pagefault_handler");
			return err_push(err, LIB_ERR_MEMOBJ_PAGEFAULT_HANDLER);
		}
	}

	/* Map into spawn vspace */

	struct memobj *spawn_memobj = NULL;
	struct vregion *spawn_vregion = NULL;

	err = spawn_vspace_map_anon_fixed_attr(si, base, size, &spawn_vregion, &spawn_memobj, elf_to_vregion_flags(flags));
	if(err_is_fail(err)) {
		return err_push(err, SPAWN_ERR_VSPACE_MAP);
	}

	for (lvaddr_t offset = 0; offset < size; offset += sz) {
		sz = 1UL << log2floor(size - offset);
		struct capref frame = {
				.cnode = si->segcn,
				.slot  = spawn_vspace_slot++,
		};
		genvaddr_t genvaddr = vspace_lvaddr_to_genvaddr(offset);
		err = memobj->f.fill(spawn_memobj, genvaddr, frame, sz);
		if(err_is_fail(err)) {
			return err_push(err, LIB_ERR_MEMOBJ_FILL);
		}
		err = spawn_memobj->f.pagefault(spawn_memobj, spawn_vregion, offset, 0);
		if(err_is_fail(err)) {
			DEBUG_ERR(err, "lib_err_memobj_pagefault_handler");
			return err_push(err, LIB_ERR_MEMOBJ_PAGEFAULT_HANDLER);
		}
	}

	genvaddr_t genvaddr = vregion_get_base_addr(vregion) + base_offset;
	*retbase = (void*)vspace_genvaddr_to_lvaddr(genvaddr);

	return SYS_ERR_OK;
}

/**
 * \brief Load the elf image
 */
errval_t spawn_arch_load(struct spawninfo *si, lvaddr_t binary, size_t binary_size, genvaddr_t *entry, void **arch_info)
{
	errval_t err;

	// Reset the elfloader_slot
	si->elfload_slot = 0;

	struct capref cnode_cap = {
			.cnode = si->rootcn,
			.slot  = ROOTCN_SLOT_SEGCN,
	};

	err = cnode_create_raw(cnode_cap, &si->segcn, DEFAULT_CNODE_SLOTS, NULL);
	if(err_is_fail(err)) {
		return err_push(err, SPAWN_ERR_CREATE_SEGCN);
	}

	// TODO: find out if we need tls?
	si->tls_init_base = 0;
	si->tls_init_len = si->tls_total_len = 0;

	// load the binary
	err = elf_load(EM_HOST, elf_allocate, si, binary, binary_size, entry);
	if(err_is_fail(err)) {
		return err;
	}

	return SYS_ERR_OK;
}

void spawn_arch_set_registers(void *arch_load_info,
		                          dispatcher_handle_t handle,
		                          arch_registers_state_t *enabled_area,
		                          arch_registers_state_t *disabled_area)
{
	enabled_area->tp = get_dispatcher_shared_generic(handle)->udisp;
	enabled_area->lr = 0;
	enabled_area->sp = get_dispatcher_shared_generic(handle)->udisp + OFFSETOF_DISP_GENERIC + OFFSETOF_DISP_PRIV_STACK_LIMIT;

	disabled_area->tp = get_dispatcher_shared_generic(handle)->udisp;
	disabled_area->lr = 0;
	// set up the stack pointer in user-mode dispatcher's stack, which has also ready been mapped into vspace
	disabled_area->sp = get_dispatcher_shared_generic(handle)->udisp + OFFSETOF_DISP_GENERIC + OFFSETOF_DISP_PRIV_STACK_LIMIT;
}
