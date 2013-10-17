/*
 * dispatcher_arch.h
 *
 *  Created on: Aug 17, 2012
 *      Author: robert
 */

#ifndef ARCH_TILEPRO_BARRELFISH_DISPATCHER_H
#define ARCH_TILEPRO_BARRELFISH_DISPATCHER_H

#include <target/tilepro/barrelfish/dispatcher_target.h>


static inline struct dispatcher_generic*
get_dispatcher_generic(dispatcher_handle_t handle)
{
	struct dispatcher_tilepro *disp_tilepro = (struct dispatcher_tilepro*)handle;
	return &disp_tilepro->generic;
}

static inline size_t get_dispatcher_size(void)
{
	return sizeof(struct dispatcher_tilepro);
}

#endif /* ARCH_TILEPRO_BARRELFISH_DISPATCHER_H */
