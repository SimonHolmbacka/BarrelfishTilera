#ifndef __interdisp_IF_H
#define __interdisp_IF_H 1
/*
 * Copyright (c) 2010, ETH Zurich.
 * All rights reserved.
 * 
 * INTERFACE NAME: interdisp
 * INTEFACE FILE: /home/andreas/barrelfish/new_test2/if/interdisp.if
 * INTERFACE DESCRIPTION: Interface between domains spanning cores
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
typedef uint64_t interdisp_genpaddr_t;
typedef uint64_t interdisp_genvaddr_t;
typedef uint32_t interdisp_rsrcid_t;
typedef uint32_t interdisp_errval_t;
typedef uint32_t interdisp_cycles_t;
typedef uint32_t interdisp_iref_t;
typedef uint8_t interdisp_coreid_t;
typedef uint32_t interdisp_domainid_t;

/*
 * Forward declaration of binding type
 */
struct interdisp_binding;

/*
 * Contination (callback) and control function types
 */
typedef  void interdisp_bind_continuation_fn(void *st, errval_t err, struct interdisp_binding *_binding);
typedef  bool interdisp_can_send_fn(struct interdisp_binding *_binding);
typedef  errval_t interdisp_register_send_fn(struct interdisp_binding *_binding, struct waitset *ws, struct event_closure _continuation);
typedef  errval_t interdisp_change_waitset_fn(struct interdisp_binding *_binding, struct waitset *ws);
typedef  errval_t interdisp_control_fn(struct interdisp_binding *_binding, idc_control_t control);
typedef  void interdisp_error_handler_fn(struct interdisp_binding *_binding, errval_t err);

/*
 * Enumeration for message numbers
 */
typedef enum interdisp_msg_enum {
    interdisp___dummy__msgnum = 0,
    interdisp_dispatcher_initialized__msgnum = 1,
    interdisp_send_cap_request__msgnum = 2,
    interdisp_send_cap_reply__msgnum = 3,
    interdisp_wakeup_thread__msgnum = 4,
    interdisp_create_thread__msgnum = 5,
    interdisp_span_slave__msgnum = 6,
    interdisp_span_slave_done__msgnum = 7,
    interdisp_span_eager_connect__msgnum = 8
} interdisp_msg_enum;

/*
 * Message type signatures (transmit)
 */
typedef  errval_t interdisp_dispatcher_initialized__tx_method_fn(struct interdisp_binding *_binding, struct event_closure _continuation, interdisp_genvaddr_t id);
typedef  errval_t interdisp_send_cap_request__tx_method_fn(struct interdisp_binding *_binding, struct event_closure _continuation, struct capref cap, interdisp_genvaddr_t info);
typedef  errval_t interdisp_send_cap_reply__tx_method_fn(struct interdisp_binding *_binding, struct event_closure _continuation, interdisp_errval_t err);
typedef  errval_t interdisp_wakeup_thread__tx_method_fn(struct interdisp_binding *_binding, struct event_closure _continuation, interdisp_genvaddr_t thread);
typedef  errval_t interdisp_create_thread__tx_method_fn(struct interdisp_binding *_binding, struct event_closure _continuation, interdisp_genvaddr_t start_func, interdisp_genvaddr_t arg, uint64_t stacksize);
typedef  errval_t interdisp_span_slave__tx_method_fn(struct interdisp_binding *_binding, struct event_closure _continuation);
typedef  errval_t interdisp_span_slave_done__tx_method_fn(struct interdisp_binding *_binding, struct event_closure _continuation);
typedef  errval_t interdisp_span_eager_connect__tx_method_fn(struct interdisp_binding *_binding, struct event_closure _continuation, interdisp_coreid_t core_id);

/*
 * Message type signatures (receive)
 */
typedef  void interdisp_dispatcher_initialized__rx_method_fn(struct interdisp_binding *_binding, interdisp_genvaddr_t id);
typedef  void interdisp_send_cap_request__rx_method_fn(struct interdisp_binding *_binding, struct capref cap, interdisp_genvaddr_t info);
typedef  void interdisp_send_cap_reply__rx_method_fn(struct interdisp_binding *_binding, interdisp_errval_t err);
typedef  void interdisp_wakeup_thread__rx_method_fn(struct interdisp_binding *_binding, interdisp_genvaddr_t thread);
typedef  void interdisp_create_thread__rx_method_fn(struct interdisp_binding *_binding, interdisp_genvaddr_t start_func, interdisp_genvaddr_t arg, uint64_t stacksize);
typedef  void interdisp_span_slave__rx_method_fn(struct interdisp_binding *_binding);
typedef  void interdisp_span_slave_done__rx_method_fn(struct interdisp_binding *_binding);
typedef  void interdisp_span_eager_connect__rx_method_fn(struct interdisp_binding *_binding, interdisp_coreid_t core_id);

/*
 * Struct type for holding the args for each msg
 */
struct interdisp_dispatcher_initialized__args {
    interdisp_genvaddr_t id;
};
struct interdisp_send_cap_request__args {
    struct capref cap;
    interdisp_genvaddr_t info;
};
struct interdisp_send_cap_reply__args {
    interdisp_errval_t err;
};
struct interdisp_wakeup_thread__args {
    interdisp_genvaddr_t thread;
};
struct interdisp_create_thread__args {
    interdisp_genvaddr_t start_func;
    interdisp_genvaddr_t arg;
    uint64_t stacksize;
};
struct interdisp_span_eager_connect__args {
    interdisp_coreid_t core_id;
};

/*
 * Union type for all message arguments
 */
union interdisp_arg_union {
    struct interdisp_dispatcher_initialized__args dispatcher_initialized;
    struct interdisp_send_cap_request__args send_cap_request;
    struct interdisp_send_cap_reply__args send_cap_reply;
    struct interdisp_wakeup_thread__args wakeup_thread;
    struct interdisp_create_thread__args create_thread;
    struct interdisp_span_eager_connect__args span_eager_connect;
};

/*
 * VTable struct definition for the interface (transmit)
 */
struct interdisp_tx_vtbl {
    interdisp_dispatcher_initialized__tx_method_fn *dispatcher_initialized;
    interdisp_send_cap_request__tx_method_fn *send_cap_request;
    interdisp_send_cap_reply__tx_method_fn *send_cap_reply;
    interdisp_wakeup_thread__tx_method_fn *wakeup_thread;
    interdisp_create_thread__tx_method_fn *create_thread;
    interdisp_span_slave__tx_method_fn *span_slave;
    interdisp_span_slave_done__tx_method_fn *span_slave_done;
    interdisp_span_eager_connect__tx_method_fn *span_eager_connect;
};

/*
 * VTable struct definition for the interface (receive)
 */
struct interdisp_rx_vtbl {
    interdisp_dispatcher_initialized__rx_method_fn *dispatcher_initialized;
    interdisp_send_cap_request__rx_method_fn *send_cap_request;
    interdisp_send_cap_reply__rx_method_fn *send_cap_reply;
    interdisp_wakeup_thread__rx_method_fn *wakeup_thread;
    interdisp_create_thread__rx_method_fn *create_thread;
    interdisp_span_slave__rx_method_fn *span_slave;
    interdisp_span_slave_done__rx_method_fn *span_slave_done;
    interdisp_span_eager_connect__rx_method_fn *span_eager_connect;
};

/*
 * Incoming connect callback type
 */
typedef  errval_t interdisp_connect_fn(void *st, struct interdisp_binding *binding);

/*
 * Export state struct
 */
struct interdisp_export {
    struct idc_export common;
    interdisp_connect_fn *connect_cb;
    struct waitset *waitset;
    void *st;
};

/*
 * Export function
 */
extern  errval_t interdisp_export(void *st, idc_export_callback_fn *export_cb, interdisp_connect_fn *connect_cb, struct waitset *ws, idc_export_flags_t flags);

/*
 * The Binding structure
 */
struct interdisp_binding {
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
    interdisp_can_send_fn *can_send;
    
    /* register an event for when a message is likely to be able to be sent */
    interdisp_register_send_fn *register_send;
    
    /* change the waitset used by a binding */
    interdisp_change_waitset_fn *change_waitset;
    
    /* perform control operations */
    interdisp_control_fn *control;
    
    /* error handler for any async errors associated with this binding */
    /* must be filled-in by user */
    interdisp_error_handler_fn *error_handler;
    
    /* Message send functions (filled in by binding) */
    struct interdisp_tx_vtbl tx_vtbl;
    
    /* Incoming message handlers (filled in by user) */
    struct interdisp_rx_vtbl rx_vtbl;
    
    /* Private state belonging to the binding implementation */
    union interdisp_arg_union tx_union;
    union interdisp_arg_union rx_union;
    struct waitset_chanstate register_chanstate;
    struct waitset_chanstate tx_cont_chanstate;
    enum interdisp_msg_enum tx_msgnum;
    enum interdisp_msg_enum rx_msgnum;
    int tx_msg_fragment;
    int rx_msg_fragment;
    size_t tx_str_pos;
    size_t rx_str_pos;
    size_t tx_str_len;
    size_t rx_str_len;
    struct event_queue_node event_qnode;
    interdisp_bind_continuation_fn *bind_cont;
};

/*
 * Generic bind function
 */
extern  errval_t interdisp_bind(iref_t i, interdisp_bind_continuation_fn *_continuation, void *st, struct waitset *waitset, idc_bind_flags_t flags);

/*
 * Send wrappers
 */
static inline errval_t interdisp_dispatcher_initialized__tx(struct interdisp_binding *_binding, struct event_closure _continuation, interdisp_genvaddr_t id) __attribute__ ((always_inline));
static inline errval_t interdisp_dispatcher_initialized__tx(struct interdisp_binding *_binding, struct event_closure _continuation, interdisp_genvaddr_t id)
{
    return(((_binding->tx_vtbl).dispatcher_initialized)(_binding, _continuation, id));
}

static inline errval_t interdisp_send_cap_request__tx(struct interdisp_binding *_binding, struct event_closure _continuation, struct capref cap, interdisp_genvaddr_t info) __attribute__ ((always_inline));
static inline errval_t interdisp_send_cap_request__tx(struct interdisp_binding *_binding, struct event_closure _continuation, struct capref cap, interdisp_genvaddr_t info)
{
    return(((_binding->tx_vtbl).send_cap_request)(_binding, _continuation, cap, info));
}

static inline errval_t interdisp_send_cap_reply__tx(struct interdisp_binding *_binding, struct event_closure _continuation, interdisp_errval_t err) __attribute__ ((always_inline));
static inline errval_t interdisp_send_cap_reply__tx(struct interdisp_binding *_binding, struct event_closure _continuation, interdisp_errval_t err)
{
    return(((_binding->tx_vtbl).send_cap_reply)(_binding, _continuation, err));
}

static inline errval_t interdisp_wakeup_thread__tx(struct interdisp_binding *_binding, struct event_closure _continuation, interdisp_genvaddr_t thread) __attribute__ ((always_inline));
static inline errval_t interdisp_wakeup_thread__tx(struct interdisp_binding *_binding, struct event_closure _continuation, interdisp_genvaddr_t thread)
{
    return(((_binding->tx_vtbl).wakeup_thread)(_binding, _continuation, thread));
}

static inline errval_t interdisp_create_thread__tx(struct interdisp_binding *_binding, struct event_closure _continuation, interdisp_genvaddr_t start_func, interdisp_genvaddr_t arg, uint64_t stacksize) __attribute__ ((always_inline));
static inline errval_t interdisp_create_thread__tx(struct interdisp_binding *_binding, struct event_closure _continuation, interdisp_genvaddr_t start_func, interdisp_genvaddr_t arg, uint64_t stacksize)
{
    return(((_binding->tx_vtbl).create_thread)(_binding, _continuation, start_func, arg, stacksize));
}

static inline errval_t interdisp_span_slave__tx(struct interdisp_binding *_binding, struct event_closure _continuation) __attribute__ ((always_inline));
static inline errval_t interdisp_span_slave__tx(struct interdisp_binding *_binding, struct event_closure _continuation)
{
    return(((_binding->tx_vtbl).span_slave)(_binding, _continuation));
}

static inline errval_t interdisp_span_slave_done__tx(struct interdisp_binding *_binding, struct event_closure _continuation) __attribute__ ((always_inline));
static inline errval_t interdisp_span_slave_done__tx(struct interdisp_binding *_binding, struct event_closure _continuation)
{
    return(((_binding->tx_vtbl).span_slave_done)(_binding, _continuation));
}

static inline errval_t interdisp_span_eager_connect__tx(struct interdisp_binding *_binding, struct event_closure _continuation, interdisp_coreid_t core_id) __attribute__ ((always_inline));
static inline errval_t interdisp_span_eager_connect__tx(struct interdisp_binding *_binding, struct event_closure _continuation, interdisp_coreid_t core_id)
{
    return(((_binding->tx_vtbl).span_eager_connect)(_binding, _continuation, core_id));
}


/*
 * Backend-specific includes
 */
#ifdef CONFIG_FLOUNDER_BACKEND_LMP
#include <if/interdisp_lmp_defs.h>
#endif // CONFIG_FLOUNDER_BACKEND_LMP
#ifdef CONFIG_FLOUNDER_BACKEND_UMP
#include <if/interdisp_ump_defs.h>
#endif // CONFIG_FLOUNDER_BACKEND_UMP
#ifdef CONFIG_FLOUNDER_BACKEND_UMP_IPI
#include <if/interdisp_ump_ipi_defs.h>
#endif // CONFIG_FLOUNDER_BACKEND_UMP_IPI
#ifdef CONFIG_FLOUNDER_BACKEND_MULTIHOP
#include <if/interdisp_multihop_defs.h>
#endif // CONFIG_FLOUNDER_BACKEND_MULTIHOP
#ifdef CONFIG_FLOUNDER_BACKEND_UDN
#include <if/interdisp_udn_defs.h>
#endif // CONFIG_FLOUNDER_BACKEND_UDN
/*
 * And we're done
 */
#endif // __interdisp_IF_H
