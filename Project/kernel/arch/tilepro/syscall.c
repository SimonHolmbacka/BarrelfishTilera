/*
 * syscall.c
 *
 *  Created on: Dec 4, 2012
 *      Author: robert
 */

#include <kernel.h>
#include <barrelfish_kpi/syscalls.h>
#include <barrelfish_kpi/lmp.h>
#include <barrelfish_kpi/sys_debug.h>
#include <barrelfish_kpi/paging_target.h>
#include <paging_kernel_arch.h>
#include <dispatch.h>
#include <exec.h>
#include <stdio.h>
#include <syscall.h>

#include <arch/spr_def.h>
#include <arch/sim_def.h>
#include <arch/udn.h>


static struct sysret handle_dispatcher_setup(struct capability* to, arch_registers_state_t* context_save)
{
	capaddr_t odptr = context_save->r3;
	capaddr_t cptr = context_save->r4;
	int depth = context_save->r5 & 0xff;
	int run = context_save->r6;
	capaddr_t vptr = context_save->r7;
	capaddr_t dptr = context_save->r8;

	return sys_dispatcher_setup(to, cptr, depth, vptr, dptr, run, odptr);
}

static struct sysret handle_dispatcher_properties(struct capability* to, arch_registers_state_t* context_save)
{
	struct sysret ret = { .error = SYS_ERR_OK, .value = 0 };
	assert(!"implement me!\n");
	return ret;
}

static struct sysret handle_dispatcher_perf_mon(struct capability* to, arch_registers_state_t* context_save)
{
	struct sysret ret = { .error = SYS_ERR_OK, .value = 0 };
	assert(!"implement me!\n");
	return ret;
}

static struct sysret handle_dispatcher_setup_guest(struct capability* to, arch_registers_state_t* context_save)
{
	struct sysret ret = { .error = SYS_ERR_OK, .value = 0 };
	assert(!"implement me!\n");
	return ret;
}

/*
 * Return with physical base address of frame
 */
static struct sysret handle_frame_identify(struct capability* to, arch_registers_state_t* context_save)
{
	assert(to->type == ObjType_Frame || to->type == ObjType_DevFrame);
	assert((to->u.frame.base & BASE_PAGE_MASK) == 0);
	assert(to->u.frame.bits < BASE_PAGE_SIZE);

	return (struct sysret){
		.error = SYS_ERR_OK,
		.value = to->u.frame.base | to->u.frame.bits,
	};
}

static struct sysret handle_cnode_copy(struct capability* root, arch_registers_state_t* context_save)
{
	capaddr_t destcn_cptr = context_save->r3;
	capaddr_t source_cptr = context_save->r4;
	capaddr_t dest_slot = context_save->r7;
	int destcn_vbits = context_save->r6 & 0xff;
	int source_vbits = context_save->r5 & 0xff;

	return sys_copy_or_mint(root, destcn_cptr, dest_slot, source_cptr, destcn_vbits, source_vbits, 0, 0, false);
}

/*
 * r0 = CNodeCmd_Mint,
 * r1 = invoke_bits,
 * r2 = invoke_cptr,
 * r3 = to,
 * r4 = from,
 * r5 = frombits,
 * r6 = tobits,
 * r7 = slot,
 * r8 = param1,
 * r9 = param2,
 */
static struct sysret handle_cnode_mint(struct capability* root, arch_registers_state_t* context_save)
{
	capaddr_t destcn_cptr = context_save->r3;
	capaddr_t source_cptr = context_save->r4;
	capaddr_t dest_slot = context_save->r7;
	int destcn_vbits = context_save->r6 & 0xff;
	int source_vbits = context_save->r5 & 0xff;
	uintptr_t param1 = context_save->r8;
	uintptr_t param2 = context_save->r9;

	return sys_copy_or_mint(root, destcn_cptr, dest_slot, source_cptr, destcn_vbits, source_vbits, param1, param2, true);
}

static struct sysret handle_cnode_retype(struct capability* root, arch_registers_state_t* context_save)
{
	// Source capability cptr
	capaddr_t source_cptr = context_save->r3;
	// get new type
	enum objtype new_type = context_save->r6 & 0xff;
	// Object bits for variable-sized types
	uint8_t objbits = (uint8_t)(context_save->r5 & 0xff);
	// Destination cnode cptr
	capaddr_t dest_cnode_cptr = context_save->r7;
	// Destination slot number
	capaddr_t dest_slot = context_save->r8;
	// Valid bits in destination cnode cptr
	uint8_t dest_vbits = (uint8_t)(context_save->r4 & 0xff);

	return sys_retype(root, source_cptr, new_type, objbits, dest_cnode_cptr, dest_slot, dest_vbits, false);
}

static struct sysret handle_cnode_create(struct capability* to, arch_registers_state_t* context_save)
{
	struct sysret ret = { .error = SYS_ERR_OK, .value = 0 };
	assert(!"implement me!\n");
	return ret;
}

static struct sysret handle_cnode_delete(struct capability* root, arch_registers_state_t* context_save)
{
	// Source capability cptr
	capaddr_t cptr = context_save->r3;
	// Valid bits in destination cnode cptr
	uint8_t bits = (uint8_t)(context_save->r4 & 0xff);

	return sys_delete(root, cptr, bits, false);
}

static struct sysret handle_cnode_revoke(struct capability* root, arch_registers_state_t* context_save)
{
	capaddr_t cptr = (capaddr_t)context_save->r3;
	int bits = (int)context_save->r4;

	return sys_revoke(root, cptr, bits, false);
}

static struct sysret handle_vnode_map(struct capability *ptable, arch_registers_state_t* context_save)
{
	capaddr_t dest_slot = (capaddr_t)context_save->r4;
	capaddr_t source_cptr = (capaddr_t)context_save->r3;
	int source_vbits = context_save->r5;
	uint64_t flags = ((uint64_t)context_save->r6 << 32) | (context_save->r7);
	uintptr_t offset = (uintptr_t)context_save->r8;
	uintptr_t pte_count = (uintptr_t)context_save->r9;
	return sys_map(ptable, dest_slot, source_cptr, source_vbits, flags, offset, pte_count);
}

static struct sysret handle_vnode_unmap(struct capability* pgtable, arch_registers_state_t* context_save)
{
	capaddr_t  mapping_cptr = (capaddr_t)context_save->r3;
	int mapping_bits = context_save->r4;
	size_t entry = (size_t)context_save->r5;
	size_t pte_count = (size_t)context_save->r6;

	errval_t err;
    struct cte *mapping;
    err = caps_lookup_slot(&dcb_current->cspace.cap, mapping_cptr, mapping_bits, &mapping, CAPRIGHTS_READ_WRITE);
    if(err_is_fail(err)) {
		return SYSRET(err_push(err, SYS_ERR_CAP_NOT_FOUND));
	}

	err = page_mappings_unmap(pgtable, mapping, entry, pte_count);
	return SYSRET(err);
}

/**
 * \brief Spawn a new core
 */
static struct sysret sys_monitor_spawn_core(coreid_t core_id, enum cpu_type cpu_type,
                                     uint32_t entry_most, uint32_t entry_least) {
	if (cpu_type != CPU_TILEPRO) {
		return SYSRET(SYS_ERR_CORE_NOT_FOUND);
	}

	HV_Topology topology = hv_inquire_topology();
	uint32_t y = core_id / topology.width;
	uint32_t x = core_id % topology.width;
	int len = 4; // tag + boot_msg + 2*entry
	uint32_t header = fill_dynamic_header(x, y, 0, len);
	udn_send(header);
	udn_send(STARTUP_CORE_TAG);
	uint32_t msg = 'b' << 24 | 'o' << 16 | 'o' << 8 | 't';
	udn_send(msg);
	udn_send(entry_most);
	udn_send(entry_least);
	uint32_t answer = udn0_receive();
	assert(((answer >> 24) & 0xFF) == 'o');
	assert(((answer >> 16) & 0xFF) == 'k');
	assert(((answer >> 0) & 0xFF) == core_id);

	return SYSRET(SYS_ERR_OK);
}

static struct sysret handle_kernel_spawn_core(struct capability* to, arch_registers_state_t* context_save)
{
	coreid_t core_id = context_save->r3;
	enum cpu_type cpu_type = context_save->r4;
	uint32_t entry_most = context_save->r5;
	uint32_t entry_least = context_save->r6;
	return sys_monitor_spawn_core(core_id, cpu_type, entry_most, entry_least);
}

static struct sysret handle_kernel_get_core_id(struct capability* to, arch_registers_state_t* context_save)
{
	return (struct sysret){ .error = SYS_ERR_OK, .value = my_core_id };
}

static struct sysret handle_kernel_get_arch_id(struct capability* to, arch_registers_state_t* context_save)
{
	// No arch id (whatever that is) just use the core id.
	//printf("Calling arch_id: %d\n", my_core_id);
	return (struct sysret){ .error = SYS_ERR_OK, .value = my_core_id };
}

/*
 * for monitor to identify caps
 */
static struct sysret handle_kernel_identify_cap(struct capability* kernel_cap, arch_registers_state_t* context_save)
{
	capaddr_t cptr = context_save->r3;
	int bits = context_save->r4;
	struct capability *retbuf = (void*)context_save->r5;

	return sys_monitor_identify_cap(&dcb_current->cspace.cap, cptr, bits, retbuf);
}

static struct sysret handle_kernel_identify_domains_cap(struct capability* to, arch_registers_state_t* context_save)
{
	struct sysret ret = { .error = SYS_ERR_OK, .value = 0 };
	assert(!"implement me!\n");
	return ret;
}

static struct sysret handle_kernel_remote_cap(struct capability* kernel_cap, arch_registers_state_t* context_save)
{
	struct capability *root = &dcb_current->cspace.cap;
	capaddr_t cptr = context_save->r3;
	int bits = context_save->r4;
	bool remote = (bool)context_save->r5;

	struct cte *cte;
	errval_t err = caps_lookup_slot(root, cptr, bits, &cte, CAPRIGHTS_WRITE);
	if(err_is_fail(err)) {
		return SYSRET(err_push(err, SYS_ERR_IDENTIFY_LOOKUP));
	}

	set_cap_remote(cte, remote);
	bool has_desc = has_descendants(cte);

	return (struct sysret){ .error = SYS_ERR_OK, .value = has_desc };
}

static struct sysret handle_kernel_iden_cnode_get_cap(struct capability* to, arch_registers_state_t* context_save)
{
	struct sysret ret = { .error = SYS_ERR_OK, .value = 0 };
	assert(!"implement me!\n");
	return ret;
}

/*
 * create monitor cap
 */
static struct sysret handle_kernel_create_cap(struct capability* to, arch_registers_state_t* context_save)
{
	/* Create the cap in the destination */
    capaddr_t cnode_cptr = context_save->r3;
    int cnode_vbits = context_save->r4;
    size_t slot = context_save->r5;
    struct capability *src = (struct capability*)context_save->r6;

    /* Certain types cannot be created here */
	if((src->type == ObjType_Null) || (src->type == ObjType_EndPoint) ||
	    (src->type == ObjType_Dispatcher) || (src->type == ObjType_Kernel) ||
	    (src->type == ObjType_IRQTable)) {
		return SYSRET(SYS_ERR_ILLEGAL_DEST_TYPE);
	}

	return SYSRET(caps_create_from_existing(&dcb_current->cspace.cap, cnode_cptr, cnode_vbits, slot, src));
}

static struct sysret handle_kernel_nullify_cap(struct capability* to, arch_registers_state_t* context_save)
{
	struct sysret ret = { .error = SYS_ERR_OK, .value = 0 };
	assert(!"implement me!\n");
	return ret;
}

static struct sysret handle_kernel_setup_trace(struct capability* to, arch_registers_state_t* context_save)
{
	struct sysret ret = { .error = SYS_ERR_OK, .value = 0 };
	assert(!"implement me!\n");
	return ret;
}

static struct sysret handle_kernel_register(struct capability* to, arch_registers_state_t* context_save)
{
	assert(context_save->r3);
    capaddr_t ep_caddr = context_save->r3;

    return sys_monitor_register(ep_caddr);
}

static struct sysret handle_kernel_domain_id(struct capability* to, arch_registers_state_t* context_save)
{
	struct sysret ret = { .error = SYS_ERR_OK, .value = 0 };
	assert(!"implement me!\n");
	return ret;
}

static struct sysret handle_monitor_retype(struct capability* to, arch_registers_state_t* context_save)
{
	struct sysret ret = { .error = SYS_ERR_OK, .value = 0 };
	assert(!"implement me!\n");
	return ret;
}

static struct sysret handle_monitor_delete(struct capability* to, arch_registers_state_t* context_save)
{
	struct sysret ret = { .error = SYS_ERR_OK, .value = 0 };
	assert(!"implement me!\n");
	return ret;
}

static struct sysret handle_monitor_revoke(struct capability* to, arch_registers_state_t* context_save)
{
	struct sysret ret = { .error = SYS_ERR_OK, .value = 0 };
	assert(!"implement me!\n");
	return ret;
}

static struct sysret handle_kernel_sync_timer(struct capability* to, arch_registers_state_t* context_save)
{
	// TODO: we need to do something here about timer. now do nothing.
	struct sysret ret = { .error = SYS_ERR_OK, .value = 0 };
	return ret;
}

static struct sysret kernel_dump_ptables(struct capability *cap, arch_registers_state_t* context_save)
{
    assert(cap->type == ObjType_Kernel);

    printf("kernel_dump_ptables\n");

    capaddr_t dispcaddr = context_save->r3;

    struct cte *dispcte;
    errval_t err = caps_lookup_slot(&dcb_current->cspace.cap, dispcaddr, CPTR_BITS,
                           &dispcte, CAPRIGHTS_WRITE);
    if (err_is_fail(err)) {
        printf("failed to lookup dispatcher cap\n");
        return SYSRET(err_push(err, SYS_ERR_DISP_FRAME));
    }
    struct capability *dispcap = &dispcte->cap;
    if (dispcap->type != ObjType_Dispatcher) {
        printf("dispcap is not dispatcher cap\n");
        return SYSRET(err_push(err, SYS_ERR_DISP_FRAME_INVALID));
    }

    struct dcb *dispatcher = dispcap->u.dispatcher.dcb;

    paging_dump_tables(dispatcher);

    return SYSRET(SYS_ERR_OK);
}

// invocation function handler
typedef struct sysret (*invocation_handler_t)(struct capability* to, arch_registers_state_t* context_save);

// invocation function table
static invocation_handler_t invocations[ObjType_Num][CAP_MAX_CMD] = {
		[ObjType_Dispatcher] = {
				[DispatcherCmd_Setup]      = handle_dispatcher_setup,
		        [DispatcherCmd_Properties] = handle_dispatcher_properties,
		        [DispatcherCmd_PerfMon]    = handle_dispatcher_perf_mon,
		        [DispatcherCmd_SetupGuest] = handle_dispatcher_setup_guest
		},
		[ObjType_Frame] = {
				[FrameCmd_Identify] = handle_frame_identify
		},
		[ObjType_DevFrame] = {
				[FrameCmd_Identify] = handle_frame_identify
		},
		[ObjType_CNode] = {
				[CNodeCmd_Copy]   = handle_cnode_copy,
				[CNodeCmd_Mint]   = handle_cnode_mint,
				[CNodeCmd_Retype] = handle_cnode_retype,
				[CNodeCmd_Create] = handle_cnode_create,
				[CNodeCmd_Delete] = handle_cnode_delete,
				[CNodeCmd_Revoke] = handle_cnode_revoke
	    },
	    [ObjType_VNode_tilepro_l1] = {
	    		[VNodeCmd_Map]   = handle_vnode_map,
	    		[VNodeCmd_Unmap] = handle_vnode_unmap
		},
		[ObjType_VNode_tilepro_l2] = {
				[VNodeCmd_Map]   = handle_vnode_map,
				[VNodeCmd_Unmap] = handle_vnode_unmap
		},
		[ObjType_Kernel] = {
				[KernelCmd_Spawn_core]   		 = handle_kernel_spawn_core,
				[KernelCmd_Get_core_id]  		 = handle_kernel_get_core_id,
				[KernelCmd_Get_arch_id]			 = handle_kernel_get_arch_id,
				[KernelCmd_Identify_cap]		 = handle_kernel_identify_cap,
				[KernelCmd_Identify_domains_cap] = handle_kernel_identify_domains_cap,
				[KernelCmd_Remote_cap] 			 = handle_kernel_remote_cap,
				[KernelCmd_Iden_cnode_get_cap] 	 = handle_kernel_iden_cnode_get_cap,
				[KernelCmd_Create_cap]   		 = handle_kernel_create_cap,
				[KernelCmd_Nullify_cap]  		 = handle_kernel_nullify_cap,
				[KernelCmd_Setup_trace]  		 = handle_kernel_setup_trace,
				[KernelCmd_Register]     		 = handle_kernel_register,
				[KernelCmd_Domain_Id]    		 = handle_kernel_domain_id,
				[MonitorCmd_Retype]      		 = handle_monitor_retype,
				[MonitorCmd_Delete]      		 = handle_monitor_delete,
				[MonitorCmd_Revoke]      		 = handle_monitor_revoke,
				[KernelCmd_Sync_timer]   		 = handle_kernel_sync_timer,
				[KernelCmd_DumpPTables]          = kernel_dump_ptables,
		}
};

/*
 * syscall, invoke a cap
 *
 * @param context_save This pointer is pointing to the memory where all the registers ware saved before syscall.
 * r0 - r9 are for passing arguments.
 *
 * r0 = CNodeCmd_Retype,
 * r1 = invoke_bits,
 * r2 = invoke_cptr
 */
static struct sysret sys_invoke(arch_registers_state_t *context_save)
{
	struct sysret ret = { .error = SYS_ERR_OK, .value = 0 };

	uint8_t invoke_bits = (uint8_t)(context_save->r1 & 0xff);
	capaddr_t invoke_cptr = context_save->r2;

	debug(SUBSYS_SYSCALL, "sys_invoke(0x%"PRIxCADDR"(%u))\n", invoke_cptr, invoke_bits);

	// Capability to invoke
	struct capability *to = NULL;
	ret.error = caps_lookup_cap(&dcb_current->cspace.cap, invoke_cptr, invoke_bits, &to, CAPRIGHTS_READ);
	if(err_is_fail(ret.error)) {
		return ret;
	}
	assert(to != NULL);
	assert(to->type < ObjType_Num);

	// if cap is of endpoint, then do local message passing (LMP)
	if(to->type == ObjType_EndPoint) {
		//printf("Sending local message...\n");

		// create a receiver
		struct dcb *listener = to->u.endpoint.listener;
		assert(listener != NULL);

		if(listener->disp == 0) {
			ret.error = SYS_ERR_LMP_NO_TARGET;
			return ret;
		}

		// see lmp_chan_arch.h
		uint8_t flags = (uint8_t)((context_save->r1 >> 16) & 0xf);
		uint8_t length_words = (uint8_t)((context_save->r1 >> 20) & 0xff);
		uint8_t send_bits = (uint8_t)((context_save->r1 >> 8) & 0xff);
		capaddr_t send_cptr = context_save->r0;

		// limit length of message from buggy/malicious sender
		length_words = min(length_words, LMP_MSG_LENGTH);

		// does the sender want to yield their timeslice on success?
		bool sync = flags & LMP_FLAG_SYNC;
		// does the sender want to yield to the target if undeliverable?
		bool yield = flags & LMP_FLAG_YIELD;

		// create a buffer to store the msg
		uintptr_t msg_words[LMP_MSG_LENGTH];
		msg_words[0] = context_save->r3;
		msg_words[1] = context_save->r4;
		msg_words[2] = context_save->r5;
		msg_words[3] = context_save->r6;
		msg_words[4] = context_save->r7;
		msg_words[5] = context_save->r8;
		msg_words[6] = context_save->r9;

		// try to deliver message
		ret.error = lmp_deliver(to, dcb_current, msg_words, length_words, send_cptr, send_bits);

		/* Switch to reciever upon successful delivery with sync flag,
		 * or (some cases of) unsuccessful delivery with yield flag */
		enum err_code err_code = err_no(ret.error);
		if((sync && err_is_ok(ret.error)) ||
		   (yield && (err_code == SYS_ERR_LMP_BUF_OVERFLOW ||
				      err_code == SYS_ERR_LMP_CAPTRANSFER_DST_CNODE_LOOKUP ||
				      err_code == SYS_ERR_LMP_CAPTRANSFER_DST_CNODE_INVALID ||
				      err_code == SYS_ERR_LMP_CAPTRANSFER_DST_SLOT_OCCUPIED))) {
			if(err_is_fail(ret.error)) {
				struct dispatcher_shared_generic *current_disp = get_dispatcher_shared_generic(dcb_current->disp);
				struct dispatcher_shared_generic *listener_disp = get_dispatcher_shared_generic(listener->disp);
				debug(SUBSYS_DISPATCH,
					  "LMP failed; %.*s yields to %.*s: %u\n",
					  DISP_NAME_LEN,
					  current_disp->name,
					  DISP_NAME_LEN,
					  listener_disp->name,
					  err_code);
			}

			// special-case context switch: ensure correct state in current DCB
			dispatcher_handle_t handle = dcb_current->disp;
			struct dispatcher_shared_tilepro *disp_tilepro = get_dispatcher_shared_tilepro(handle);
			// check if the current dcb is disabled?
			dcb_current->disabled = dispatcher_is_disabled_ip(handle, context_save->pc);
			// here we want to save the return data to r0, and ensure we are in the right save area
			// but most of registers we have already saved in the syscall handler
			if(dcb_current->disabled) {
				assert(context_save == &disp_tilepro->disabled_save_area);
				context_save->r0 = ret.error;
			} else {
				assert(context_save == &disp_tilepro->enabled_save_area);
				context_save->r0 = ret.error;
			}

			// dispatch listener to receive the msg
			//printf("Dispatching the receiver on core %u...\n", my_core_id);
			dispatch(listener);
			tilepro_panic("Dispatch returned!\n");  // should not reach here
		}
	} else {  // if cap is not endpoint, call kernel handler through dispatch table

		uint8_t cmd = (uint8_t)(context_save->r0 & 0xff);
		if(cmd >= CAP_MAX_CMD) {
			ret.error = SYS_ERR_ILLEGAL_INVOCATION;
			return ret;
		}

		// Call the invocation
		invocation_handler_t invocation = invocations[to->type][cmd];
		if(invocation == NULL) {
			ret.error = SYS_ERR_ILLEGAL_INVOCATION;
			return ret;
		} else {
			ret = invocation(to, context_save);
		}
	}

	return ret;
}

/*
 * this function is to handle a breakpoint debug syscall.
 * developers can use this syscall to debug the code manually.
 */
static void handle_debug_set_breakpoint(void)
{
	__insn_mtspr(SPR_SIM_CONTROL, SIM_CONTROL_PANIC);
}

/*
 * this function is to handle debug syscall.
 * haven't completed all.
 *
 * r0 = debug_message.
 */
static struct sysret sys_debug(arch_registers_state_t *context_save)
{
	struct sysret retval = { .error = SYS_ERR_OK };

	switch(context_save->r0) {

	case DEBUG_CONTEXT_COUNTER_RESET:
		dispatch_csc_reset();
		break;

	case DEBUG_CONTEXT_COUNTER_READ:
		retval.value = dispatch_get_csc();
		break;

	case DEBUG_SET_BREAKPOINT:
		handle_debug_set_breakpoint();
		break;

	case DEBUG_GET_TSC_PER_MS:
		// TODO: here is a fake value!
		retval.value = 0;
		break;

	// we have not implemented all the debug options,
	// so we treat them illegal.
	default:
		printk(LOG_ERR, "invalid sys_debug msg type %lu\n", context_save->r0);
		retval.error = err_push(retval.error, SYS_ERR_ILLEGAL_SYSCALL);
		break;
	}

	return retval;
}

/**
 * System call dispatch routine.
 *
 * @param regs, for all the available registers for the system calls.
 * Normally we only use r0-r9 to pass the arguments to syscalls,
 * but sometimes we need to access other registers for some purpose, e.g. context switch,
 * so we put all registers as syscall's arguments.
 *
 * @return struct sysret, for all calls except yield / invoke.
 */
struct sysret sys_syscall(arch_registers_state_t *context_save);
struct sysret sys_syscall(arch_registers_state_t *context_save)
{
	STATIC_ASSERT_OFFSETOF(struct sysret, error, 0);
	struct sysret r = { .error = SYS_ERR_INVARGS_SYSCALL, .value = 0 };

    // get the syscall number saved in r10.
    uint32_t syscall = context_save->r10;

    switch(syscall) {

    case SYSCALL_INVOKE:
    	r = sys_invoke(context_save);
		break;

	case SYSCALL_YIELD:
		// handle to kernel's syscall to yield CPU to the next dispatcher
		r = sys_yield((capaddr_t)context_save->r0);
		break;

	case SYSCALL_NOP:
		// We actually really pause for one cycle :)
		__asm volatile("nop \n\t");
		r.error = SYS_ERR_OK;
		break;

	case SYSCALL_REBOOT:
		reboot();
		break;

	case SYSCALL_PRINT:
		r.error = (hv_console_write((HV_VirtAddr)context_save->r0, context_save->r1) >= 0) ? SYS_ERR_OK : SYS_ERR_INVALID_USER_BUFFER;
		break;

	case SYSCALL_DEBUG:
		r = sys_debug(context_save);
		break;

	default:
		panic("Illegal syscall");
		r.error = SYS_ERR_ILLEGAL_SYSCALL;
		break;
    }  // end of switch-block

    if(r.error) {
    	debug(SUBSYS_SYSCALL, "syscall failed %08"PRIx32" => %08"PRIxERRV"\n", syscall, r.error);
    }

    // If dcb_current got removed, dispatch another one
    if(dcb_current == NULL) {
    	assert(err_is_ok(r.error));
    	dispatch(schedule());
    }

    return r;
}
