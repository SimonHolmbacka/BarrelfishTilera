#ifndef __hpet_IF_H
#define __hpet_IF_H 1
/*
 * Copyright (c) 2010, ETH Zurich.
 * All rights reserved.
 * 
 * INTERFACE NAME: hpet
 * INTEFACE FILE: /home/andreas/barrelfish/new_test2/if/hpet.if
 * INTERFACE DESCRIPTION: The HPET Interface
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
typedef uint64_t hpet_genpaddr_t;
typedef uint64_t hpet_genvaddr_t;
typedef uint32_t hpet_rsrcid_t;
typedef uint32_t hpet_errval_t;
typedef uint32_t hpet_cycles_t;
typedef uint32_t hpet_iref_t;
typedef uint8_t hpet_coreid_t;
typedef uint32_t hpet_domainid_t;

/*
 * Forward declaration of binding type
 */
struct hpet_binding;

/*
 * Contination (callback) and control function types
 */
typedef  void hpet_bind_continuation_fn(void *st, errval_t err, struct hpet_binding *_binding);
typedef  bool hpet_can_send_fn(struct hpet_binding *_binding);
typedef  errval_t hpet_register_send_fn(struct hpet_binding *_binding, struct waitset *ws, struct event_closure _continuation);
typedef  errval_t hpet_change_waitset_fn(struct hpet_binding *_binding, struct waitset *ws);
typedef  errval_t hpet_control_fn(struct hpet_binding *_binding, idc_control_t control);
typedef  void hpet_error_handler_fn(struct hpet_binding *_binding, errval_t err);

/*
 * Enumeration for message numbers
 */
typedef enum hpet_msg_enum {
    hpet___dummy__msgnum = 0,
    hpet_counter_request__msgnum = 1,
    hpet_counter_reply__msgnum = 2
} hpet_msg_enum;

/*
 * Message type signatures (transmit)
 */
typedef  errval_t hpet_counter_request__tx_method_fn(struct hpet_binding *_binding, struct event_closure _continuation);
typedef  errval_t hpet_counter_reply__tx_method_fn(struct hpet_binding *_binding, struct event_closure _continuation, struct capref cap, uint64_t idx);

/*
 * Message type signatures (receive)
 */
typedef  void hpet_counter_request__rx_method_fn(struct hpet_binding *_binding);
typedef  void hpet_counter_reply__rx_method_fn(struct hpet_binding *_binding, struct capref cap, uint64_t idx);

/*
 * Struct type for holding the args for each msg
 */
struct hpet_counter_reply__args {
    struct capref cap;
    uint64_t idx;
};

/*
 * Union type for all message arguments
 */
union hpet_arg_union {
    struct hpet_counter_reply__args counter_reply;
};

/*
 * VTable struct definition for the interface (transmit)
 */
struct hpet_tx_vtbl {
    hpet_counter_request__tx_method_fn *counter_request;
    hpet_counter_reply__tx_method_fn *counter_reply;
};

/*
 * VTable struct definition for the interface (receive)
 */
struct hpet_rx_vtbl {
    hpet_counter_request__rx_method_fn *counter_request;
    hpet_counter_reply__rx_method_fn *counter_reply;
};

/*
 * Incoming connect callback type
 */
typedef  errval_t hpet_connect_fn(void *st, struct hpet_binding *binding);

/*
 * Export state struct
 */
struct hpet_export {
    struct idc_export common;
    hpet_connect_fn *connect_cb;
    struct waitset *waitset;
    void *st;
};

/*
 * Export function
 */
extern  errval_t hpet_export(void *st, idc_export_callback_fn *export_cb, hpet_connect_fn *connect_cb, struct waitset *ws, idc_export_flags_t flags);

/*
 * The Binding structure
 */
struct hpet_binding {
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
    hpet_can_send_fn *can_send;
    
    /* register an event for when a message is likely to be able to be sent */
    hpet_register_send_fn *register_send;
    
    /* change the waitset used by a binding */
    hpet_change_waitset_fn *change_waitset;
    
    /* perform control operations */
    hpet_control_fn *control;
    
    /* error handler for any async errors associated with this binding */
    /* must be filled-in by user */
    hpet_error_handler_fn *error_handler;
    
    /* Message send functions (filled in by binding) */
    struct hpet_tx_vtbl tx_vtbl;
    
    /* Incoming message handlers (filled in by user) */
    struct hpet_rx_vtbl rx_vtbl;
    
    /* Private state belonging to the binding implementation */
    union hpet_arg_union tx_union;
    union hpet_arg_union rx_union;
    struct waitset_chanstate register_chanstate;
    struct waitset_chanstate tx_cont_chanstate;
    enum hpet_msg_enum tx_msgnum;
    enum hpet_msg_enum rx_msgnum;
    int tx_msg_fragment;
    int rx_msg_fragment;
    size_t tx_str_pos;
    size_t rx_str_pos;
    size_t tx_str_len;
    size_t rx_str_len;
    struct event_queue_node event_qnode;
    hpet_bind_continuation_fn *bind_cont;
};

/*
 * Generic bind function
 */
extern  errval_t hpet_bind(iref_t i, hpet_bind_continuation_fn *_continuation, void *st, struct waitset *waitset, idc_bind_flags_t flags);

/*
 * Send wrappers
 */
static inline errval_t hpet_counter_request__tx(struct hpet_binding *_binding, struct event_closure _continuation) __attribute__ ((always_inline));
static inline errval_t hpet_counter_request__tx(struct hpet_binding *_binding, struct event_closure _continuation)
{
    return(((_binding->tx_vtbl).counter_request)(_binding, _continuation));
}

static inline errval_t hpet_counter_reply__tx(struct hpet_binding *_binding, struct event_closure _continuation, struct capref cap, uint64_t idx) __attribute__ ((always_inline));
static inline errval_t hpet_counter_reply__tx(struct hpet_binding *_binding, struct event_closure _continuation, struct capref cap, uint64_t idx)
{
    return(((_binding->tx_vtbl).counter_reply)(_binding, _continuation, cap, idx));
}


/*
 * Backend-specific includes
 */
#ifdef CONFIG_FLOUNDER_BACKEND_LMP
#include <if/hpet_lmp_defs.h>
#endif // CONFIG_FLOUNDER_BACKEND_LMP
#ifdef CONFIG_FLOUNDER_BACKEND_UMP
#include <if/hpet_ump_defs.h>
#endif // CONFIG_FLOUNDER_BACKEND_UMP
#ifdef CONFIG_FLOUNDER_BACKEND_UMP_IPI
#include <if/hpet_ump_ipi_defs.h>
#endif // CONFIG_FLOUNDER_BACKEND_UMP_IPI
#ifdef CONFIG_FLOUNDER_BACKEND_MULTIHOP
#include <if/hpet_multihop_defs.h>
#endif // CONFIG_FLOUNDER_BACKEND_MULTIHOP
#ifdef CONFIG_FLOUNDER_BACKEND_UDN
#include <if/hpet_udn_defs.h>
#endif // CONFIG_FLOUNDER_BACKEND_UDN
/*
 * And we're done
 */
#endif // __hpet_IF_H
