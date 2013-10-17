/*
 * dispatcher_target.h
 *
 *  Created on: Aug 20, 2012
 *      Author: robert
 */

#ifndef TARGET_TILEPRO_BARRELFISH_DISPATCHER_H
#define TARGET_TILEPRO_BARRELFISH_DISPATCHER_H

#include <barrelfish_kpi/dispatcher_shared.h>
#include <barrelfish_kpi/dispatcher_shared_arch.h>
#include <barrelfish/dispatcher.h>

/// Dispatcher structure (including data accessed only by user code)
struct dispatcher_tilepro {
    struct dispatcher_shared_tilepro d;  ///< Shared (user/kernel) data. Must be first.
    struct dispatcher_generic generic;   ///< User private data
    /* Incoming LMP endpoints (buffers and receive cap pointers) follow */
};


#endif /* TARGET_TILEPRO_BARRELFISH_DISPATCHER_H */
