/*
 * \brief tilepro's pmap management
 *
 * pmap_arch.c
 *
 *  Created on: Nov 30, 2012
 *      Author: jackie
 */

#include <barrelfish/barrelfish.h>
#include <barrelfish/caddr.h>

// Location and size of virtual address space reserved for mapping
// frames backing refill_slabs
#define META_DATA_RESERVED_BASE ((lvaddr_t)1UL * 1024 * 1024 * 1024)
#define META_DATA_RESERVED_SIZE (BASE_PAGE_SIZE * 64)

static size_t max_slabs_required(size_t bytes)
{
	// Perform a slab allocation for every page (do_map -> slab_alloc)
	size_t base_pages = DIVIDE_ROUND_UP(bytes, BASE_PAGE_SIZE);
	// Perform a slab allocation for every L2 (get_ptable -> find_vnode)
	size_t l2_page_tables = DIVIDE_ROUND_UP(base_pages, BASE_PAGE_TABLE_ENTRIES);
	// Perform a slab allocation for every L1 (do_map -> find_vnode)
	size_t l1_page_tables = DIVIDE_ROUND_UP(l2_page_tables, LARGE_PAGE_TABLE_ENTRIES);
	return (base_pages + l2_page_tables + l1_page_tables);
}

/*
 * this function is to set up page table entry flags based on the vregion's flags
 */
static paging_tilepro_flags_t vregion_to_pmap_flags(vregion_flags_t flags)
{
	// at beginning, set the common attributes
	paging_tilepro_flags_t pmap_flags = TILEPRO_PTE_PRESENT | TILEPRO_PTE_USER | TILEPRO_PTE_DIRTY;

	if(!(flags & VREGION_FLAGS_GUARD)) {
		if(flags & VREGION_FLAGS_READ) {  // read allowed
			pmap_flags |= TILEPRO_PTE_READABLE;
		}
		if(flags & VREGION_FLAGS_WRITE) {  // write allowed
			pmap_flags |= TILEPRO_PTE_WRITABLE;
		}
		if(flags & VREGION_FLAGS_EXECUTE) {  // execute allowed
			pmap_flags |= TILEPRO_PTE_EXECUTABLE;
		}
		if(flags & VREGION_FLAGS_NOCACHE) {  // no cache
			pmap_flags |= TILEPRO_PTE_UNCACHED;
		} else {
			// here we have to set one mode for tilera?
			// we allow L1 & L2 cache, and shut down L3 cache.
			pmap_flags |= TILEPRO_PTE_NO_L3_CACHE;
		}
		// in tilera arch, there is nothing to do with message passing buffer,
		// so we ignore the flag VREGION_FLAGS_MPB.
	}
	return pmap_flags;
}

/**
 * \brief Allocates a new VNode, adding it to the page table and our metadata
 */
static errval_t alloc_vnode(struct pmap_tilepro *pmap, struct vnode *root, enum objtype type, uint32_t entry, struct vnode **retvnode)
{
	errval_t err;

	// allocate memory for new vnode
	struct vnode *new_vnode = slab_alloc(&pmap->slab);
	if(new_vnode == NULL ) {
		return LIB_ERR_SLAB_ALLOC_FAIL;
	}

	// get the new vnode's capability
	err = slot_alloc(&new_vnode->u.vnode.cap);
	if(err_is_fail(err)) {
		return err_push(err, LIB_ERR_SLOT_ALLOC);
	}

	// create a new vnode according to cap and type
	err = vnode_create(new_vnode->u.vnode.cap, type);
	if(err_is_fail(err)) {
		return err_push(err, LIB_ERR_VNODE_CREATE);
	}

	// map entry to root (page table)
	err = vnode_map(root->u.vnode.cap, new_vnode->u.vnode.cap, entry, TILEPRO_PTE_PRESENT | TILEPRO_PTE_USER | TILEPRO_PTE_DIRTY, 0, 1);
	if(err_is_fail(err)) {
		return err_push(err, LIB_ERR_VNODE_MAP);
	}

	// initialize vnode meta data
	new_vnode->is_vnode = true;
	new_vnode->entry = entry;
	new_vnode->next = root->u.vnode.children;
	root->u.vnode.children = new_vnode;
	new_vnode->u.vnode.children = NULL;

	if(retvnode) {
		*retvnode = new_vnode;
	}
	return SYS_ERR_OK;
}

/**
 * \brief Starting at a given root, return the vnode with entry equal to #entry
 */
static struct vnode *find_vnode(struct vnode *root, uint32_t entry)
{
	assert(root != NULL);
	assert(root->is_vnode);
	struct vnode *n;

	for(n = root->u.vnode.children; n != NULL ; n = n->next) {
		if(n->entry == entry) {
			return n;
		}
	}
	return NULL;
}

/**
 * \brief Returns the vnode for the pagetable mapping a given vspace address
 */
static errval_t get_ptable(struct pmap_tilepro *pmap, genvaddr_t vaddr, struct vnode **ptable)
{
	uintptr_t index = TILEPRO_L1_OFFSET(vaddr);

    if((*ptable = find_vnode(&pmap->root, index)) == NULL) {
    	struct vnode *tmp = NULL; // Tmp variable for passing to alloc_vnode

        errval_t err = alloc_vnode(pmap, &pmap->root, ObjType_VNode_tilepro_l2, index, &tmp);
        if(err_is_fail(err)) {
            DEBUG_ERR(err, "alloc_vnode");
            return err;
        }
        assert(tmp != NULL);
        *ptable = tmp; // Set argument to received value

        if(err_is_fail(err)) {
            return err_push(err, LIB_ERR_PMAP_ALLOC_VNODE);
        }
    }

    return SYS_ERR_OK;
}

static void remove_vnode(struct vnode *root, struct vnode *item)
{
	assert(root->is_vnode);
	struct vnode *walk = root->u.vnode.children;
	struct vnode *prev = NULL;
	while(walk) {
		if(walk == item) {
			if(prev) {
				prev->next = walk->next;
				return;
			} else {
				root->u.vnode.children = walk->next;
				return;
			}
		}
		prev = walk;
		walk = walk->next;
	}
	USER_PANIC("Should not get here");
}

static bool has_vnode(struct vnode *root, uint32_t entry, size_t len)
{
    assert(root != NULL);
    assert(root->is_vnode);
    struct vnode *n;

    uint32_t end_entry = entry + len;

    for (n = root->u.vnode.children; n; n = n->next) {
        if (n->is_vnode && n->entry == entry) {
            return true;
        }
        // n is frame
        uint32_t end = n->entry + n->u.frame.pte_count;
        if (n->entry < entry && end > end_entry) {
            return true;
        }
        if (n->entry >= entry && n->entry < end_entry) {
            return true;
        }
    }

    return false;
}

static errval_t do_single_map(struct pmap_tilepro *pmap,
		                         genvaddr_t vaddr,
		                         genvaddr_t vend,
		                         struct capref frame,
		                         size_t offset,
		                         size_t pte_count,
		                         vregion_flags_t flags)
{
	// translate flags
	paging_tilepro_flags_t pmap_flags = vregion_to_pmap_flags(flags);

	// Get the page table
	struct vnode *ptable;
	errval_t err = get_ptable(pmap, vaddr, &ptable);
	if(err_is_fail(err)) {
		return err_push(err, LIB_ERR_PMAP_GET_PTABLE);
	}

	// check if there is an overlapping mapping
	if(has_vnode(ptable, TILEPRO_L2_OFFSET(vaddr), pte_count)) {
		printf("page already exists in 0x%"PRIxGENVADDR"--0x%"PRIxGENVADDR"\n", vaddr, vend);
		return LIB_ERR_PMAP_EXISTING_MAPPING;
	}

	// setup userspace mapping
	struct vnode *page = slab_alloc(&pmap->slab);
	assert(page);
	page->is_vnode = false;
	page->entry = TILEPRO_L2_OFFSET(vaddr);
	page->next = ptable->u.vnode.children;
	ptable->u.vnode.children = page;
	page->u.frame.cap = frame;
	page->u.frame.offset = offset;
	page->u.frame.flags = flags;
	page->u.frame.pte_count = pte_count;

	// do map
	err = vnode_map(ptable->u.vnode.cap, frame, TILEPRO_L2_OFFSET(vaddr), pmap_flags, offset, pte_count);
	if(err_is_fail(err)) {
		return err_push(err, LIB_ERR_VNODE_MAP);
	}

	return SYS_ERR_OK;
}

/*
 * continue to do the mapping following map(...)
 */
static errval_t do_map(struct pmap_tilepro *pmap,
		                 genvaddr_t vaddr,
		                 struct capref frame,
		                 size_t offset,
		                 size_t size,
		                 vregion_flags_t flags,
		                 size_t *retoff,
		                 size_t *retsize)
{
	errval_t err;

	size = ROUND_UP(size, BASE_PAGE_SIZE);
	size_t pte_count = DIVIDE_ROUND_UP(size, BASE_PAGE_SIZE);
	genvaddr_t vend = vaddr + size;

	if(TILEPRO_L1_OFFSET(vaddr) == TILEPRO_L1_OFFSET(vend - 1)) {
		// fast path
		err = do_single_map(pmap, vaddr, vend, frame, offset, pte_count, flags);
		if(err_is_fail(err)) {
			DEBUG_ERR(err, "[do_map] in fast path");
			return err_push(err, LIB_ERR_PMAP_DO_MAP);
		}
	} else { // multiple leaf page tables
		// first leaf
		uint32_t c = TILEPRO_L2_MAX_ENTRIES - TILEPRO_L2_OFFSET(vaddr);
		genvaddr_t temp_end = vaddr + c * BASE_PAGE_SIZE;
		err = do_single_map(pmap, vaddr, temp_end, frame, offset, c, flags);
		if(err_is_fail(err)) {
			return err_push(err, LIB_ERR_PMAP_DO_MAP);
		}

		// map full leaves
		while(TILEPRO_L1_OFFSET(temp_end) < TILEPRO_L1_OFFSET(vend)) { // update vars
			vaddr = temp_end;
			temp_end = vaddr + TILEPRO_L2_MAX_ENTRIES * BASE_PAGE_SIZE;
			offset += c * BASE_PAGE_SIZE;
			c = TILEPRO_L2_MAX_ENTRIES;
			// copy cap
			struct capref next;
			err = slot_alloc(&next);
			if(err_is_fail(err)) {
				return err_push(err, LIB_ERR_PMAP_DO_MAP);
			}
			err = cap_copy(next, frame);
			if (err_is_fail(err)) {
				return err_push(err, LIB_ERR_PMAP_DO_MAP);
			}
			frame = next;

			// do mapping
			err = do_single_map(pmap, vaddr, temp_end, frame, offset, TILEPRO_L2_MAX_ENTRIES, flags);
			if(err_is_fail(err)) {
				return err_push(err, LIB_ERR_PMAP_DO_MAP);
			}
		}

		// map remaining part
		offset += c * BASE_PAGE_SIZE;
		c = TILEPRO_L2_OFFSET(vend) - TILEPRO_L2_OFFSET(temp_end);
		if(c) {
			// copy cap
			struct capref next;
			err = slot_alloc(&next);
			if(err_is_fail(err)) {
				return err_push(err, LIB_ERR_PMAP_DO_MAP);
			}
			err = cap_copy(next, frame);
			if(err_is_fail(err)) {
				return err_push(err, LIB_ERR_PMAP_DO_MAP);
			}

			// do mapping
			err = do_single_map(pmap, temp_end, vend, next, offset, c, flags);
			if(err_is_fail(err)) {
				return err_push(err, LIB_ERR_PMAP_DO_MAP);
			}
		}
	}
	if(retoff) {
		*retoff = offset;
	}
	if(retsize) {
		*retsize = size;
	}
	//has_vnode_debug = false;
	return SYS_ERR_OK;

#if 0
	errval_t err;
	uintptr_t pmap_flags = vregion_flags_to_kpi_paging_flags(flags);

	for (size_t i = offset; i < offset + size; i += BASE_PAGE_SIZE) {

		vaddr += BASE_PAGE_SIZE;
	}

	if (retoff) {
		*retoff = offset;
	}
	if (retsize) {
		*retsize = size;
	}
	return SYS_ERR_OK;
#endif
}

/**
 * \brief Refill slabs used for metadata
 *
 * \param pmap     The pmap to refill in
 * \param request  The number of slabs the allocator must have
 * when the function returns
 *
 * When the current pmap is initialized,
 * it reserves some virtual address space for metadata.
 * This reserved address space is used here
 *
 * Can only be called for the current pmap
 * Will recursively call into itself till it has enough slabs
 */
static errval_t refill_slabs(struct pmap_tilepro *pmap, size_t request)
{
	errval_t err;

	/* Keep looping till we have #request slabs */
	while(slab_freecount(&pmap->slab) < request) {
		// Amount of bytes required for #request
		size_t bytes = SLAB_STATIC_SIZE((request - slab_freecount(&pmap->slab)), sizeof(struct vnode));

		/* Get a frame of that size */
		struct capref cap;
		err = frame_alloc(&cap, bytes, &bytes);
		if(err_is_fail(err)) {
			return err_push(err, LIB_ERR_FRAME_ALLOC);
		}

		/* If we do not have enough slabs to map the frame in, recurse */
		size_t required_slabs_for_frame = max_slabs_required(bytes);
		if(slab_freecount(&pmap->slab) < required_slabs_for_frame) {
			// If we recurse, we require more slabs than to map a single page
			assert(required_slabs_for_frame > 4);
			err = refill_slabs(pmap, required_slabs_for_frame);
			if(err_is_fail(err)) {
				return err_push(err, LIB_ERR_SLAB_REFILL);
			}
		}

		/* Perform mapping */
		genvaddr_t genvaddr = pmap->vregion_offset;
		pmap->vregion_offset += (genvaddr_t) bytes;
		assert(pmap->vregion_offset < (vregion_get_base_addr(&pmap->vregion) + vregion_get_size(&pmap->vregion)));

		err = do_map(pmap, genvaddr, cap, 0, bytes, VREGION_FLAGS_READ_WRITE, NULL, NULL);
		if(err_is_fail(err)) {
			return err_push(err, LIB_ERR_PMAP_DO_MAP);
		}

		/* Grow the slab */
		lvaddr_t buf = vspace_genvaddr_to_lvaddr(genvaddr);
		slab_grow(&pmap->slab, (void*)buf, bytes);
	}
	return SYS_ERR_OK;
}

/*
 * Determine a suitable address for a given memory object
 *
 * @param pmap (in), The pmap object
 * @param memobj (in), input, The memory object to determine the address for
 * @param alignment (in), input, Minimum alignment
 * @param retvaddr (out), Pointer to return the determined address
 *
 * Relies on vspace.c code maintaining an ordered list of vregions
 */
static errval_t tilepro_determine_addr(struct pmap *pmap, struct memobj *memobj, size_t alignment, genvaddr_t *retvaddr)
{
	// get the first vregion from the beginning of vspace
	struct vregion *current = pmap->vspace->head;
	assert(current != NULL); // assume there's always at least one existing entry

	// check the alignment value, making sure it is the multiple of base page size
	if(alignment == 0) {
		alignment = BASE_PAGE_SIZE;
	} else {
		alignment = ROUND_UP(alignment, BASE_PAGE_SIZE);
	}

	// align the size of memory object
	size_t memobj_size = ROUND_UP(memobj->size, alignment);

	while(current->next) { // Try to insert between existing mappings
		genvaddr_t current_base = vregion_get_base_addr(current);
		genvaddr_t current_size = ROUND_UP(vregion_get_size(current), BASE_PAGE_SIZE);
		genvaddr_t next_base = vregion_get_base_addr(current->next);

		// sanity-check for page alignment
		assert(current_base % BASE_PAGE_SIZE == 0);
		assert(next_base % BASE_PAGE_SIZE == 0);

		if((next_base > current_base + current_size + memobj_size) &&
		   (current_base + current_size > META_DATA_RESERVED_BASE)) { // Ensure mappings are larger than META_DATA_RESERVED_BASE
			*retvaddr = ROUND_UP(current_base + current_size, alignment);
			return SYS_ERR_OK;
		}

		current = current->next;
	}  // the end of while-loop

	// if could not find the address, place it to the last one
	// TODO: do we need to have a upper bound for this address?
	*retvaddr = ROUND_UP(vregion_get_base_addr(current) + vregion_get_size(current), alignment);
	assert(retvaddr != NULL);
	return SYS_ERR_OK;
}

/**
 * \brief Create page mappings
 *
 * \param pmap     The pmap object
 * \param vaddr    The virtual address to create the mapping for
 * \param frame    The frame cap to map in
 * \param offset   Offset into the frame cap
 * \param size     Size of the mapping
 * \param flags    Flags for the mapping
 * \param retoff   If non-NULL, filled in with adjusted offset of mapped region
 * \param retsize  If non-NULL, filled in with adjusted size of mapped region
 */
static errval_t tilepro_map(struct pmap* pmap,
		                      genvaddr_t vaddr,
		                      struct capref frame,
		                      size_t offset,
		                      size_t size,
		                      vregion_flags_t flags,
		                      size_t *retoffset,
		                      size_t *retsize)
{
	// before we carry out the real page mapping,
	// we are gonna do something to avoid errors

	struct pmap_tilepro *pmap_tilepro = (struct pmap_tilepro*) pmap;

	size += BASE_PAGE_OFFSET(offset);
	size = ROUND_UP(size, BASE_PAGE_SIZE);
	offset -= BASE_PAGE_OFFSET(offset);

	// if the slab allocator has less memory than required,
	// then we need to get more memory.
	const size_t slabs_reserve = 3; // == max_slabs_required(1)
	uint64_t slabs_free = slab_freecount(&pmap_tilepro->slab);
	size_t slabs_required = max_slabs_required(size) + slabs_reserve;
	if(slabs_free < slabs_required) {
		if(pmap == get_current_pmap()) {
			errval_t err = refill_slabs(pmap_tilepro, slabs_required);
			if(err_is_fail(err)) {
				return err_push(err, LIB_ERR_SLAB_REFILL);
			}
		} else {
			size_t bytes = SLAB_STATIC_SIZE((slabs_required - slabs_free), sizeof(struct vnode));
			void *buf = malloc(bytes);
			if(!buf) {
				return LIB_ERR_MALLOC_FAIL;
			}
			slab_grow(&pmap_tilepro->slab, buf, bytes);
		}
	}

	// do the real mapping
	return do_map(pmap_tilepro, vaddr, frame, offset, size, flags, retoffset, retsize);
}

static struct vnode *find_ptable(struct pmap_tilepro *pmap, genvaddr_t vaddr)
{
	uintptr_t index = TILEPRO_L1_OFFSET(vaddr);
    return find_vnode(&pmap->root, index);
}

static errval_t do_single_unmap(struct pmap_tilepro *pmap, genvaddr_t vaddr, size_t pte_count, bool delete_cap)
{
    errval_t err;
    struct vnode *pt = find_ptable(pmap, vaddr);
    if(pt) {
    	// analog to do_single_map we use 10 bits for tracking pages in user space -SG
        struct vnode *page = find_vnode(pt, TILEPRO_L2_OFFSET(vaddr));
        if(page && page->u.frame.pte_count == pte_count) {
            err = vnode_unmap(pt->u.vnode.cap, page->u.frame.cap, page->entry, page->u.frame.pte_count);
            if(err_is_fail(err)) {
                DEBUG_ERR(err, "vnode_unmap");
                return err_push(err, LIB_ERR_VNODE_UNMAP);
            }

            // Free up the resources
            if(delete_cap) {
                err = cap_destroy(page->u.frame.cap);
                if(err_is_fail(err)) {
                    return err_push(err, LIB_ERR_PMAP_DO_SINGLE_UNMAP);
                }
            }
            remove_vnode(pt, page);
            slab_free(&pmap->slab, page);
        } else {
            return LIB_ERR_PMAP_FIND_VNODE;
        }
    }
    return SYS_ERR_OK;
}

/**
 * \brief Remove page mappings
 *
 * \param pmap     The pmap object
 * \param vaddr    The start of the virtual addres to remove
 * \param size     The size of virtual address to remove
 * \param retsize  If non-NULL, filled in with the actual size removed
 */
static errval_t tilepro_unmap(struct pmap *pmap, genvaddr_t vaddr, size_t size, size_t *retsize)
{
    errval_t err, ret = SYS_ERR_OK;
    struct pmap_tilepro *pmap_tilepro = (struct pmap_tilepro*)pmap;
    size = ROUND_UP(size, BASE_PAGE_SIZE);
    size_t pte_count = size / BASE_PAGE_SIZE;
    genvaddr_t vend = vaddr + size;

    if(TILEPRO_L1_OFFSET(vaddr) == TILEPRO_L1_OFFSET(vend - 1)) {
    	// fast path
        err = do_single_unmap(pmap_tilepro, vaddr, pte_count, false);
        if(err_is_fail(err)) {
            return err_push(err, LIB_ERR_PMAP_UNMAP);
        }
    } else { // slow path
        // unmap first leaf
        uint32_t c = TILEPRO_L2_MAX_ENTRIES - TILEPRO_L2_OFFSET(vaddr);
        err = do_single_unmap(pmap_tilepro, vaddr, c, false);
        if(err_is_fail(err)) {
            return err_push(err, LIB_ERR_PMAP_UNMAP);
        }

        // unmap full leaves
        vaddr += c * BASE_PAGE_SIZE;
        while(TILEPRO_L1_OFFSET(vaddr) < TILEPRO_L1_OFFSET(vend)) {
            c = TILEPRO_L2_MAX_ENTRIES;
            err = do_single_unmap(pmap_tilepro, vaddr, c, true);
            if(err_is_fail(err)) {
                return err_push(err, LIB_ERR_PMAP_UNMAP);
            }
            vaddr += c * BASE_PAGE_SIZE;
        }

        // unmap remaining part
        c = TILEPRO_L2_OFFSET(vend) - TILEPRO_L2_OFFSET(vaddr);
        if(c) {
        	err = do_single_unmap(pmap_tilepro, vaddr, c, true);
            if(err_is_fail(err)) {
            	return err_push(err, LIB_ERR_PMAP_UNMAP);
            }
        }
    }

    if(retsize) {
    	*retsize = size;
    }

    return ret;
}

/*
 *
 */
static errval_t tilepro_modify_flags(struct pmap* pmap,
		                                 genvaddr_t vaddr,
		                                 size_t size,
		                                 vregion_flags_t flags,
		                                 size_t *retsize)
{
	assert(!"implement me");
	return SYS_ERR_OK;
}

/*
 *
 */
static errval_t tilepro_lookup(struct pmap *pmap,
		                          genvaddr_t vaddr,
		                          genvaddr_t *retvaddr,
		                          size_t *retsize,
		                          struct capref *retcap,
		                          genvaddr_t *retoffset,
		                          vregion_flags_t *retflags)
{
	assert(!"implement me");
	return SYS_ERR_OK;
}

// structure to save the info of the node in vtree,
// used for tree serialization.
struct serial_entry {
	uint16_t entry;     ///< Entry
	uint8_t depth;      ///< Depth of this node (0 = root)
	cslot_t slot;       ///< Slot number (in page cnode) of vnode cap
};

/*
 * core tree serialization function.
 */
static errval_t serialise_tree(int depth, struct vnode *v, struct serial_entry *out, size_t outlen, size_t *outpos)
{
	assert(v != NULL);
	errval_t err;

	// don't serialise leaf pages (yet!)
	if(!v->is_vnode) {
		return SYS_ERR_OK;
	}

	if(*outpos >= outlen) {
		return LIB_ERR_SERIALISE_BUFOVERFLOW;
	}

	// serialise this node
	out[(*outpos)++] = (struct serial_entry) {
		.depth = depth,
		.entry = v->entry,
		.slot = v->u.vnode.cap.slot
	};

	// depth-first walk
	for(struct vnode *c = v->u.vnode.children; c != NULL ; c = c->next) {
		err = serialise_tree(depth + 1, c, out, outlen, outpos);
		if(err_is_fail(err)) {
			return err;
		}
	}

	return SYS_ERR_OK;
}

/*
 * Serialise vtree to a flat structure, for passing to another process
 * This is used by spawn_vspace to communicate the vnode capabilities to the child.
 *
 * @param pmap, current pmap structure;
 * @param buf (out), returned buffer storing the serialized vtree;
 * @param buflen, buffer length.
 */
static errval_t tilepro_serialise(struct pmap *pmap, void *buf, size_t buflen)
{
	errval_t err;
	struct pmap_tilepro *pmap_tilepro = (struct pmap_tilepro*) pmap;

	// XXX: check alignment of buffer
	assert((uintptr_t )buf % sizeof(uintptr_t) == 0);

	struct serial_entry *out = buf;
	size_t outlen = buflen / sizeof(struct serial_entry);
	size_t outpos = 0;

	// this is the real tree serialization function.
	err = serialise_tree(0, &pmap_tilepro->root, out, outlen, &outpos);
	if (err_is_ok(err)) {
		// store length in first entry's slot number
		assert(out[0].slot == 0);
		out[0].slot = outpos;
	}

	return err;
}

/*
 * core tree deserialization function.
 */
static errval_t deserialise_tree(struct pmap *pmap, struct serial_entry **in, size_t *inlen, int depth, struct vnode *parent)
{
	errval_t err;

	struct pmap_tilepro *pmap_tilepro = (struct pmap_tilepro*)pmap;

	if(*inlen == 0) {
		return SYS_ERR_OK;
	}

	while((*inlen > 0) && ((*in)->depth == depth)) {
		// ensure slab allocator has sufficient space
		err = refill_slabs(pmap_tilepro, 5);  // referred to x86 code
		if(err_is_fail(err)) {
			return err_push(err, LIB_ERR_SLAB_REFILL);
		}

		// allocate storage for the new vnode
		struct vnode *n = slab_alloc(&pmap_tilepro->slab);
		assert(n != NULL);

		// populate it and append to parent's list of children
		n->is_vnode  = true;
		n->entry = (*in)->entry;
		n->u.vnode.cap.cnode = cnode_page;
		n->u.vnode.cap.slot = (*in)->slot;
		n->u.vnode.children = NULL;
		n->next = parent->u.vnode.children;
		parent->u.vnode.children = n;
		(*in)++;
		(*inlen)--;

		// is next entry a child of the last node?
		if((*inlen > 0) && ((*in)->depth > depth)) {
			assert((*in)->depth == depth + 1); // depth-first, no missing nodes
			err = deserialise_tree(pmap, in, inlen, depth + 1, n);
			if(err_is_fail(err)) {
				return err;
			}
		}
	}

	assert((*in)->depth < depth);
	return SYS_ERR_OK;
}

/**
 * \brief Deserialise vtree from a flat structure, for importing from another process
 * This is used in a newly-spawned child
 */
static errval_t tilepro_deserialise(struct pmap *pmap, void *buf, size_t buflen)
{
	errval_t err;

	struct pmap_tilepro *pmap_tilepro = (struct pmap_tilepro*) pmap;

	// XXX: check alignment of buffer
	assert((uintptr_t )buf % sizeof(uintptr_t) == 0);

	// extract length and sanity-check
	struct serial_entry *in = buf;
	assert(buflen > sizeof(struct serial_entry));
	size_t inlen = in[0].slot;
	assert(inlen * sizeof(struct serial_entry) <= buflen);
	in++;
	inlen--;

	err = deserialise_tree(pmap, &in, &inlen, 1, &pmap_tilepro->root);
	return err;
}

/*
 *
 */
static errval_t tilepro_dump(struct pmap *pmap, struct pmap_dump_info *buf, size_t buflen, size_t *items_written)
{
	assert(!"implement me");
	return SYS_ERR_OK;
}

/*
 * structure for all pmap functions.
 */
static struct pmap_funcs pmap_funcs = {
		.determine_addr = tilepro_determine_addr,
		.map = tilepro_map,
		.unmap = tilepro_unmap,
		.modify_flags = tilepro_modify_flags,
		.lookup = tilepro_lookup,
		.serialise = tilepro_serialise,
		.deserialise = tilepro_deserialise,
		.dump = tilepro_dump
};

/*
 * initialize the pmap, including slab allocator and vnode root
 */
errval_t pmap_init(struct pmap *pmap,
		            struct vspace *vspace,
		            struct capref vnode,
		            struct slot_allocator *opt_slot_alloc)
{
	struct pmap_tilepro* pmap_tilepro = (struct pmap_tilepro*) pmap;

	// generic pmap part
	pmap->f = pmap_funcs;
	pmap->vspace = vspace;

	// slot allocator is optional
	if(opt_slot_alloc != NULL ) {
		pmap->slot_alloc = opt_slot_alloc;
	} else {
		// use default allocator for this dispatcher
		pmap->slot_alloc = get_default_slot_allocator();
	}

	// tilepro-specific pmap part
	// initialize slab allocator
	slab_init(&pmap_tilepro->slab, sizeof(struct vnode), NULL);
	slab_grow(&pmap_tilepro->slab, pmap_tilepro->slab_buffer, sizeof(pmap_tilepro->slab_buffer));

	// initialize vnode root
	pmap_tilepro->root.is_vnode = true;
	pmap_tilepro->root.u.vnode.cap = vnode;
	pmap_tilepro->root.next = NULL;
	pmap_tilepro->root.u.vnode.children = NULL;

	return SYS_ERR_OK;
}

/*
 * initialize the vregion and vspace
 */
errval_t pmap_current_init(bool init_domain)
{
	struct pmap_tilepro *pmap_tilepro = (struct pmap_tilepro*)get_current_pmap();

	// To reserve a block of virtual address space,
	// a vregion representing the address space is required.
	// We construct a superficial one here and add it to the vregion list.
	struct vregion *vregion = &pmap_tilepro->vregion;
	assert((void* )vregion > (void* )pmap_tilepro);
	assert((void* )vregion < (void* )(pmap_tilepro + 1));
	vregion->vspace = NULL;
	vregion->memobj = NULL;
	vregion->base = META_DATA_RESERVED_BASE;
	vregion->size = META_DATA_RESERVED_SIZE;
	vregion->offset = 0;
	vregion->flags = 0;
	vregion->next = NULL;

	struct vspace *vspace = pmap_tilepro->p.vspace;
	assert(!vspace->head);
	vspace->head = vregion;

	pmap_tilepro->vregion_offset = pmap_tilepro->vregion.base;

	return SYS_ERR_OK;
}
