#ifndef __mem_IF_H
#define __mem_IF_H 1
/*
 * Copyright (c) 2010, ETH Zurich.
 * All rights reserved.
 * 
 * INTERFACE NAME: mem
 * INTEFACE FILE: /home/andreas/barrelfish/new_test2/if/mem.if
 * INTERFACE DESCRIPTION: Memory allocation RPC interface
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
typedef uint64_t mem_genpaddr_t;
typedef uint64_t mem_genvaddr_t;
typedef uint32_t mem_rsrcid_t;
typedef uint32_t mem_errval_t;
typedef uint32_t mem_cycles_t;
typedef uint32_t mem_iref_t;
typedef uint8_t mem_coreid_t;
typedef uint32_t mem_domainid_t;

/*
 * Forward declaration of binding type
 */
struct mem_binding;

/*
 * Contination (callback) and control function types
 */
typedef  void mem_bind_continuation_fn(void *st, errval_t err, struct mem_binding *_binding);
typedef  bool mem_can_send_fn(struct mem_binding *_binding);
typedef  errval_t mem_register_send_fn(struct mem_binding *_binding, struct waitset *ws, struct event_closure _continuation);
typedef  errval_t mem_change_waitset_fn(struct mem_binding *_binding, struct waitset *ws);
typedef  errval_t mem_control_fn(struct mem_binding *_binding, idc_control_t control);
typedef  void mem_error_handler_fn(struct mem_binding *_binding, errval_t err);

/*
 * Enumeration for message numbers
 */
typedef enum mem_msg_enum {
    mem___dummy__msgnum = 0,
    mem_allocate_call__msgnum = 1,
    mem_allocate_response__msgnum = 2,
    mem_steal_call__msgnum = 3,
    mem_steal_response__msgnum = 4,
    mem_available_call__msgnum = 5,
    mem_available_response__msgnum = 6,
    mem_free_monitor_call__msgnum = 7,
    mem_free_monitor_response__msgnum = 8
} mem_msg_enum;

/*
 * Message type signatures (transmit)
 */
typedef  errval_t mem_allocate_call__tx_method_fn(struct mem_binding *_binding, struct event_closure _continuation, uint8_t bits, mem_genpaddr_t minbase, mem_genpaddr_t maxlimit);
typedef  errval_t mem_allocate_response__tx_method_fn(struct mem_binding *_binding, struct event_closure _continuation, mem_errval_t ret, struct capref mem_cap);
typedef  errval_t mem_steal_call__tx_method_fn(struct mem_binding *_binding, struct event_closure _continuation, uint8_t bits, mem_genpaddr_t minbase, mem_genpaddr_t maxlimit);
typedef  errval_t mem_steal_response__tx_method_fn(struct mem_binding *_binding, struct event_closure _continuation, mem_errval_t ret, struct capref mem_cap);
typedef  errval_t mem_available_call__tx_method_fn(struct mem_binding *_binding, struct event_closure _continuation);
typedef  errval_t mem_available_response__tx_method_fn(struct mem_binding *_binding, struct event_closure _continuation, mem_genpaddr_t mem_avail, mem_genpaddr_t mem_total);
typedef  errval_t mem_free_monitor_call__tx_method_fn(struct mem_binding *_binding, struct event_closure _continuation, struct capref mem_cap, mem_genpaddr_t base, uint8_t bits);
typedef  errval_t mem_free_monitor_response__tx_method_fn(struct mem_binding *_binding, struct event_closure _continuation, mem_errval_t err);

/*
 * Message type signatures (receive)
 */
typedef  void mem_allocate_call__rx_method_fn(struct mem_binding *_binding, uint8_t bits, mem_genpaddr_t minbase, mem_genpaddr_t maxlimit);
typedef  void mem_allocate_response__rx_method_fn(struct mem_binding *_binding, mem_errval_t ret, struct capref mem_cap);
typedef  void mem_steal_call__rx_method_fn(struct mem_binding *_binding, uint8_t bits, mem_genpaddr_t minbase, mem_genpaddr_t maxlimit);
typedef  void mem_steal_response__rx_method_fn(struct mem_binding *_binding, mem_errval_t ret, struct capref mem_cap);
typedef  void mem_available_call__rx_method_fn(struct mem_binding *_binding);
typedef  void mem_available_response__rx_method_fn(struct mem_binding *_binding, mem_genpaddr_t mem_avail, mem_genpaddr_t mem_total);
typedef  void mem_free_monitor_call__rx_method_fn(struct mem_binding *_binding, struct capref mem_cap, mem_genpaddr_t base, uint8_t bits);
typedef  void mem_free_monitor_response__rx_method_fn(struct mem_binding *_binding, mem_errval_t err);

/*
 * Struct type for holding the args for each msg
 */
struct mem_allocate_call__args {
    uint8_t bits;
    mem_genpaddr_t minbase;
    mem_genpaddr_t maxlimit;
};
struct mem_allocate_response__args {
    mem_errval_t ret;
    struct capref mem_cap;
};
struct mem_steal_call__args {
    uint8_t bits;
    mem_genpaddr_t minbase;
    mem_genpaddr_t maxlimit;
};
struct mem_steal_response__args {
    mem_errval_t ret;
    struct capref mem_cap;
};
struct mem_available_response__args {
    mem_genpaddr_t mem_avail;
    mem_genpaddr_t mem_total;
};
struct mem_free_monitor_call__args {
    struct capref mem_cap;
    mem_genpaddr_t base;
    uint8_t bits;
};
struct mem_free_monitor_response__args {
    mem_errval_t err;
};

/*
 * Union type for all message arguments
 */
union mem_arg_union {
    struct mem_allocate_call__args allocate_call;
    struct mem_allocate_response__args allocate_response;
    struct mem_steal_call__args steal_call;
    struct mem_steal_response__args steal_response;
    struct mem_available_response__args available_response;
    struct mem_free_monitor_call__args free_monitor_call;
    struct mem_free_monitor_response__args free_monitor_response;
};

/*
 * VTable struct definition for the interface (transmit)
 */
struct mem_tx_vtbl {
    mem_allocate_call__tx_method_fn *allocate_call;
    mem_allocate_response__tx_method_fn *allocate_response;
    mem_steal_call__tx_method_fn *steal_call;
    mem_steal_response__tx_method_fn *steal_response;
    mem_available_call__tx_method_fn *available_call;
    mem_available_response__tx_method_fn *available_response;
    mem_free_monitor_call__tx_method_fn *free_monitor_call;
    mem_free_monitor_response__tx_method_fn *free_monitor_response;
};

/*
 * VTable struct definition for the interface (receive)
 */
struct mem_rx_vtbl {
    mem_allocate_call__rx_method_fn *allocate_call;
    mem_allocate_response__rx_method_fn *allocate_response;
    mem_steal_call__rx_method_fn *steal_call;
    mem_steal_response__rx_method_fn *steal_response;
    mem_available_call__rx_method_fn *available_call;
    mem_available_response__rx_method_fn *available_response;
    mem_free_monitor_call__rx_method_fn *free_monitor_call;
    mem_free_monitor_response__rx_method_fn *free_monitor_response;
};

/*
 * Incoming connect callback type
 */
typedef  errval_t mem_connect_fn(void *st, struct mem_binding *binding);

/*
 * Export state struct
 */
struct mem_export {
    struct idc_export common;
    mem_connect_fn *connect_cb;
    struct waitset *waitset;
    void *st;
};

/*
 * Export function
 */
extern  errval_t mem_export(void *st, idc_export_callback_fn *export_cb, mem_connect_fn *connect_cb, struct waitset *ws, idc_export_flags_t flags);

/*
 * The Binding structure
 */
struct mem_binding {
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
    mem_can_send_fn *can_send;
    
    /* register an event for when a message is likely to be able to be sent */
    mem_register_send_fn *register_send;
    
    /* change the waitset used by a binding */
    mem_change_waitset_fn *change_waitset;
    
    /* perform control operations */
    mem_control_fn *control;
    
    /* error handler for any async errors associated with this binding */
    /* must be filled-in by user */
    mem_error_handler_fn *error_handler;
    
    /* Message send functions (filled in by binding) */
    struct mem_tx_vtbl tx_vtbl;
    
    /* Incoming message handlers (filled in by user) */
    struct mem_rx_vtbl rx_vtbl;
    
    /* Private state belonging to the binding implementation */
    union mem_arg_union tx_union;
    union mem_arg_union rx_union;
    struct waitset_chanstate register_chanstate;
    struct waitset_chanstate tx_cont_chanstate;
    enum mem_msg_enum tx_msgnum;
    enum mem_msg_enum rx_msgnum;
    int tx_msg_fragment;
    int rx_msg_fragment;
    size_t tx_str_pos;
    size_t rx_str_pos;
    size_t tx_str_len;
    size_t rx_str_len;
    struct event_queue_node event_qnode;
    mem_bind_continuation_fn *bind_cont;
};

/*
 * Generic bind function
 */
extern  errval_t mem_bind(iref_t i, mem_bind_continuation_fn *_continuation, void *st, struct waitset *waitset, idc_bind_flags_t flags);

/*
 * Send wrappers
 */
static inline errval_t mem_allocate_call__tx(struct mem_binding *_binding, struct event_closure _continuation, uint8_t bits, mem_genpaddr_t minbase, mem_genpaddr_t maxlimit) __attribute__ ((always_inline));
static inline errval_t mem_allocate_call__tx(struct mem_binding *_binding, struct event_closure _continuation, uint8_t bits, mem_genpaddr_t minbase, mem_genpaddr_t maxlimit)
{
    return(((_binding->tx_vtbl).allocate_call)(_binding, _continuation, bits, minbase, maxlimit));
}

static inline errval_t mem_allocate_response__tx(struct mem_binding *_binding, struct event_closure _continuation, mem_errval_t ret, struct capref mem_cap) __attribute__ ((always_inline));
static inline errval_t mem_allocate_response__tx(struct mem_binding *_binding, struct event_closure _continuation, mem_errval_t ret, struct capref mem_cap)
{
    return(((_binding->tx_vtbl).allocate_response)(_binding, _continuation, ret, mem_cap));
}

static inline errval_t mem_steal_call__tx(struct mem_binding *_binding, struct event_closure _continuation, uint8_t bits, mem_genpaddr_t minbase, mem_genpaddr_t maxlimit) __attribute__ ((always_inline));
static inline errval_t mem_steal_call__tx(struct mem_binding *_binding, struct event_closure _continuation, uint8_t bits, mem_genpaddr_t minbase, mem_genpaddr_t maxlimit)
{
    return(((_binding->tx_vtbl).steal_call)(_binding, _continuation, bits, minbase, maxlimit));
}

static inline errval_t mem_steal_response__tx(struct mem_binding *_binding, struct event_closure _continuation, mem_errval_t ret, struct capref mem_cap) __attribute__ ((always_inline));
static inline errval_t mem_steal_response__tx(struct mem_binding *_binding, struct event_closure _continuation, mem_errval_t ret, struct capref mem_cap)
{
    return(((_binding->tx_vtbl).steal_response)(_binding, _continuation, ret, mem_cap));
}

static inline errval_t mem_available_call__tx(struct mem_binding *_binding, struct event_closure _continuation) __attribute__ ((always_inline));
static inline errval_t mem_available_call__tx(struct mem_binding *_binding, struct event_closure _continuation)
{
    return(((_binding->tx_vtbl).available_call)(_binding, _continuation));
}

static inline errval_t mem_available_response__tx(struct mem_binding *_binding, struct event_closure _continuation, mem_genpaddr_t mem_avail, mem_genpaddr_t mem_total) __attribute__ ((always_inline));
static inline errval_t mem_available_response__tx(struct mem_binding *_binding, struct event_closure _continuation, mem_genpaddr_t mem_avail, mem_genpaddr_t mem_total)
{
    return(((_binding->tx_vtbl).available_response)(_binding, _continuation, mem_avail, mem_total));
}

static inline errval_t mem_free_monitor_call__tx(struct mem_binding *_binding, struct event_closure _continuation, struct capref mem_cap, mem_genpaddr_t base, uint8_t bits) __attribute__ ((always_inline));
static inline errval_t mem_free_monitor_call__tx(struct mem_binding *_binding, struct event_closure _continuation, struct capref mem_cap, mem_genpaddr_t base, uint8_t bits)
{
    return(((_binding->tx_vtbl).free_monitor_call)(_binding, _continuation, mem_cap, base, bits));
}

static inline errval_t mem_free_monitor_response__tx(struct mem_binding *_binding, struct event_closure _continuation, mem_errval_t err) __attribute__ ((always_inline));
static inline errval_t mem_free_monitor_response__tx(struct mem_binding *_binding, struct event_closure _continuation, mem_errval_t err)
{
    return(((_binding->tx_vtbl).free_monitor_response)(_binding, _continuation, err));
}


/*
 * Backend-specific includes
 */
#ifdef CONFIG_FLOUNDER_BACKEND_LMP
#include <if/mem_lmp_defs.h>
#endif // CONFIG_FLOUNDER_BACKEND_LMP
#ifdef CONFIG_FLOUNDER_BACKEND_UMP
#include <if/mem_ump_defs.h>
#endif // CONFIG_FLOUNDER_BACKEND_UMP
#ifdef CONFIG_FLOUNDER_BACKEND_UMP_IPI
#include <if/mem_ump_ipi_defs.h>
#endif // CONFIG_FLOUNDER_BACKEND_UMP_IPI
#ifdef CONFIG_FLOUNDER_BACKEND_MULTIHOP
#include <if/mem_multihop_defs.h>
#endif // CONFIG_FLOUNDER_BACKEND_MULTIHOP
#ifdef CONFIG_FLOUNDER_BACKEND_UDN
#include <if/mem_udn_defs.h>
#endif // CONFIG_FLOUNDER_BACKEND_UDN
/*
 * And we're done
 */
#endif // __mem_IF_H
