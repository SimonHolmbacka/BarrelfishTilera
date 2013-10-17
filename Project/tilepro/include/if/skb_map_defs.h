#ifndef __skb_map_IF_H
#define __skb_map_IF_H 1
/*
 * Copyright (c) 2010, ETH Zurich.
 * All rights reserved.
 * 
 * INTERFACE NAME: skb_map
 * INTEFACE FILE: /home/andreas/barrelfish/new_test2/if/skb_map.if
 * INTERFACE DESCRIPTION: SKB's map testapp interface
 * 
 * This file is distributed under the terms in the attached LICENSE
 * file. If you do not find this file, copies can be found by
 * writing to:
 * ETH Zurich D-INFK, Universitaetstr.6, CH-8092 Zurich.
 * Attn: Systems Group.
 * 
 * THIS FILE IS AUTOMATICALLY GENERATED BY FLOUNDER: DO NOT EDIT!
 */

#include <flounder/flounder.h>

/*
 * Concrete type definitions
 */
typedef uint64_t skb_map_genpaddr_t;
typedef uint64_t skb_map_genvaddr_t;
typedef uint32_t skb_map_rsrcid_t;
typedef uint32_t skb_map_errval_t;
typedef uint32_t skb_map_cycles_t;
typedef uint32_t skb_map_iref_t;
typedef uint8_t skb_map_coreid_t;
typedef uint32_t skb_map_domainid_t;

/*
 * Forward declaration of binding type
 */
struct skb_map_binding;

/*
 * Contination (callback) and control function types
 */
typedef  void skb_map_bind_continuation_fn(void *st, errval_t err, struct skb_map_binding *_binding);
typedef  bool skb_map_can_send_fn(struct skb_map_binding *_binding);
typedef  errval_t skb_map_register_send_fn(struct skb_map_binding *_binding, struct waitset *ws, struct event_closure _continuation);
typedef  errval_t skb_map_change_waitset_fn(struct skb_map_binding *_binding, struct waitset *ws);
typedef  errval_t skb_map_control_fn(struct skb_map_binding *_binding, idc_control_t control);
typedef  void skb_map_error_handler_fn(struct skb_map_binding *_binding, errval_t err);

/*
 * Enumeration for message numbers
 */
typedef enum skb_map_msg_enum {
    skb_map___dummy__msgnum = 0,
    skb_map_sharepage__msgnum = 1,
    skb_map_mapfunction__msgnum = 2,
    skb_map_mapfinished__msgnum = 3,
    skb_map_initialized__msgnum = 4
} skb_map_msg_enum;

/*
 * Message type signatures (transmit)
 */
typedef  errval_t skb_map_sharepage__tx_method_fn(struct skb_map_binding *_binding, struct event_closure _continuation, struct capref cap, uint64_t pagenumber);
typedef  errval_t skb_map_mapfunction__tx_method_fn(struct skb_map_binding *_binding, struct event_closure _continuation, uint64_t functionnumber, uint64_t startaddress, uint64_t endaddress);
typedef  errval_t skb_map_mapfinished__tx_method_fn(struct skb_map_binding *_binding, struct event_closure _continuation, uint64_t coreid);
typedef  errval_t skb_map_initialized__tx_method_fn(struct skb_map_binding *_binding, struct event_closure _continuation, uint64_t core_id);

/*
 * Message type signatures (receive)
 */
typedef  void skb_map_sharepage__rx_method_fn(struct skb_map_binding *_binding, struct capref cap, uint64_t pagenumber);
typedef  void skb_map_mapfunction__rx_method_fn(struct skb_map_binding *_binding, uint64_t functionnumber, uint64_t startaddress, uint64_t endaddress);
typedef  void skb_map_mapfinished__rx_method_fn(struct skb_map_binding *_binding, uint64_t coreid);
typedef  void skb_map_initialized__rx_method_fn(struct skb_map_binding *_binding, uint64_t core_id);

/*
 * Struct type for holding the args for each msg
 */
struct skb_map_sharepage__args {
    struct capref cap;
    uint64_t pagenumber;
};
struct skb_map_mapfunction__args {
    uint64_t functionnumber;
    uint64_t startaddress;
    uint64_t endaddress;
};
struct skb_map_mapfinished__args {
    uint64_t coreid;
};
struct skb_map_initialized__args {
    uint64_t core_id;
};

/*
 * Union type for all message arguments
 */
union skb_map_arg_union {
    struct skb_map_sharepage__args sharepage;
    struct skb_map_mapfunction__args mapfunction;
    struct skb_map_mapfinished__args mapfinished;
    struct skb_map_initialized__args initialized;
};

/*
 * VTable struct definition for the interface (transmit)
 */
struct skb_map_tx_vtbl {
    skb_map_sharepage__tx_method_fn *sharepage;
    skb_map_mapfunction__tx_method_fn *mapfunction;
    skb_map_mapfinished__tx_method_fn *mapfinished;
    skb_map_initialized__tx_method_fn *initialized;
};

/*
 * VTable struct definition for the interface (receive)
 */
struct skb_map_rx_vtbl {
    skb_map_sharepage__rx_method_fn *sharepage;
    skb_map_mapfunction__rx_method_fn *mapfunction;
    skb_map_mapfinished__rx_method_fn *mapfinished;
    skb_map_initialized__rx_method_fn *initialized;
};

/*
 * Incoming connect callback type
 */
typedef  errval_t skb_map_connect_fn(void *st, struct skb_map_binding *binding);

/*
 * Export state struct
 */
struct skb_map_export {
    struct idc_export common;
    skb_map_connect_fn *connect_cb;
    struct waitset *waitset;
    void *st;
};

/*
 * Export function
 */
extern  errval_t skb_map_export(void *st, idc_export_callback_fn *export_cb, skb_map_connect_fn *connect_cb, struct waitset *ws, idc_export_flags_t flags);

/*
 * The Binding structure
 */
struct skb_map_binding {
    /* Arbitrary user state pointer */
    void *st;
    
    /* Waitset used for receive handlers and send continuations */
    struct waitset *waitset;
    
    /* Mutex for the use of user code. */
    /* Must be held before any operation where there is a possibility of */
    /* concurrent access to the same binding (eg. multiple threads, or */
    /* asynchronous event handlers that use the same binding object). */
    struct event_mutex mutex;
    
    /* returns true iff a message could currently be accepted by the binding */
    skb_map_can_send_fn *can_send;
    
    /* register an event for when a message is likely to be able to be sent */
    skb_map_register_send_fn *register_send;
    
    /* change the waitset used by a binding */
    skb_map_change_waitset_fn *change_waitset;
    
    /* perform control operations */
    skb_map_control_fn *control;
    
    /* error handler for any async errors associated with this binding */
    /* must be filled-in by user */
    skb_map_error_handler_fn *error_handler;
    
    /* Message send functions (filled in by binding) */
    struct skb_map_tx_vtbl tx_vtbl;
    
    /* Incoming message handlers (filled in by user) */
    struct skb_map_rx_vtbl rx_vtbl;
    
    /* Private state belonging to the binding implementation */
    union skb_map_arg_union tx_union;
    union skb_map_arg_union rx_union;
    struct waitset_chanstate register_chanstate;
    struct waitset_chanstate tx_cont_chanstate;
    enum skb_map_msg_enum tx_msgnum;
    enum skb_map_msg_enum rx_msgnum;
    int tx_msg_fragment;
    int rx_msg_fragment;
    size_t tx_str_pos;
    size_t rx_str_pos;
    size_t tx_str_len;
    size_t rx_str_len;
    struct event_queue_node event_qnode;
    skb_map_bind_continuation_fn *bind_cont;
};

/*
 * Generic bind function
 */
extern  errval_t skb_map_bind(iref_t i, skb_map_bind_continuation_fn *_continuation, void *st, struct waitset *waitset, idc_bind_flags_t flags);

/*
 * Send wrappers
 */
static inline errval_t skb_map_sharepage__tx(struct skb_map_binding *_binding, struct event_closure _continuation, struct capref cap, uint64_t pagenumber) __attribute__ ((always_inline));
static inline errval_t skb_map_sharepage__tx(struct skb_map_binding *_binding, struct event_closure _continuation, struct capref cap, uint64_t pagenumber)
{
    return(((_binding->tx_vtbl).sharepage)(_binding, _continuation, cap, pagenumber));
}

static inline errval_t skb_map_mapfunction__tx(struct skb_map_binding *_binding, struct event_closure _continuation, uint64_t functionnumber, uint64_t startaddress, uint64_t endaddress) __attribute__ ((always_inline));
static inline errval_t skb_map_mapfunction__tx(struct skb_map_binding *_binding, struct event_closure _continuation, uint64_t functionnumber, uint64_t startaddress, uint64_t endaddress)
{
    return(((_binding->tx_vtbl).mapfunction)(_binding, _continuation, functionnumber, startaddress, endaddress));
}

static inline errval_t skb_map_mapfinished__tx(struct skb_map_binding *_binding, struct event_closure _continuation, uint64_t coreid) __attribute__ ((always_inline));
static inline errval_t skb_map_mapfinished__tx(struct skb_map_binding *_binding, struct event_closure _continuation, uint64_t coreid)
{
    return(((_binding->tx_vtbl).mapfinished)(_binding, _continuation, coreid));
}

static inline errval_t skb_map_initialized__tx(struct skb_map_binding *_binding, struct event_closure _continuation, uint64_t core_id) __attribute__ ((always_inline));
static inline errval_t skb_map_initialized__tx(struct skb_map_binding *_binding, struct event_closure _continuation, uint64_t core_id)
{
    return(((_binding->tx_vtbl).initialized)(_binding, _continuation, core_id));
}


/*
 * Backend-specific includes
 */
#ifdef CONFIG_FLOUNDER_BACKEND_LMP
#include <if/skb_map_lmp_defs.h>
#endif // CONFIG_FLOUNDER_BACKEND_LMP
#ifdef CONFIG_FLOUNDER_BACKEND_UMP
#include <if/skb_map_ump_defs.h>
#endif // CONFIG_FLOUNDER_BACKEND_UMP
#ifdef CONFIG_FLOUNDER_BACKEND_UMP_IPI
#include <if/skb_map_ump_ipi_defs.h>
#endif // CONFIG_FLOUNDER_BACKEND_UMP_IPI
#ifdef CONFIG_FLOUNDER_BACKEND_MULTIHOP
#include <if/skb_map_multihop_defs.h>
#endif // CONFIG_FLOUNDER_BACKEND_MULTIHOP
#ifdef CONFIG_FLOUNDER_BACKEND_UDN
#include <if/skb_map_udn_defs.h>
#endif // CONFIG_FLOUNDER_BACKEND_UDN
/*
 * And we're done
 */
#endif // __skb_map_IF_H
