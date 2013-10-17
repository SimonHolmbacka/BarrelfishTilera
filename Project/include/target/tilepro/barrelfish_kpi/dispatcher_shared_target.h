/*
 * dispatcher_shared_target.h
 *
 *  Created on: Aug 20, 2012
 *      Author: robert
 */

#ifndef TARGET_TILEPRO_BARRELFISH_KPI_DISPATCHER_SHARED_H
#define TARGET_TILEPRO_BARRELFISH_KPI_DISPATCHER_SHARED_H

#include <barrelfish_kpi/dispatcher_shared.h>

struct dispatcher_shared_tilepro {
    struct dispatcher_shared_generic d; ///< Generic portion

    lvaddr_t    crit_pc_low;        ///< Critical section lower PC bound
    lvaddr_t    crit_pc_high;       ///< Critical section upper PC bound

    uint8_t	asid;  ///< The ASID of the process.

    // these two topological variables here are used to determine the core id
    uint8_t tile_width;
    uint8_t tile_height;

    // this is to return an incoming channel id for this dispatcher
    // we don't have a upper limit for this value, so it could be 2^32-1, but indeed it is impossible
    // TODO: set a reasonable upper constraint for this value
    uint32_t in_chan_count;

    struct registers_tilepro enabled_save_area;  ///< Enabled register save area
    struct registers_tilepro disabled_save_area; ///< Disabled register save area
    struct registers_tilepro trap_save_area;     ///< Trap register save area
};

static inline struct dispatcher_shared_tilepro*
get_dispatcher_shared_tilepro(dispatcher_handle_t handle)
{
    return (struct dispatcher_shared_tilepro*)handle;
}

static inline uint8_t
dispatcher_tilepro_get_asid(dispatcher_handle_t handle)
{
    struct dispatcher_shared_tilepro *disp =
    		get_dispatcher_shared_tilepro(handle);
    return disp->asid;
}

static inline void
dispatcher_tilepro_set_asid(dispatcher_handle_t handle, uint8_t asid)
{
    struct dispatcher_shared_tilepro *disp =
    		get_dispatcher_shared_tilepro(handle);
    disp->asid = asid;
}

static inline uint8_t
dispatcher_tilepro_get_tile_width(dispatcher_handle_t handle)
{
    struct dispatcher_shared_tilepro *disp =
    		get_dispatcher_shared_tilepro(handle);
    return disp->tile_width;
}

static inline void
dispatcher_tilepro_set_tile_width(dispatcher_handle_t handle, uint8_t width)
{
    struct dispatcher_shared_tilepro *disp =
    		get_dispatcher_shared_tilepro(handle);
    disp->tile_width = width;
}

static inline uint8_t
dispatcher_tilepro_get_tile_height(dispatcher_handle_t handle)
{
    struct dispatcher_shared_tilepro *disp =
    		get_dispatcher_shared_tilepro(handle);
    return disp->tile_height;
}

static inline void
dispatcher_tilepro_set_tile_height(dispatcher_handle_t handle, uint8_t height)
{
    struct dispatcher_shared_tilepro *disp =
    		get_dispatcher_shared_tilepro(handle);
    disp->tile_height = height;
}

static inline void
dispatcher_tilepro_start_in_chan_count(dispatcher_handle_t handle)
{
    struct dispatcher_shared_tilepro *disp = get_dispatcher_shared_tilepro(handle);
    disp->in_chan_count = 0;
}

static inline uint32_t
dispatcher_tilepro_get_in_chan_id(dispatcher_handle_t handle)
{
    struct dispatcher_shared_tilepro *disp = get_dispatcher_shared_tilepro(handle);
    // return the current number for new incoming channel ID, and increment by one then for next use
    return disp->in_chan_count++;
}

#endif /* TARGET_TILEPRO_BARRELFISH_KPI_DISPATCHER_SHARED_H */
