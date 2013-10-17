/*
 * dispatcher_shared_arch.h
 *
 *  Created on: Aug 17, 2012
 *      Author: robert
 */

#ifndef ARCH_TILEPRO_BARRELFISH_KPI_DISPATCHER_SHARED_ARCH_H_
#define ARCH_TILEPRO_BARRELFISH_KPI_DISPATCHER_SHARED_ARCH_H_

#include <target/tilepro/barrelfish_kpi/dispatcher_shared_target.h>

/**
 * \brief Returns whether dispatcher is currently disabled, given IP.
 *
 * \param disp  Pointer to dispatcher
 * \param ip    User-level instruction pointer.
 *
 * \return true if dispatcher disabled, false otherwise.
 */
static inline bool dispatcher_is_disabled_ip(dispatcher_handle_t handle, uintptr_t rip)
{
	struct dispatcher_shared_generic *disp = get_dispatcher_shared_generic(handle);
    /* one crit_pc pair */
    struct dispatcher_shared_tilepro *disptile = get_dispatcher_shared_tilepro(handle);

    return (disp->disabled) || ((disptile->crit_pc_low <= rip) && (rip < disptile->crit_pc_high));
}

static inline arch_registers_state_t*
dispatcher_get_enabled_save_area(dispatcher_handle_t handle)
{
    return &((struct dispatcher_shared_tilepro*)handle)->enabled_save_area;
}

static inline arch_registers_state_t*
dispatcher_get_disabled_save_area(dispatcher_handle_t handle)
{
    return &((struct dispatcher_shared_tilepro *)handle)->disabled_save_area;
}

static inline arch_registers_state_t*
dispatcher_get_trap_save_area(dispatcher_handle_t handle)
{
    return &((struct dispatcher_shared_tilepro *)handle)->trap_save_area;
}

#endif /* ARCH_TILEPRO_BARRELFISH_KPI_DISPATCHER_SHARED_ARCH_H_ */
