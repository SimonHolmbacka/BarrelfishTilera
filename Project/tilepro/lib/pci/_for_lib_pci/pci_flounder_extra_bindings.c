/*
 * Copyright (c) 2010, ETH Zurich.
 * All rights reserved.
 * 
 * INTERFACE NAME: pci
 * INTEFACE FILE: /home/andreas/barrelfish/new_test2/if/pci.if
 * INTERFACE DESCRIPTION: The PCI Interface
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
#include <if/pci_rpcclient_defs.h>

/*
 * RPC wrapper functions
 */
static  errval_t pci_init_pci_device__rpc(struct pci_rpc_client *_rpc, uint32_t class_code, uint32_t sub_class, uint32_t prog_if, uint32_t vendor_id, uint32_t device_id, uint32_t bus, uint32_t dev, uint32_t fun, uint8_t coreid, uint32_t vector, pci_errval_t *err, uint8_t *nr_allocated_bars, pci_caps_per_bar_t **caps_per_bar)
{
    errval_t _err = SYS_ERR_OK;
    assert(!(_rpc->rpc_in_progress));
    assert((_rpc->async_error) == SYS_ERR_OK);
    _rpc->rpc_in_progress = true;
    _rpc->reply_present = false;
    
    // call send function
    _err = ((((_rpc->b)->tx_vtbl).init_pci_device_call)(_rpc->b, NOP_CONT, class_code, sub_class, prog_if, vendor_id, device_id, bus, dev, fun, coreid, vector));
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
    struct pci_binding *_binding = _rpc->b;
    *err = (((_binding->rx_union).init_pci_device_response).err);
    *nr_allocated_bars = (((_binding->rx_union).init_pci_device_response).nr_allocated_bars);
    *caps_per_bar = (((_binding->rx_union).init_pci_device_response).caps_per_bar);
    
    out:
    _rpc->rpc_in_progress = false;
    return(_err);
}

static  errval_t pci_init_legacy_device__rpc(struct pci_rpc_client *_rpc, uint16_t iomin, uint16_t iomax, uint8_t irq, uint8_t coreid, uint32_t vector, pci_errval_t *err, struct capref *iocap)
{
    errval_t _err = SYS_ERR_OK;
    assert(!(_rpc->rpc_in_progress));
    assert((_rpc->async_error) == SYS_ERR_OK);
    _rpc->rpc_in_progress = true;
    _rpc->reply_present = false;
    
    // call send function
    _err = ((((_rpc->b)->tx_vtbl).init_legacy_device_call)(_rpc->b, NOP_CONT, iomin, iomax, irq, coreid, vector));
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
    struct pci_binding *_binding = _rpc->b;
    *err = (((_binding->rx_union).init_legacy_device_response).err);
    *iocap = (((_binding->rx_union).init_legacy_device_response).iocap);
    
    out:
    _rpc->rpc_in_progress = false;
    return(_err);
}

static  errval_t pci_get_cap__rpc(struct pci_rpc_client *_rpc, uint32_t idx, uint32_t cap_nr, pci_errval_t *err, struct capref *cap, uint8_t *type)
{
    errval_t _err = SYS_ERR_OK;
    assert(!(_rpc->rpc_in_progress));
    assert((_rpc->async_error) == SYS_ERR_OK);
    _rpc->rpc_in_progress = true;
    _rpc->reply_present = false;
    
    // call send function
    _err = ((((_rpc->b)->tx_vtbl).get_cap_call)(_rpc->b, NOP_CONT, idx, cap_nr));
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
    struct pci_binding *_binding = _rpc->b;
    *err = (((_binding->rx_union).get_cap_response).err);
    *cap = (((_binding->rx_union).get_cap_response).cap);
    *type = (((_binding->rx_union).get_cap_response).type);
    
    out:
    _rpc->rpc_in_progress = false;
    return(_err);
}


/*
 * Receive handlers
 */
static  void pci_init_pci_device__rpc_rx_handler(struct pci_binding *_binding, pci_errval_t err, uint8_t nr_allocated_bars, pci_caps_per_bar_t caps_per_bar)
{
    // get RPC client state pointer
    struct pci_rpc_client *_rpc = _binding->st;
    
    // XXX: stash reply parameters in binding object
    // depending on the interconnect driver, they're probably already there
    ((_binding->rx_union).init_pci_device_response).err = err;
    ((_binding->rx_union).init_pci_device_response).nr_allocated_bars = nr_allocated_bars;
    ((_binding->rx_union).init_pci_device_response).caps_per_bar = ((pci_caps_per_bar_t *)(caps_per_bar));
    
    // notify RPC function, and we're done
    _rpc->reply_present = true;
}

static  void pci_init_legacy_device__rpc_rx_handler(struct pci_binding *_binding, pci_errval_t err, struct capref iocap)
{
    // get RPC client state pointer
    struct pci_rpc_client *_rpc = _binding->st;
    
    // XXX: stash reply parameters in binding object
    // depending on the interconnect driver, they're probably already there
    ((_binding->rx_union).init_legacy_device_response).err = err;
    ((_binding->rx_union).init_legacy_device_response).iocap = iocap;
    
    // notify RPC function, and we're done
    _rpc->reply_present = true;
}

static  void pci_get_cap__rpc_rx_handler(struct pci_binding *_binding, pci_errval_t err, struct capref cap, uint8_t type)
{
    // get RPC client state pointer
    struct pci_rpc_client *_rpc = _binding->st;
    
    // XXX: stash reply parameters in binding object
    // depending on the interconnect driver, they're probably already there
    ((_binding->rx_union).get_cap_response).err = err;
    ((_binding->rx_union).get_cap_response).cap = cap;
    ((_binding->rx_union).get_cap_response).type = type;
    
    // notify RPC function, and we're done
    _rpc->reply_present = true;
}


/*
 * RPC Vtable
 */
static  struct pci_rpc_vtbl pci_rpc_vtbl = {
    .init_pci_device = pci_init_pci_device__rpc,
    .init_legacy_device = pci_init_legacy_device__rpc,
    .get_cap = pci_get_cap__rpc,
};

/*
 * Error handler
 */
static  void pci_rpc_client_error(struct pci_binding *_binding, errval_t _err)
{
    // get RPC client state pointer
    struct pci_rpc_client *_rpc = _binding->st;
    
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
 errval_t pci_rpc_client_init(struct pci_rpc_client *rpc, struct pci_binding *binding)
{
    errval_t _err;
    
    // Setup state of RPC client object
    rpc->b = binding;
    rpc->reply_present = false;
    rpc->rpc_in_progress = false;
    rpc->async_error = SYS_ERR_OK;
    waitset_init(&(rpc->rpc_waitset));
    flounder_support_waitset_chanstate_init(&(rpc->dummy_chanstate));
    rpc->vtbl = pci_rpc_vtbl;
    binding->st = rpc;
    
    // Change waitset on binding
    _err = ((binding->change_waitset)(binding, &(rpc->rpc_waitset)));
    if (err_is_fail(_err)) {
        waitset_destroy(&(rpc->rpc_waitset));
        return(err_push(_err, FLOUNDER_ERR_CHANGE_WAITSET));
    }
    
    // Set RX handlers on binding object for RPCs
    (binding->rx_vtbl).init_pci_device_response = pci_init_pci_device__rpc_rx_handler;
    (binding->rx_vtbl).init_legacy_device_response = pci_init_legacy_device__rpc_rx_handler;
    (binding->rx_vtbl).get_cap_response = pci_get_cap__rpc_rx_handler;
    
    // Set error handler on binding object
    binding->error_handler = pci_rpc_client_error;
    
    return(SYS_ERR_OK);
}

