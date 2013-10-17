/*
 * Copyright (c) 2010, ETH Zurich.
 * All rights reserved.
 * 
 * INTERFACE NAME: monitor_mem
 * INTEFACE FILE: /home/andreas/barrelfish/new_test2/if/monitor_mem.if
 * INTERFACE DESCRIPTION: Inter-monitor memory allocation
 * 
 * This file is distributed under the terms in the attached LICENSE
 * file. If you do not find this file, copies can be found by
 * writing to:
 * ETH Zurich D-INFK, Universitaetstr.6, CH-8092 Zurich.
 * Attn: Systems Group.
 * 
 * THIS FILE IS AUTOMATICALLY GENERATED BY FLOUNDER: DO NOT EDIT!
 */

/*
 * Generated Stub for RPC
 */

#include <barrelfish/barrelfish.h>
#include <flounder/flounder_support.h>
#include <if/monitor_mem_rpcclient_defs.h>

/*
 * RPC wrapper functions
 */
static  errval_t monitor_mem_alloc__rpc(struct monitor_mem_rpc_client *_rpc, uint8_t size_bits, monitor_mem_genpaddr_t minbase, monitor_mem_genpaddr_t maxlimit, monitor_mem_errval_t *err, monitor_mem_caprep_t *caprep)
{
    errval_t _err = SYS_ERR_OK;
    assert(!(_rpc->rpc_in_progress));
    assert((_rpc->async_error) == SYS_ERR_OK);
    _rpc->rpc_in_progress = true;
    _rpc->reply_present = false;
    
    // call send function
    _err = ((((_rpc->b)->tx_vtbl).alloc_call)(_rpc->b, NOP_CONT, size_bits, minbase, maxlimit));
    if (err_is_fail(_err)) {
        goto out;
    }
    
    // wait for message to be sent and reply or error to be present
    while (((!(_rpc->reply_present)) || (!(((_rpc->b)->can_send)(_rpc->b)))) && ((_rpc->async_error) == SYS_ERR_OK)) {
        _err = event_dispatch(&(_rpc->rpc_waitset));
        if (err_is_fail(_err)) {
            _err = err_push(_err, LIB_ERR_EVENT_DISPATCH);
            goto out;
        }
    }
    
    if (err_is_fail(_rpc->async_error)) {
        _err = (_rpc->async_error);
        _rpc->async_error = SYS_ERR_OK;
        goto out;
    }
    
    // grab reply variables out of binding
    struct monitor_mem_binding *_binding = _rpc->b;
    *err = (((_binding->rx_union).alloc_response).err);
    *caprep = (((_binding->rx_union).alloc_response).caprep);
    
    out:
    _rpc->rpc_in_progress = false;
    return(_err);
}


/*
 * Receive handlers
 */
static  void monitor_mem_alloc__rpc_rx_handler(struct monitor_mem_binding *_binding, monitor_mem_errval_t err, monitor_mem_caprep_t caprep)
{
    // get RPC client state pointer
    struct monitor_mem_rpc_client *_rpc = _binding->st;
    
    // XXX: stash reply parameters in binding object
    // depending on the interconnect driver, they're probably already there
    ((_binding->rx_union).alloc_response).err = err;
    ((_binding->rx_union).alloc_response).caprep = caprep;
    
    // notify RPC function, and we're done
    _rpc->reply_present = true;
}


/*
 * RPC Vtable
 */
static  struct monitor_mem_rpc_vtbl monitor_mem_rpc_vtbl = {
    .alloc = monitor_mem_alloc__rpc,
};

/*
 * Error handler
 */
static  void monitor_mem_rpc_client_error(struct monitor_mem_binding *_binding, errval_t _err)
{
    // get RPC client state pointer
    struct monitor_mem_rpc_client *_rpc = _binding->st;
    
    if (_rpc->rpc_in_progress) {
        assert(err_is_fail(_err));
        _rpc->async_error = _err;
        // kick waitset with dummy event
        flounder_support_register(&(_rpc->rpc_waitset), &(_rpc->dummy_chanstate), dummy_event_closure, true);
    } else {
        USER_PANIC_ERR(_err, "async error in RPC");
    }
}


/*
 * Init function
 */
 errval_t monitor_mem_rpc_client_init(struct monitor_mem_rpc_client *rpc, struct monitor_mem_binding *binding)
{
    errval_t _err;
    
    // Setup state of RPC client object
    rpc->b = binding;
    rpc->reply_present = false;
    rpc->rpc_in_progress = false;
    rpc->async_error = SYS_ERR_OK;
    waitset_init(&(rpc->rpc_waitset));
    flounder_support_waitset_chanstate_init(&(rpc->dummy_chanstate));
    rpc->vtbl = monitor_mem_rpc_vtbl;
    binding->st = rpc;
    
    // Change waitset on binding
    _err = ((binding->change_waitset)(binding, &(rpc->rpc_waitset)));
    if (err_is_fail(_err)) {
        waitset_destroy(&(rpc->rpc_waitset));
        return(err_push(_err, FLOUNDER_ERR_CHANGE_WAITSET));
    }
    
    // Set RX handlers on binding object for RPCs
    (binding->rx_vtbl).alloc_response = monitor_mem_alloc__rpc_rx_handler;
    
    // Set error handler on binding object
    binding->error_handler = monitor_mem_rpc_client_error;
    
    return(SYS_ERR_OK);
}

