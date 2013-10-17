/*
 * Copyright (c) 2010, ETH Zurich.
 * All rights reserved.
 * 
 * INTERFACE NAME: octopus
 * INTEFACE FILE: /home/andreas/barrelfish/new_test2/if/octopus.if
 * INTERFACE DESCRIPTION: octopus RPC Interface
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
#include <if/octopus_rpcclient_defs.h>

/*
 * RPC wrapper functions
 */
static  errval_t octopus_get_identifier__rpc(struct octopus_rpc_client *_rpc, uint64_t *id)
{
    errval_t _err = SYS_ERR_OK;
    assert(!(_rpc->rpc_in_progress));
    assert((_rpc->async_error) == SYS_ERR_OK);
    _rpc->rpc_in_progress = true;
    _rpc->reply_present = false;
    
    // call send function
    _err = ((((_rpc->b)->tx_vtbl).get_identifier_call)(_rpc->b, NOP_CONT));
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
    struct octopus_binding *_binding = _rpc->b;
    *id = (((_binding->rx_union).get_identifier_response).id);
    
    out:
    _rpc->rpc_in_progress = false;
    return(_err);
}

static  errval_t octopus_identify__rpc(struct octopus_rpc_client *_rpc, uint64_t id, octopus_binding_type_t type)
{
    errval_t _err = SYS_ERR_OK;
    assert(!(_rpc->rpc_in_progress));
    assert((_rpc->async_error) == SYS_ERR_OK);
    _rpc->rpc_in_progress = true;
    _rpc->reply_present = false;
    
    // call send function
    _err = ((((_rpc->b)->tx_vtbl).identify_call)(_rpc->b, NOP_CONT, id, type));
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
    
    out:
    _rpc->rpc_in_progress = false;
    return(_err);
}

static  errval_t octopus_get_names__rpc(struct octopus_rpc_client *_rpc, const char *query, octopus_trigger_t t, char **output, octopus_trigger_id_t *tid, octopus_errval_t *error_code)
{
    errval_t _err = SYS_ERR_OK;
    assert(!(_rpc->rpc_in_progress));
    assert((_rpc->async_error) == SYS_ERR_OK);
    _rpc->rpc_in_progress = true;
    _rpc->reply_present = false;
    
    // call send function
    _err = ((((_rpc->b)->tx_vtbl).get_names_call)(_rpc->b, NOP_CONT, query, t));
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
    struct octopus_binding *_binding = _rpc->b;
    *output = (((_binding->rx_union).get_names_response).output);
    *tid = (((_binding->rx_union).get_names_response).tid);
    *error_code = (((_binding->rx_union).get_names_response).error_code);
    
    out:
    _rpc->rpc_in_progress = false;
    return(_err);
}

static  errval_t octopus_get__rpc(struct octopus_rpc_client *_rpc, const char *query, octopus_trigger_t t, char **output, octopus_trigger_id_t *tid, octopus_errval_t *error_code)
{
    errval_t _err = SYS_ERR_OK;
    assert(!(_rpc->rpc_in_progress));
    assert((_rpc->async_error) == SYS_ERR_OK);
    _rpc->rpc_in_progress = true;
    _rpc->reply_present = false;
    
    // call send function
    _err = ((((_rpc->b)->tx_vtbl).get_call)(_rpc->b, NOP_CONT, query, t));
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
    struct octopus_binding *_binding = _rpc->b;
    *output = (((_binding->rx_union).get_response).output);
    *tid = (((_binding->rx_union).get_response).tid);
    *error_code = (((_binding->rx_union).get_response).error_code);
    
    out:
    _rpc->rpc_in_progress = false;
    return(_err);
}

static  errval_t octopus_set__rpc(struct octopus_rpc_client *_rpc, const char *query, uint64_t mode, octopus_trigger_t t, bool get, char **record, octopus_trigger_id_t *tid, octopus_errval_t *error_code)
{
    errval_t _err = SYS_ERR_OK;
    assert(!(_rpc->rpc_in_progress));
    assert((_rpc->async_error) == SYS_ERR_OK);
    _rpc->rpc_in_progress = true;
    _rpc->reply_present = false;
    
    // call send function
    _err = ((((_rpc->b)->tx_vtbl).set_call)(_rpc->b, NOP_CONT, query, mode, t, get));
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
    struct octopus_binding *_binding = _rpc->b;
    *record = (((_binding->rx_union).set_response).record);
    *tid = (((_binding->rx_union).set_response).tid);
    *error_code = (((_binding->rx_union).set_response).error_code);
    
    out:
    _rpc->rpc_in_progress = false;
    return(_err);
}

static  errval_t octopus_del__rpc(struct octopus_rpc_client *_rpc, const char *query, octopus_trigger_t t, octopus_trigger_id_t *tid, octopus_errval_t *error_code)
{
    errval_t _err = SYS_ERR_OK;
    assert(!(_rpc->rpc_in_progress));
    assert((_rpc->async_error) == SYS_ERR_OK);
    _rpc->rpc_in_progress = true;
    _rpc->reply_present = false;
    
    // call send function
    _err = ((((_rpc->b)->tx_vtbl).del_call)(_rpc->b, NOP_CONT, query, t));
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
    struct octopus_binding *_binding = _rpc->b;
    *tid = (((_binding->rx_union).del_response).tid);
    *error_code = (((_binding->rx_union).del_response).error_code);
    
    out:
    _rpc->rpc_in_progress = false;
    return(_err);
}

static  errval_t octopus_exists__rpc(struct octopus_rpc_client *_rpc, const char *query, octopus_trigger_t t, octopus_trigger_id_t *tid, octopus_errval_t *error_code)
{
    errval_t _err = SYS_ERR_OK;
    assert(!(_rpc->rpc_in_progress));
    assert((_rpc->async_error) == SYS_ERR_OK);
    _rpc->rpc_in_progress = true;
    _rpc->reply_present = false;
    
    // call send function
    _err = ((((_rpc->b)->tx_vtbl).exists_call)(_rpc->b, NOP_CONT, query, t));
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
    struct octopus_binding *_binding = _rpc->b;
    *tid = (((_binding->rx_union).exists_response).tid);
    *error_code = (((_binding->rx_union).exists_response).error_code);
    
    out:
    _rpc->rpc_in_progress = false;
    return(_err);
}

static  errval_t octopus_wait_for__rpc(struct octopus_rpc_client *_rpc, const char *query, char **record, octopus_errval_t *error_code)
{
    errval_t _err = SYS_ERR_OK;
    assert(!(_rpc->rpc_in_progress));
    assert((_rpc->async_error) == SYS_ERR_OK);
    _rpc->rpc_in_progress = true;
    _rpc->reply_present = false;
    
    // call send function
    _err = ((((_rpc->b)->tx_vtbl).wait_for_call)(_rpc->b, NOP_CONT, query));
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
    struct octopus_binding *_binding = _rpc->b;
    *record = (((_binding->rx_union).wait_for_response).record);
    *error_code = (((_binding->rx_union).wait_for_response).error_code);
    
    out:
    _rpc->rpc_in_progress = false;
    return(_err);
}

static  errval_t octopus_remove_trigger__rpc(struct octopus_rpc_client *_rpc, uint64_t id, octopus_errval_t *error_code)
{
    errval_t _err = SYS_ERR_OK;
    assert(!(_rpc->rpc_in_progress));
    assert((_rpc->async_error) == SYS_ERR_OK);
    _rpc->rpc_in_progress = true;
    _rpc->reply_present = false;
    
    // call send function
    _err = ((((_rpc->b)->tx_vtbl).remove_trigger_call)(_rpc->b, NOP_CONT, id));
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
    struct octopus_binding *_binding = _rpc->b;
    *error_code = (((_binding->rx_union).remove_trigger_response).error_code);
    
    out:
    _rpc->rpc_in_progress = false;
    return(_err);
}

static  errval_t octopus_subscribe__rpc(struct octopus_rpc_client *_rpc, const char *query, uint64_t trigger_fn, uint64_t state, uint64_t *id, octopus_errval_t *error_code)
{
    errval_t _err = SYS_ERR_OK;
    assert(!(_rpc->rpc_in_progress));
    assert((_rpc->async_error) == SYS_ERR_OK);
    _rpc->rpc_in_progress = true;
    _rpc->reply_present = false;
    
    // call send function
    _err = ((((_rpc->b)->tx_vtbl).subscribe_call)(_rpc->b, NOP_CONT, query, trigger_fn, state));
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
    struct octopus_binding *_binding = _rpc->b;
    *id = (((_binding->rx_union).subscribe_response).id);
    *error_code = (((_binding->rx_union).subscribe_response).error_code);
    
    out:
    _rpc->rpc_in_progress = false;
    return(_err);
}

static  errval_t octopus_unsubscribe__rpc(struct octopus_rpc_client *_rpc, uint64_t id, octopus_errval_t *error_code)
{
    errval_t _err = SYS_ERR_OK;
    assert(!(_rpc->rpc_in_progress));
    assert((_rpc->async_error) == SYS_ERR_OK);
    _rpc->rpc_in_progress = true;
    _rpc->reply_present = false;
    
    // call send function
    _err = ((((_rpc->b)->tx_vtbl).unsubscribe_call)(_rpc->b, NOP_CONT, id));
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
    struct octopus_binding *_binding = _rpc->b;
    *error_code = (((_binding->rx_union).unsubscribe_response).error_code);
    
    out:
    _rpc->rpc_in_progress = false;
    return(_err);
}

static  errval_t octopus_publish__rpc(struct octopus_rpc_client *_rpc, const char *record, octopus_errval_t *error_code)
{
    errval_t _err = SYS_ERR_OK;
    assert(!(_rpc->rpc_in_progress));
    assert((_rpc->async_error) == SYS_ERR_OK);
    _rpc->rpc_in_progress = true;
    _rpc->reply_present = false;
    
    // call send function
    _err = ((((_rpc->b)->tx_vtbl).publish_call)(_rpc->b, NOP_CONT, record));
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
    struct octopus_binding *_binding = _rpc->b;
    *error_code = (((_binding->rx_union).publish_response).error_code);
    
    out:
    _rpc->rpc_in_progress = false;
    return(_err);
}

static  errval_t octopus_get_cap__rpc(struct octopus_rpc_client *_rpc, const char *key, struct capref *retcap, octopus_errval_t *reterr)
{
    errval_t _err = SYS_ERR_OK;
    assert(!(_rpc->rpc_in_progress));
    assert((_rpc->async_error) == SYS_ERR_OK);
    _rpc->rpc_in_progress = true;
    _rpc->reply_present = false;
    
    // call send function
    _err = ((((_rpc->b)->tx_vtbl).get_cap_call)(_rpc->b, NOP_CONT, key));
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
    struct octopus_binding *_binding = _rpc->b;
    *retcap = (((_binding->rx_union).get_cap_response).retcap);
    *reterr = (((_binding->rx_union).get_cap_response).reterr);
    
    out:
    _rpc->rpc_in_progress = false;
    return(_err);
}

static  errval_t octopus_put_cap__rpc(struct octopus_rpc_client *_rpc, const char *key, struct capref storecap, octopus_errval_t *reterr)
{
    errval_t _err = SYS_ERR_OK;
    assert(!(_rpc->rpc_in_progress));
    assert((_rpc->async_error) == SYS_ERR_OK);
    _rpc->rpc_in_progress = true;
    _rpc->reply_present = false;
    
    // call send function
    _err = ((((_rpc->b)->tx_vtbl).put_cap_call)(_rpc->b, NOP_CONT, key, storecap));
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
    struct octopus_binding *_binding = _rpc->b;
    *reterr = (((_binding->rx_union).put_cap_response).reterr);
    
    out:
    _rpc->rpc_in_progress = false;
    return(_err);
}

static  errval_t octopus_remove_cap__rpc(struct octopus_rpc_client *_rpc, const char *key, octopus_errval_t *reterr)
{
    errval_t _err = SYS_ERR_OK;
    assert(!(_rpc->rpc_in_progress));
    assert((_rpc->async_error) == SYS_ERR_OK);
    _rpc->rpc_in_progress = true;
    _rpc->reply_present = false;
    
    // call send function
    _err = ((((_rpc->b)->tx_vtbl).remove_cap_call)(_rpc->b, NOP_CONT, key));
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
    struct octopus_binding *_binding = _rpc->b;
    *reterr = (((_binding->rx_union).remove_cap_response).reterr);
    
    out:
    _rpc->rpc_in_progress = false;
    return(_err);
}


/*
 * Receive handlers
 */
static  void octopus_get_identifier__rpc_rx_handler(struct octopus_binding *_binding, uint64_t id)
{
    // get RPC client state pointer
    struct octopus_rpc_client *_rpc = _binding->st;
    
    // XXX: stash reply parameters in binding object
    // depending on the interconnect driver, they're probably already there
    ((_binding->rx_union).get_identifier_response).id = id;
    
    // notify RPC function, and we're done
    _rpc->reply_present = true;
}

static  void octopus_identify__rpc_rx_handler(struct octopus_binding *_binding)
{
    // get RPC client state pointer
    struct octopus_rpc_client *_rpc = _binding->st;
    
    // XXX: stash reply parameters in binding object
    // depending on the interconnect driver, they're probably already there
    
    // notify RPC function, and we're done
    _rpc->reply_present = true;
}

static  void octopus_get_names__rpc_rx_handler(struct octopus_binding *_binding, char *output, octopus_trigger_id_t tid, octopus_errval_t error_code)
{
    // get RPC client state pointer
    struct octopus_rpc_client *_rpc = _binding->st;
    
    // XXX: stash reply parameters in binding object
    // depending on the interconnect driver, they're probably already there
    ((_binding->rx_union).get_names_response).output = output;
    ((_binding->rx_union).get_names_response).tid = tid;
    ((_binding->rx_union).get_names_response).error_code = error_code;
    
    // notify RPC function, and we're done
    _rpc->reply_present = true;
}

static  void octopus_get__rpc_rx_handler(struct octopus_binding *_binding, char *output, octopus_trigger_id_t tid, octopus_errval_t error_code)
{
    // get RPC client state pointer
    struct octopus_rpc_client *_rpc = _binding->st;
    
    // XXX: stash reply parameters in binding object
    // depending on the interconnect driver, they're probably already there
    ((_binding->rx_union).get_response).output = output;
    ((_binding->rx_union).get_response).tid = tid;
    ((_binding->rx_union).get_response).error_code = error_code;
    
    // notify RPC function, and we're done
    _rpc->reply_present = true;
}

static  void octopus_set__rpc_rx_handler(struct octopus_binding *_binding, char *record, octopus_trigger_id_t tid, octopus_errval_t error_code)
{
    // get RPC client state pointer
    struct octopus_rpc_client *_rpc = _binding->st;
    
    // XXX: stash reply parameters in binding object
    // depending on the interconnect driver, they're probably already there
    ((_binding->rx_union).set_response).record = record;
    ((_binding->rx_union).set_response).tid = tid;
    ((_binding->rx_union).set_response).error_code = error_code;
    
    // notify RPC function, and we're done
    _rpc->reply_present = true;
}

static  void octopus_del__rpc_rx_handler(struct octopus_binding *_binding, octopus_trigger_id_t tid, octopus_errval_t error_code)
{
    // get RPC client state pointer
    struct octopus_rpc_client *_rpc = _binding->st;
    
    // XXX: stash reply parameters in binding object
    // depending on the interconnect driver, they're probably already there
    ((_binding->rx_union).del_response).tid = tid;
    ((_binding->rx_union).del_response).error_code = error_code;
    
    // notify RPC function, and we're done
    _rpc->reply_present = true;
}

static  void octopus_exists__rpc_rx_handler(struct octopus_binding *_binding, octopus_trigger_id_t tid, octopus_errval_t error_code)
{
    // get RPC client state pointer
    struct octopus_rpc_client *_rpc = _binding->st;
    
    // XXX: stash reply parameters in binding object
    // depending on the interconnect driver, they're probably already there
    ((_binding->rx_union).exists_response).tid = tid;
    ((_binding->rx_union).exists_response).error_code = error_code;
    
    // notify RPC function, and we're done
    _rpc->reply_present = true;
}

static  void octopus_wait_for__rpc_rx_handler(struct octopus_binding *_binding, char *record, octopus_errval_t error_code)
{
    // get RPC client state pointer
    struct octopus_rpc_client *_rpc = _binding->st;
    
    // XXX: stash reply parameters in binding object
    // depending on the interconnect driver, they're probably already there
    ((_binding->rx_union).wait_for_response).record = record;
    ((_binding->rx_union).wait_for_response).error_code = error_code;
    
    // notify RPC function, and we're done
    _rpc->reply_present = true;
}

static  void octopus_remove_trigger__rpc_rx_handler(struct octopus_binding *_binding, octopus_errval_t error_code)
{
    // get RPC client state pointer
    struct octopus_rpc_client *_rpc = _binding->st;
    
    // XXX: stash reply parameters in binding object
    // depending on the interconnect driver, they're probably already there
    ((_binding->rx_union).remove_trigger_response).error_code = error_code;
    
    // notify RPC function, and we're done
    _rpc->reply_present = true;
}

static  void octopus_subscribe__rpc_rx_handler(struct octopus_binding *_binding, uint64_t id, octopus_errval_t error_code)
{
    // get RPC client state pointer
    struct octopus_rpc_client *_rpc = _binding->st;
    
    // XXX: stash reply parameters in binding object
    // depending on the interconnect driver, they're probably already there
    ((_binding->rx_union).subscribe_response).id = id;
    ((_binding->rx_union).subscribe_response).error_code = error_code;
    
    // notify RPC function, and we're done
    _rpc->reply_present = true;
}

static  void octopus_unsubscribe__rpc_rx_handler(struct octopus_binding *_binding, octopus_errval_t error_code)
{
    // get RPC client state pointer
    struct octopus_rpc_client *_rpc = _binding->st;
    
    // XXX: stash reply parameters in binding object
    // depending on the interconnect driver, they're probably already there
    ((_binding->rx_union).unsubscribe_response).error_code = error_code;
    
    // notify RPC function, and we're done
    _rpc->reply_present = true;
}

static  void octopus_publish__rpc_rx_handler(struct octopus_binding *_binding, octopus_errval_t error_code)
{
    // get RPC client state pointer
    struct octopus_rpc_client *_rpc = _binding->st;
    
    // XXX: stash reply parameters in binding object
    // depending on the interconnect driver, they're probably already there
    ((_binding->rx_union).publish_response).error_code = error_code;
    
    // notify RPC function, and we're done
    _rpc->reply_present = true;
}

static  void octopus_get_cap__rpc_rx_handler(struct octopus_binding *_binding, struct capref retcap, octopus_errval_t reterr)
{
    // get RPC client state pointer
    struct octopus_rpc_client *_rpc = _binding->st;
    
    // XXX: stash reply parameters in binding object
    // depending on the interconnect driver, they're probably already there
    ((_binding->rx_union).get_cap_response).retcap = retcap;
    ((_binding->rx_union).get_cap_response).reterr = reterr;
    
    // notify RPC function, and we're done
    _rpc->reply_present = true;
}

static  void octopus_put_cap__rpc_rx_handler(struct octopus_binding *_binding, octopus_errval_t reterr)
{
    // get RPC client state pointer
    struct octopus_rpc_client *_rpc = _binding->st;
    
    // XXX: stash reply parameters in binding object
    // depending on the interconnect driver, they're probably already there
    ((_binding->rx_union).put_cap_response).reterr = reterr;
    
    // notify RPC function, and we're done
    _rpc->reply_present = true;
}

static  void octopus_remove_cap__rpc_rx_handler(struct octopus_binding *_binding, octopus_errval_t reterr)
{
    // get RPC client state pointer
    struct octopus_rpc_client *_rpc = _binding->st;
    
    // XXX: stash reply parameters in binding object
    // depending on the interconnect driver, they're probably already there
    ((_binding->rx_union).remove_cap_response).reterr = reterr;
    
    // notify RPC function, and we're done
    _rpc->reply_present = true;
}


/*
 * RPC Vtable
 */
static  struct octopus_rpc_vtbl octopus_rpc_vtbl = {
    .get_identifier = octopus_get_identifier__rpc,
    .identify = octopus_identify__rpc,
    .get_names = octopus_get_names__rpc,
    .get = octopus_get__rpc,
    .set = octopus_set__rpc,
    .del = octopus_del__rpc,
    .exists = octopus_exists__rpc,
    .wait_for = octopus_wait_for__rpc,
    .remove_trigger = octopus_remove_trigger__rpc,
    .subscribe = octopus_subscribe__rpc,
    .unsubscribe = octopus_unsubscribe__rpc,
    .publish = octopus_publish__rpc,
    .get_cap = octopus_get_cap__rpc,
    .put_cap = octopus_put_cap__rpc,
    .remove_cap = octopus_remove_cap__rpc,
};

/*
 * Error handler
 */
static  void octopus_rpc_client_error(struct octopus_binding *_binding, errval_t _err)
{
    // get RPC client state pointer
    struct octopus_rpc_client *_rpc = _binding->st;
    
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
 errval_t octopus_rpc_client_init(struct octopus_rpc_client *rpc, struct octopus_binding *binding)
{
    errval_t _err;
    
    // Setup state of RPC client object
    rpc->b = binding;
    rpc->reply_present = false;
    rpc->rpc_in_progress = false;
    rpc->async_error = SYS_ERR_OK;
    waitset_init(&(rpc->rpc_waitset));
    flounder_support_waitset_chanstate_init(&(rpc->dummy_chanstate));
    rpc->vtbl = octopus_rpc_vtbl;
    binding->st = rpc;
    
    // Change waitset on binding
    _err = ((binding->change_waitset)(binding, &(rpc->rpc_waitset)));
    if (err_is_fail(_err)) {
        waitset_destroy(&(rpc->rpc_waitset));
        return(err_push(_err, FLOUNDER_ERR_CHANGE_WAITSET));
    }
    
    // Set RX handlers on binding object for RPCs
    (binding->rx_vtbl).get_identifier_response = octopus_get_identifier__rpc_rx_handler;
    (binding->rx_vtbl).identify_response = octopus_identify__rpc_rx_handler;
    (binding->rx_vtbl).get_names_response = octopus_get_names__rpc_rx_handler;
    (binding->rx_vtbl).get_response = octopus_get__rpc_rx_handler;
    (binding->rx_vtbl).set_response = octopus_set__rpc_rx_handler;
    (binding->rx_vtbl).del_response = octopus_del__rpc_rx_handler;
    (binding->rx_vtbl).exists_response = octopus_exists__rpc_rx_handler;
    (binding->rx_vtbl).wait_for_response = octopus_wait_for__rpc_rx_handler;
    (binding->rx_vtbl).remove_trigger_response = octopus_remove_trigger__rpc_rx_handler;
    (binding->rx_vtbl).subscribe_response = octopus_subscribe__rpc_rx_handler;
    (binding->rx_vtbl).unsubscribe_response = octopus_unsubscribe__rpc_rx_handler;
    (binding->rx_vtbl).publish_response = octopus_publish__rpc_rx_handler;
    (binding->rx_vtbl).get_cap_response = octopus_get_cap__rpc_rx_handler;
    (binding->rx_vtbl).put_cap_response = octopus_put_cap__rpc_rx_handler;
    (binding->rx_vtbl).remove_cap_response = octopus_remove_cap__rpc_rx_handler;
    
    // Set error handler on binding object
    binding->error_handler = octopus_rpc_client_error;
    
    return(SYS_ERR_OK);
}

