/*
 * sys_debug.c
 *
 *  Created on: Sep 11, 2012
 *      Author: robert
 */

#include <barrelfish/barrelfish.h>
#include <barrelfish/sys_debug.h>
#include <barrelfish_kpi/sys_debug.h>

#include <assert.h>

errval_t sys_nop(void)
{
	assert(!"implement me");
}

errval_t sys_reboot(void)
{
	assert(!"implement me");
}

errval_t sys_debug_context_counter_reset(void)
{
	assert(!"implement me");
}

errval_t sys_debug_context_counter_read(uint64_t *ret)
{
	assert(!"implement me");
}

errval_t sys_debug_print_context_counter(void)
{
	assert(!"implement me");
}

errval_t sys_debug_timeslice_counter_read(uint64_t *ret)
{
	assert(!"implement me");
}

errval_t sys_debug_get_tsc_per_ms(uint64_t *ret)
{
	struct sysret sr = syscall2(SYSCALL_DEBUG, DEBUG_GET_TSC_PER_MS);
	*ret = sr.value;
	return sr.error;
}

errval_t sys_debug_get_apic_timer(uint32_t *ret)
{
	assert(!"implement me");
}

errval_t sys_debug_get_apic_ticks_per_sec(uint32_t *ret)
{
	assert(!"implement me");
}

errval_t sys_debug_print_timeslice(void)
{
	assert(!"implement me");
}

errval_t sys_debug_flush_cache(void)
{
	assert(!"implement me");
}

errval_t sys_debug_send_ipi(uint8_t destination, uint8_t shorthand, uint8_t vector)
{
	assert(!"implement me");
}

errval_t sys_debug_set_breakpoint(uintptr_t addr, uint8_t mode, uint8_t length)
{
	assert(!"implement me");
}

errval_t sys_debug_hardware_timer_read(uintptr_t* ret)
{
	assert(!"implement me");
}

errval_t sys_debug_hardware_timer_hertz_read(uintptr_t* ret)
{
	assert(!"implement me");
}
