/*
 * invocations_arch.h
 *
 *  Created on: Aug 17, 2012
 *      Author: robert
 */

#ifndef INVOCATIONS_ARCH_H_
#define INVOCATIONS_ARCH_H_

#include <barrelfish/syscall_arch.h> // for sys_invoke and cap_invoke
#include <barrelfish_kpi/dispatcher_shared.h>
#include <barrelfish/caddr.h>

#include <assert.h>

#define IMPLEMENT_ME 0

/**
 * \brief Retype a capability.
 *
 * Retypes CPtr 'cap' into 2^'objbits' caps of type 'newtype' and places them
 * into slots starting at slot 'slot' in the CNode, addressed by 'to', with
 * 'bits' address bits of 'to' valid.
 *
 * See also cap_retype(), which wraps this.
 *
 * \param root          Capability of the CNode to invoke
 * \param cap           Address of cap to retype.
 * \param newtype       Kernel object type to retype to.
 * \param objbits       Size of created objects, for variable-sized types
 * \param to            Address of CNode cap to place retyped caps into.
 * \param slot          Slot in CNode cap to start placement.
 * \param bits          Number of valid address bits in 'to'.
 *
 * \return Error code
 */
static inline errval_t invoke_cnode_retype(struct capref root,
										   capaddr_t cap,
                                           enum objtype newtype,
                                           int objbits,
                                           capaddr_t to,
                                           capaddr_t slot,
                                           int bits)
{
	// check the capability is not null
	assert(cap != CPTR_NULL);

	uint8_t invoke_bits = get_cap_valid_bits(root);
	capaddr_t invoke_cptr = get_cap_addr(root) >> (CPTR_BITS - invoke_bits);

	assert(newtype <= 0xffff);
	assert(objbits <= 0xff);
	assert(bits <= 0xff);

	return syscall10(SYSCALL_INVOKE,
					 CNodeCmd_Retype,
					 invoke_bits,
					 invoke_cptr,
					 cap,
					 bits,
					 objbits,
					 newtype,
					 to,
					 slot).error;
}

/**
 * \brief Create a capability.
 *
 * Create a new capability of type 'type' and size 'objbits'. The new cap will
 * be placed in the slot 'dest_slot' of the CNode located at 'dest_cnode_cptr'
 * in the address space rooted at 'root'.
 *
 * See also cap_create(), which wraps this.
 *
 * \param root            Capability of the CNode to invoke.
 * \param type            Kernel object type to create.
 * \param objbits         Size of created object
 *                        (ignored for fixed-size objects)
 * \param dest_cnode_cptr Address of CNode cap, where newly created cap will be
 *                        placed into.
 * \param dest_slot       Slot in CNode cap to place new cap.
 * \param dest_vbits      Number of valid address bits in 'dest_cnode_cptr'.
 *
 * \return Error code
 */
static inline errval_t invoke_cnode_create(struct capref root,
                                           enum objtype type, uint8_t objbits,
                                           capaddr_t dest_cnode_cptr,
                                           capaddr_t dest_slot,
                                           uint8_t dest_vbits)
{
	syscall3(SYSCALL_PRINT, (uintptr_t)"implement me!\n", 14);
	assert(IMPLEMENT_ME);
}

/**
 * \brief "Mint" a capability.
 *
 * Copies CPtr 'from' into slot 'slot' in the CNode, addressed by 'to', within
 * the address space, rooted at 'root' and with 'tobits' and 'frombits' address
 * bits of 'to' and 'from' valid, respectively.
 *
 * See also cap_mint(), which wraps this.
 *
 * \param root          Capability of the CNode to invoke
 * \param to            CNode to place copy into.
 * \param slot          Slot in CNode cap to place copy into.
 * \param from          Address of cap to copy.
 * \param tobits        Number of valid address bits in 'to'.
 * \param frombits      Number of valid address bits in 'from'.
 * \param param1        1st cap-dependent parameter.
 * \param param2        2nd cap-dependent parameter.
 *
 * \return Error code
 */
static inline errval_t invoke_cnode_mint(struct capref root,
										 capaddr_t to,
										 capaddr_t slot,
										 capaddr_t from,
										 int tobits,
										 int frombits,
										 uintptr_t param1,
										 uintptr_t param2)
{
	uint8_t invoke_bits = get_cap_valid_bits(root);
	capaddr_t invoke_cptr = get_cap_addr(root) >> (CPTR_BITS - invoke_bits);

	assert(slot <= 0xffff);
	assert(tobits <= 0xff);
	assert(frombits <= 0xff);

	return syscall11(SYSCALL_INVOKE,
					 CNodeCmd_Mint,
					 invoke_bits,
					 invoke_cptr,
					 to,
					 from,
					 frombits,
					 tobits,
					 slot,
					 param1,
					 param2).error;
}

/**
 * \brief Copy a capability.
 *
 * Copies CPtr 'from' into slot 'slot' in the CNode, addressed by 'to', within
 * the address space, rooted at 'root' and with 'tobits' and 'frombits' address
 * bits of 'to' and 'from' valid, respectively.
 *
 * See also cap_copy(), which wraps this.
 *
 * \param root          Capability of the CNode to invoke
 * \param to            CNode to place copy into.
 * \param slot          Slot in CNode cap to place copy into.
 * \param from          Address of cap to copy.
 * \param tobits        Number of valid address bits in 'to'.
 * \param frombits      Number of valid address bits in 'from'.
 *
 * \return Error code
 */
static inline errval_t invoke_cnode_copy(struct capref root,
										 capaddr_t to,
                                         capaddr_t slot,
                                         capaddr_t from,
                                         int tobits,
                                         int frombits)
{
	uint8_t invoke_bits = get_cap_valid_bits(root);
	capaddr_t invoke_cptr = get_cap_addr(root) >> (CPTR_BITS - invoke_bits);

	assert(slot <= 0xffff);
	assert(tobits <= 0xff);
	assert(frombits <= 0xff);

	return syscall9(SYSCALL_INVOKE,
					CNodeCmd_Copy,
					invoke_bits,
					invoke_cptr,
					to,
					from,
					frombits,
					tobits,
					slot).error;
}

/**
 * \brief Delete a capability.
 *
 * Delete the capability pointed to by 'cap', with 'bits' address bits
 * of it valid, from the address space rooted at 'root'.
 *
 * \param root  Capability of the CNode to invoke
 * \param cap   Address of cap to delete.
 * \param bits  Number of valid bits within 'cap'.
 *
 * \return Error code
 */
static inline errval_t invoke_cnode_delete(struct capref root, capaddr_t cap, int bits)
{
	uint8_t invoke_bits = get_cap_valid_bits(root);
	capaddr_t invoke_cptr = get_cap_addr(root) >> (CPTR_BITS - invoke_bits);

	assert(bits <= 0xff);

	return syscall6(SYSCALL_INVOKE, CNodeCmd_Delete, invoke_bits, invoke_cptr, cap, bits).error;
}

static inline errval_t invoke_cnode_revoke(struct capref root, capaddr_t cap, int bits)
{
	uint8_t invoke_bits = get_cap_valid_bits(root);
	capaddr_t invoke_cptr = get_cap_addr(root) >> (CPTR_BITS - invoke_bits);

	assert(bits <= 0xff);

	return syscall6(SYSCALL_INVOKE, CNodeCmd_Revoke, invoke_bits, invoke_cptr, cap, bits).error;
}

static inline errval_t invoke_vnode_map(struct capref ptable, capaddr_t slot, capaddr_t from, int frombits, uint64_t flags, uintptr_t offset, uintptr_t pte_count)
{
    uint8_t invoke_bits = get_cap_valid_bits(ptable);
    capaddr_t invoke_cptr = get_cap_addr(ptable) >> (CPTR_BITS - invoke_bits);

    // on tilera architecture, page flag is 64-bit, so we divide it into two 32-bit values to pass to the kernel.
    uint32_t flags_up_32 = flags >> 32;
    uint32_t flags_lo_32 = flags & 0xFFFFFFFF;

    assert(slot <= 0xffff);
    assert(frombits <= 0xff);

    // XXX: needs check of flags, offset, and pte_count sizes
    return syscall11(SYSCALL_INVOKE, VNodeCmd_Map, invoke_bits, invoke_cptr, from, slot, frombits, flags_up_32, flags_lo_32, offset, pte_count).error;
}

static inline errval_t invoke_vnode_unmap(struct capref cap, capaddr_t mapping_cptr, int mapping_bits, size_t entry, size_t pte_count)
{
	uint8_t invoke_bits = get_cap_valid_bits(cap);
	capaddr_t invoke_cptr = get_cap_addr(cap) >> (CPTR_BITS - invoke_bits);

	assert(entry < 256);
	assert(pte_count < 256);
	assert(mapping_bits <= 0xff);

	return syscall8(SYSCALL_INVOKE, VNodeCmd_Unmap, invoke_bits, invoke_cptr, mapping_cptr, mapping_bits, entry, pte_count).error;
}

/**
 * \brief Return the physical address and size of a frame capability
 *
 * \param frame    CSpace address of frame capability
 * \param ret      frame_identity struct filled in with relevant data
 *
 * \return Error code
 */
static inline errval_t invoke_frame_identify(struct capref frame, struct frame_identity *ret)
{
	uint8_t invoke_bits = get_cap_valid_bits(frame);
	capaddr_t invoke_cptr = get_cap_addr(frame) >> (CPTR_BITS - invoke_bits);

	struct sysret sysret = syscall4(SYSCALL_INVOKE, FrameCmd_Identify, invoke_bits, invoke_cptr);

	assert(ret != NULL);

	if(err_is_ok(sysret.error)) {
		ret->base = sysret.value & (~BASE_PAGE_MASK);
		ret->bits = sysret.value & BASE_PAGE_MASK;
		return sysret.error;
	}

	ret->base = 0;
	ret->bits = 0;
	return sysret.error;
}

static inline errval_t invoke_iocap_in(struct capref iocap, enum io_cmd cmd,
                                       uint16_t port, uint32_t *data)
{
	syscall3(SYSCALL_PRINT, (uintptr_t)"implement me!\n", 14);
	assert(IMPLEMENT_ME);
}

static inline errval_t invoke_iocap_out(struct capref iocap, enum io_cmd cmd,
                                        uint16_t port, uint32_t data)
{
	syscall3(SYSCALL_PRINT, (uintptr_t)"implement me!\n", 14);
	assert(IMPLEMENT_ME);
}


/**
 * \brief Setup a dispatcher, possibly making it runnable
 *
 * \param dispatcher    	Address of dispatcher capability
 * \param domdispatcher 	Address of existing dispatcher for domain ID
 * \param cspace_root   	Root of CSpace for new dispatcher
 * \param cspace_root_bits  Number of valid bits in cspace_root
 * \param vspace_root   	Root of VSpace for new dispatcher
 * \param dispatcher_frame 	Frame capability for dispatcher structure
 * \param run           	Make runnable if true
 *
 * Any arguments of CPTR_NULL are ignored.
 *
 * \return Error code
 */
static inline errval_t invoke_dispatcher(struct capref dispatcher,
										 struct capref domdispatcher,
										 struct capref cspace,
										 struct capref vspace,
										 struct capref dispframe,
										 bool run)
{
    uint8_t root_vbits = get_cap_valid_bits(cspace);
    capaddr_t root_caddr = get_cap_addr(cspace) >> (CPTR_BITS - root_vbits);
    capaddr_t vtree_caddr = get_cap_addr(vspace);
    capaddr_t disp_caddr = get_cap_addr(dispframe);
    capaddr_t dd_caddr = get_cap_addr(domdispatcher);
    uint8_t invoke_bits = get_cap_valid_bits(dispatcher);
    capaddr_t invoke_cptr = get_cap_addr(dispatcher) >> (CPTR_BITS - invoke_bits);

    assert(root_vbits <= 0xff);

    return syscall10(SYSCALL_INVOKE,
    				 DispatcherCmd_Setup,
    				 invoke_bits,
    				 invoke_cptr,
    				 dd_caddr,
    				 root_caddr,
    				 root_vbits & 0xff,
    				 run,
    				 vtree_caddr,
    				 disp_caddr).error;
}

/**
 * \brief Setup a VM guest DCB
 *
 * \param dcb       Dispatcher capability
 */
static inline errval_t invoke_dispatcher_setup_guest(struct capref dispatcher,
                                                     capaddr_t ep_cap,
                                                     capaddr_t vnode,
                                                     capaddr_t vmkit_guest,
                                                     capaddr_t guest_control_cap)
{
	syscall3(SYSCALL_PRINT, (uintptr_t)"implement me!\n", 14);
	assert(IMPLEMENT_ME);
}

static inline errval_t invoke_irqtable_set(struct capref irqcap, int irq,
                                           struct capref ep)
{
	syscall3(SYSCALL_PRINT, (uintptr_t)"implement me!\n", 14);
	assert(IMPLEMENT_ME);
}

static inline errval_t invoke_irqtable_delete(struct capref irqcap, int irq)
{
	syscall3(SYSCALL_PRINT, (uintptr_t)"implement me!\n", 14);
	assert(IMPLEMENT_ME);
}

/*
 * get the core ID
 * @param kern_cap (in), kernel capability reference.
 * @param core_id (out), return the core ID.
 */
static inline errval_t invoke_kernel_get_core_id(struct capref kern_cap, coreid_t *core_id)
{
	assert(core_id != NULL);

	uint8_t invoke_bits = get_cap_valid_bits(kern_cap);
	capaddr_t invoke_cptr = get_cap_addr(kern_cap) >> (CPTR_BITS - invoke_bits);

	struct sysret sysret = syscall4(SYSCALL_INVOKE, KernelCmd_Get_core_id, invoke_bits, invoke_cptr);

	if(sysret.error == SYS_ERR_OK) {
		*core_id = sysret.value;
	}

	return sysret.error;
}

static inline errval_t invoke_kernel_dump_ptables(struct capref kern_cap,
                                                  struct capref dispcap)
{
    uint8_t invoke_bits = get_cap_valid_bits(kern_cap);
    capaddr_t invoke_cptr = get_cap_addr(kern_cap) >> (CPTR_BITS - invoke_bits);

    capaddr_t dispcaddr = get_cap_addr(dispcap);
    struct sysret sysret =
    	syscall5(SYSCALL_INVOKE, KernelCmd_DumpPTables, invoke_bits, invoke_cptr, dispcaddr);
    return sysret.error;
}

static inline errval_t
invoke_dispatcher_properties(
    struct capref dispatcher,
    enum task_type type, unsigned long deadline,
    unsigned long wcet, unsigned long period,
    unsigned long release, unsigned short weight
                            )
{
	syscall3(SYSCALL_PRINT, (uintptr_t)"implement me!\n", 14);
	assert(IMPLEMENT_ME);
}

#endif /* INVOCATIONS_ARCH_H_ */
