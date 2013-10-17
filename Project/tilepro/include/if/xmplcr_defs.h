#ifndef __xmplcr_IF_H
#define __xmplcr_IF_H 1
/*
 * Copyright (c) 2010, ETH Zurich.
 * All rights reserved.
 * 
 * INTERFACE NAME: xmplcr
 * INTEFACE FILE: /home/andreas/barrelfish/new_test2/if/xmplcr.if
 * INTERFACE DESCRIPTION: Example call reply interface
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
typedef uint64_t xmplcr_genpaddr_t;
typedef uint64_t xmplcr_genvaddr_t;
typedef uint32_t xmplcr_rsrcid_t;
typedef uint32_t xmplcr_errval_t;
typedef uint32_t xmplcr_cycles_t;
typedef uint32_t xmplcr_iref_t;
typedef uint8_t xmplcr_coreid_t;
typedef uint32_t xmplcr_domainid_t;

/*
 * Forward declaration of binding type
 */
struct xmplcr_binding;

/*
 * Contination (callback) and control function types
 */
typedef  void xmplcr_bind_continuation_fn(void *st, errval_t err, struct xmplcr_binding *_binding);
typedef  bool xmplcr_can_send_fn(struct xmplcr_binding *_binding);
typedef  errval_t xmplcr_register_send_fn(struct xmplcr_binding *_binding, struct waitset *ws, struct event_closure _continuation);
typedef  errval_t xmplcr_change_waitset_fn(struct xmplcr_binding *_binding, struct waitset *ws);
typedef  errval_t xmplcr_control_fn(struct xmplcr_binding *_binding, idc_control_t control);
typedef  void xmplcr_error_handler_fn(struct xmplcr_binding *_binding, errval_t err);

/*
 * Enumeration for message numbers
 */
typedef enum xmplcr_msg_enum {
    xmplcr___dummy__msgnum = 0,
    xmplcr_mycall__msgnum = 1,
    xmplcr_myresponse__msgnum = 2
} xmplcr_msg_enum;

/*
 * Message type signatures (transmit)
 */
typedef  errval_t xmplcr_mycall__tx_method_fn(struct xmplcr_binding *_binding, struct event_closure _continuation, int32_t i);
typedef  errval_t xmplcr_myresponse__tx_method_fn(struct xmplcr_binding *_binding, struct event_closure _continuation, const char *s);

/*
 * Message type signatures (receive)
 */
typedef  void xmplcr_mycall__rx_method_fn(struct xmplcr_binding *_binding, int32_t i);
typedef  void xmplcr_myresponse__rx_method_fn(struct xmplcr_binding *_binding, char *s);

/*
 * Struct type for holding the args for each msg
 */
struct xmplcr_mycall__args {
    int32_t i;
};
struct xmplcr_myresponse__args {
    char *s;
};

/*
 * Union type for all message arguments
 */
union xmplcr_arg_union {
    struct xmplcr_mycall__args mycall;
    struct xmplcr_myresponse__args myresponse;
};

/*
 * VTable struct definition for the interface (transmit)
 */
struct xmplcr_tx_vtbl {
    xmplcr_mycall__tx_method_fn *mycall;
    xmplcr_myresponse__tx_method_fn *myresponse;
};

/*
 * VTable struct definition for the interface (receive)
 */
struct xmplcr_rx_vtbl {
    xmplcr_mycall__rx_method_fn *mycall;
    xmplcr_myresponse__rx_method_fn *myresponse;
};

/*
 * Incoming connect callback type
 */
typedef  errval_t xmplcr_connect_fn(void *st, struct xmplcr_binding *binding);

/*
 * Export state struct
 */
struct xmplcr_export {
    struct idc_export common;
    xmplcr_connect_fn *connect_cb;
    struct waitset *waitset;
    void *st;
};

/*
 * Export function
 */
extern  errval_t xmplcr_export(void *st, idc_export_callback_fn *export_cb, xmplcr_connect_fn *connect_cb, struct waitset *ws, idc_export_flags_t flags);

/*
 * The Binding structure
 */
struct xmplcr_binding {
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
    xmplcr_can_send_fn *can_send;
    
    /* register an event for when a message is likely to be able to be sent */
    xmplcr_register_send_fn *register_send;
    
    /* change the waitset used by a binding */
    xmplcr_change_waitset_fn *change_waitset;
    
    /* perform control operations */
    xmplcr_control_fn *control;
    
    /* error handler for any async errors associated with this binding */
    /* must be filled-in by user */
    xmplcr_error_handler_fn *error_handler;
    
    /* Message send functions (filled in by binding) */
    struct xmplcr_tx_vtbl tx_vtbl;
    
    /* Incoming message handlers (filled in by user) */
    struct xmplcr_rx_vtbl rx_vtbl;
    
    /* Private state belonging to the binding implementation */
    union xmplcr_arg_union tx_union;
    union xmplcr_arg_union rx_union;
    struct waitset_chanstate register_chanstate;
    struct waitset_chanstate tx_cont_chanstate;
    enum xmplcr_msg_enum tx_msgnum;
    enum xmplcr_msg_enum rx_msgnum;
    int tx_msg_fragment;
    int rx_msg_fragment;
    size_t tx_str_pos;
    size_t rx_str_pos;
    size_t tx_str_len;
    size_t rx_str_len;
    struct event_queue_node event_qnode;
    xmplcr_bind_continuation_fn *bind_cont;
};

/*
 * Generic bind function
 */
extern  errval_t xmplcr_bind(iref_t i, xmplcr_bind_continuation_fn *_continuation, void *st, struct waitset *waitset, idc_bind_flags_t flags);

/*
 * Send wrappers
 */
static inline errval_t xmplcr_mycall__tx(struct xmplcr_binding *_binding, struct event_closure _continuation, int32_t i) __attribute__ ((always_inline));
static inline errval_t xmplcr_mycall__tx(struct xmplcr_binding *_binding, struct event_closure _continuation, int32_t i)
{
    return(((_binding->tx_vtbl).mycall)(_binding, _continuation, i));
}

static inline errval_t xmplcr_myresponse__tx(struct xmplcr_binding *_binding, struct event_closure _continuation, const char *s) __attribute__ ((always_inline));
static inline errval_t xmplcr_myresponse__tx(struct xmplcr_binding *_binding, struct event_closure _continuation, const char *s)
{
    return(((_binding->tx_vtbl).myresponse)(_binding, _continuation, s));
}


/*
 * Backend-specific includes
 */
#ifdef CONFIG_FLOUNDER_BACKEND_LMP
#include <if/xmplcr_lmp_defs.h>
#endif // CONFIG_FLOUNDER_BACKEND_LMP
#ifdef CONFIG_FLOUNDER_BACKEND_UMP
#include <if/xmplcr_ump_defs.h>
#endif // CONFIG_FLOUNDER_BACKEND_UMP
#ifdef CONFIG_FLOUNDER_BACKEND_UMP_IPI
#include <if/xmplcr_ump_ipi_defs.h>
#endif // CONFIG_FLOUNDER_BACKEND_UMP_IPI
#ifdef CONFIG_FLOUNDER_BACKEND_MULTIHOP
#include <if/xmplcr_multihop_defs.h>
#endif // CONFIG_FLOUNDER_BACKEND_MULTIHOP
#ifdef CONFIG_FLOUNDER_BACKEND_UDN
#include <if/xmplcr_udn_defs.h>
#endif // CONFIG_FLOUNDER_BACKEND_UDN
/*
 * And we're done
 */
#endif // __xmplcr_IF_H