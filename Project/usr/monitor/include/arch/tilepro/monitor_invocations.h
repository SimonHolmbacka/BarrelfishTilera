/*
 * monitor_invocations.h
 *
 *  Created on: Aug 29, 2012
 *      Author: robert
 */

#ifndef INVOCATIONS_H
#define INVOCATIONS_H

#include <stdint.h>
#include <stddef.h>
#include <barrelfish_kpi/types.h>
#include <barrelfish_kpi/syscalls.h>
#include <barrelfish/syscall_arch.h>

#include <assert.h>

/**
 * \brief Spawn a new core.
 *
 * \param core_id    APIC ID of the core to try booting
 * \param cpu_type   Type of core to boot
 * \param entry      Kernel entry point in physical memory
 */
static inline errval_t invoke_monitor_spawn_core(coreid_t core_id, enum cpu_type cpu_type, forvaddr_t entry)
{
    uint8_t invoke_bits = get_cap_valid_bits(cap_kernel);
    capaddr_t invoke_cptr = get_cap_addr(cap_kernel) >> (CPTR_BITS - invoke_bits);

	return syscall8(SYSCALL_INVOKE, KernelCmd_Spawn_core, invoke_bits, invoke_cptr, core_id, cpu_type, (uint32_t)(entry >> 32), (uint32_t)entry).error;
}

static inline errval_t invoke_monitor_identify_cap(capaddr_t cap, int bits, struct capability *out)
{
	uint8_t invoke_bits = get_cap_valid_bits(cap_kernel);
	capaddr_t invoke_cptr = get_cap_addr(cap_kernel) >> (CPTR_BITS - invoke_bits);

	return syscall7(SYSCALL_INVOKE, KernelCmd_Identify_cap, invoke_bits, invoke_cptr, cap, bits, (uintptr_t)out).error;
}

static inline errval_t
invoke_monitor_identify_domains_cap(capaddr_t root_cap, int root_bits,
                                    capaddr_t cap, int bits,
                                    struct capability *out)
{
	assert(!"IMPLEMENT ME!");
}


static inline errval_t
invoke_monitor_nullify_cap(capaddr_t cap, int bits)
{
	assert(!"IMPLEMENT ME!");
}

static inline errval_t invoke_monitor_cap_remote(capaddr_t cap, int bits, bool is_remote, bool * has_descendents)
{
	uint8_t invoke_bits = get_cap_valid_bits(cap_kernel);
    capaddr_t invoke_cptr = get_cap_addr(cap_kernel) >> (CPTR_BITS - invoke_bits);

    struct sysret r;
    r = syscall7(SYSCALL_INVOKE, KernelCmd_Remote_cap, invoke_bits, invoke_cptr, cap, bits, is_remote);
    if(err_is_ok(r.error)) {
    	*has_descendents = r.value;
    }
    return r.error;
}

static inline errval_t invoke_monitor_create_cap(uint64_t *raw, capaddr_t caddr, int bits, capaddr_t slot)
{
    uint8_t invoke_bits = get_cap_valid_bits(cap_kernel);
    capaddr_t invoke_cptr = get_cap_addr(cap_kernel) >> (CPTR_BITS - invoke_bits);

    return syscall8(SYSCALL_INVOKE, KernelCmd_Create_cap, invoke_bits, invoke_cptr, caddr, bits, slot, (uintptr_t)raw).error;
}

static inline errval_t
invoke_monitor_register(struct capref ep)
{
    uint8_t invoke_bits = get_cap_valid_bits(cap_kernel);
    capaddr_t invoke_cptr = get_cap_addr(cap_kernel) >> (CPTR_BITS - invoke_bits);

    return syscall5(SYSCALL_INVOKE, KernelCmd_Register, invoke_bits, invoke_cptr, get_cap_addr(ep)).error;
}

static inline errval_t
invoke_monitor_identify_cnode_get_cap(uint64_t *cnode_raw, capaddr_t slot,
                                      struct capability *out)
{
	assert(!"IMPLEMENT ME!");
}


static inline errval_t
invoke_monitor_remote_cap_retype(capaddr_t rootcap_addr, uint8_t rootcap_vbits,
                                 capaddr_t src, enum objtype newtype,
                                 int objbits, capaddr_t to, capaddr_t slot,
                                 int bits) {
	assert(!"IMPLEMENT ME!");
}

static inline errval_t
invoke_monitor_remote_cap_delete(capaddr_t rootcap_addr, uint8_t rootcap_vbits,
                                 capaddr_t src, int bits) {
	assert(!"IMPLEMENT ME!");
}

static inline errval_t
invoke_monitor_remote_cap_revoke(capaddr_t rootcap_addr, uint8_t rootcap_vbits,
                                 capaddr_t src, int bits) {
	assert(!"IMPLEMENT ME!");
}

/**
 * \brief Set up tracing in the kernel
 *
 */
static inline errval_t
invoke_trace_setup(struct capref cap)
{
	assert(!"IMPLEMENT ME!");
}

static inline errval_t
invoke_domain_id(struct capref cap, domainid_t domain_id)
{
	assert(!"IMPLEMENT ME!");
}

static inline errval_t invoke_monitor_sync_timer(uint64_t synctime)
{
	uint8_t invoke_bits = get_cap_valid_bits(cap_kernel);
	capaddr_t invoke_cptr = get_cap_addr(cap_kernel) >> (CPTR_BITS - invoke_bits);

	return syscall6(SYSCALL_INVOKE, KernelCmd_Sync_timer, invoke_bits, invoke_cptr, synctime >> 32, synctime & 0xffffffff).error;
}

static inline errval_t
invoke_monitor_ipi_register(struct capref ep, int chanid)
{
	assert(!"IMPLEMENT ME!");
}

static inline errval_t
invoke_monitor_ipi_delete(int chanid)
{
	assert(!"IMPLEMENT ME!");
}

static inline errval_t
invoke_monitor_get_arch_id(uintptr_t *arch_id)
{
	assert(arch_id != NULL);

	uint8_t invoke_bits = get_cap_valid_bits(cap_kernel);
	capaddr_t invoke_cptr = get_cap_addr(cap_kernel) >> (CPTR_BITS - invoke_bits);

	struct sysret sysret;
	sysret = syscall4(SYSCALL_INVOKE, KernelCmd_Get_arch_id, invoke_bits, invoke_cptr);
	//printf("Returned: %ld\n", sysret.value);
	if (err_is_ok(sysret.error)) {
		*arch_id = sysret.value;
	}
	return sysret.error;
}

#endif /* INVOCATIONS_H */
