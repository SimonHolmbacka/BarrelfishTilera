/*
 * pmap_arch.h
 *
 *  Created on: Aug 17, 2012
 *      Author: robert
 */

#ifndef ARCH_TILEPRO_BARRELFISH_PMAP_H
#define ARCH_TILEPRO_BARRELFISH_PMAP_H

#include <target/tilepro/barrelfish/pmap_target.h>

#include <assert.h>

#define ARCH_DEFAULT_PMAP_SIZE sizeof(struct pmap_tilepro)

errval_t pmap_init(struct pmap *pmap,
				   struct vspace *vspace,
				   struct capref vnode,
				   struct slot_allocator *opt_slot_alloc);

errval_t pmap_current_init(bool init_domain);

#endif /* ARCH_TILEPRO_BARRELFISH_PMAP_H */
