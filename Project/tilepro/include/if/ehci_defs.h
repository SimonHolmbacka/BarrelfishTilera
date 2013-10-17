#ifndef __ehci_IF_H
#define __ehci_IF_H 1
/*
 * Copyright (c) 2010, ETH Zurich.
 * All rights reserved.
 * 
 * INTERFACE NAME: ehci
 * INTEFACE FILE: /home/andreas/barrelfish/new_test2/if/ehci.if
 * INTERFACE DESCRIPTION: Host Controller (EHCI) services
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
typedef uint64_t ehci_genpaddr_t;
typedef uint64_t ehci_genvaddr_t;
typedef uint32_t ehci_rsrcid_t;
typedef uint32_t ehci_errval_t;
typedef uint32_t ehci_cycles_t;
typedef uint32_t ehci_iref_t;
typedef uint8_t ehci_coreid_t;
typedef uint32_t ehci_domainid_t;
struct _ehci_udr__struct {
    uint8_t bmRequestType;
    uint8_t bRequest;
    uint16_t wValue;
    uint16_t wIndex;
    uint16_t wLength;
};
typedef struct _ehci_udr__struct ehci_udr_t;
struct _ehci_pipe__struct {
    uint8_t dev;
    uint8_t ep_number;
    uint8_t ep_address;
    uint8_t ep_dir;
    uint8_t ep_type;
    uint16_t ep_psz;
    uint8_t multi;
    int32_t valid;
};
typedef struct _ehci_pipe__struct ehci_pipe_t;

/*
 * Forward declaration of binding type
 */
struct ehci_binding;

/*
 * Contination (callback) and control function types
 */
typedef  void ehci_bind_continuation_fn(void *st, errval_t err, struct ehci_binding *_binding);
typedef  bool ehci_can_send_fn(struct ehci_binding *_binding);
typedef  errval_t ehci_register_send_fn(struct ehci_binding *_binding, struct waitset *ws, struct event_closure _continuation);
typedef  errval_t ehci_change_waitset_fn(struct ehci_binding *_binding, struct waitset *ws);
typedef  errval_t ehci_control_fn(struct ehci_binding *_binding, idc_control_t control);
typedef  void ehci_error_handler_fn(struct ehci_binding *_binding, errval_t err);

/*
 * Enumeration for message numbers
 */
typedef enum ehci_msg_enum {
    ehci___dummy__msgnum = 0,
    ehci_map_dev_arr__msgnum = 1,
    ehci_map_dev_arr_done__msgnum = 2,
    ehci_dctrl_exe__msgnum = 3,
    ehci_dctrl_done__msgnum = 4,
    ehci_ctrl_exe__msgnum = 5,
    ehci_ctrl_done__msgnum = 6,
    ehci_bulk_exe__msgnum = 7,
    ehci_bulk_done__msgnum = 8,
    ehci_get_core_id__msgnum = 9,
    ehci_get_core_id_resp__msgnum = 10
} ehci_msg_enum;

/*
 * Message type signatures (transmit)
 */
typedef  errval_t ehci_map_dev_arr__tx_method_fn(struct ehci_binding *_binding, struct event_closure _continuation, struct capref ref, uint32_t sz);
typedef  errval_t ehci_map_dev_arr_done__tx_method_fn(struct ehci_binding *_binding, struct event_closure _continuation, uint8_t rsp);
typedef  errval_t ehci_dctrl_exe__tx_method_fn(struct ehci_binding *_binding, struct event_closure _continuation, ehci_udr_t usb_req, uint64_t buff_ptr, uint64_t sz, uint8_t device, uint8_t debug, uint32_t id);
typedef  errval_t ehci_dctrl_done__tx_method_fn(struct ehci_binding *_binding, struct event_closure _continuation, uint32_t id);
typedef  errval_t ehci_ctrl_exe__tx_method_fn(struct ehci_binding *_binding, struct event_closure _continuation, ehci_udr_t usb_req, uint8_t device, uint8_t debug, uint32_t id);
typedef  errval_t ehci_ctrl_done__tx_method_fn(struct ehci_binding *_binding, struct event_closure _continuation, uint32_t id);
typedef  errval_t ehci_bulk_exe__tx_method_fn(struct ehci_binding *_binding, struct event_closure _continuation, ehci_pipe_t p, uint64_t buff_ptr, uint32_t len, uint8_t debug, uint32_t id);
typedef  errval_t ehci_bulk_done__tx_method_fn(struct ehci_binding *_binding, struct event_closure _continuation, uint32_t id);
typedef  errval_t ehci_get_core_id__tx_method_fn(struct ehci_binding *_binding, struct event_closure _continuation);
typedef  errval_t ehci_get_core_id_resp__tx_method_fn(struct ehci_binding *_binding, struct event_closure _continuation, uint64_t core);

/*
 * Message type signatures (receive)
 */
typedef  void ehci_map_dev_arr__rx_method_fn(struct ehci_binding *_binding, struct capref ref, uint32_t sz);
typedef  void ehci_map_dev_arr_done__rx_method_fn(struct ehci_binding *_binding, uint8_t rsp);
typedef  void ehci_dctrl_exe__rx_method_fn(struct ehci_binding *_binding, ehci_udr_t usb_req, uint64_t buff_ptr, uint64_t sz, uint8_t device, uint8_t debug, uint32_t id);
typedef  void ehci_dctrl_done__rx_method_fn(struct ehci_binding *_binding, uint32_t id);
typedef  void ehci_ctrl_exe__rx_method_fn(struct ehci_binding *_binding, ehci_udr_t usb_req, uint8_t device, uint8_t debug, uint32_t id);
typedef  void ehci_ctrl_done__rx_method_fn(struct ehci_binding *_binding, uint32_t id);
typedef  void ehci_bulk_exe__rx_method_fn(struct ehci_binding *_binding, ehci_pipe_t p, uint64_t buff_ptr, uint32_t len, uint8_t debug, uint32_t id);
typedef  void ehci_bulk_done__rx_method_fn(struct ehci_binding *_binding, uint32_t id);
typedef  void ehci_get_core_id__rx_method_fn(struct ehci_binding *_binding);
typedef  void ehci_get_core_id_resp__rx_method_fn(struct ehci_binding *_binding, uint64_t core);

/*
 * Struct type for holding the args for each msg
 */
struct ehci_map_dev_arr__args {
    struct capref ref;
    uint32_t sz;
};
struct ehci_map_dev_arr_done__args {
    uint8_t rsp;
};
struct ehci_dctrl_exe__args {
    ehci_udr_t usb_req;
    uint64_t buff_ptr;
    uint64_t sz;
    uint8_t device;
    uint8_t debug;
    uint32_t id;
};
struct ehci_dctrl_done__args {
    uint32_t id;
};
struct ehci_ctrl_exe__args {
    ehci_udr_t usb_req;
    uint8_t device;
    uint8_t debug;
    uint32_t id;
};
struct ehci_ctrl_done__args {
    uint32_t id;
};
struct ehci_bulk_exe__args {
    ehci_pipe_t p;
    uint64_t buff_ptr;
    uint32_t len;
    uint8_t debug;
    uint32_t id;
};
struct ehci_bulk_done__args {
    uint32_t id;
};
struct ehci_get_core_id_resp__args {
    uint64_t core;
};

/*
 * Union type for all message arguments
 */
union ehci_arg_union {
    struct ehci_map_dev_arr__args map_dev_arr;
    struct ehci_map_dev_arr_done__args map_dev_arr_done;
    struct ehci_dctrl_exe__args dctrl_exe;
    struct ehci_dctrl_done__args dctrl_done;
    struct ehci_ctrl_exe__args ctrl_exe;
    struct ehci_ctrl_done__args ctrl_done;
    struct ehci_bulk_exe__args bulk_exe;
    struct ehci_bulk_done__args bulk_done;
    struct ehci_get_core_id_resp__args get_core_id_resp;
};

/*
 * VTable struct definition for the interface (transmit)
 */
struct ehci_tx_vtbl {
    ehci_map_dev_arr__tx_method_fn *map_dev_arr;
    ehci_map_dev_arr_done__tx_method_fn *map_dev_arr_done;
    ehci_dctrl_exe__tx_method_fn *dctrl_exe;
    ehci_dctrl_done__tx_method_fn *dctrl_done;
    ehci_ctrl_exe__tx_method_fn *ctrl_exe;
    ehci_ctrl_done__tx_method_fn *ctrl_done;
    ehci_bulk_exe__tx_method_fn *bulk_exe;
    ehci_bulk_done__tx_method_fn *bulk_done;
    ehci_get_core_id__tx_method_fn *get_core_id;
    ehci_get_core_id_resp__tx_method_fn *get_core_id_resp;
};

/*
 * VTable struct definition for the interface (receive)
 */
struct ehci_rx_vtbl {
    ehci_map_dev_arr__rx_method_fn *map_dev_arr;
    ehci_map_dev_arr_done__rx_method_fn *map_dev_arr_done;
    ehci_dctrl_exe__rx_method_fn *dctrl_exe;
    ehci_dctrl_done__rx_method_fn *dctrl_done;
    ehci_ctrl_exe__rx_method_fn *ctrl_exe;
    ehci_ctrl_done__rx_method_fn *ctrl_done;
    ehci_bulk_exe__rx_method_fn *bulk_exe;
    ehci_bulk_done__rx_method_fn *bulk_done;
    ehci_get_core_id__rx_method_fn *get_core_id;
    ehci_get_core_id_resp__rx_method_fn *get_core_id_resp;
};

/*
 * Incoming connect callback type
 */
typedef  errval_t ehci_connect_fn(void *st, struct ehci_binding *binding);

/*
 * Export state struct
 */
struct ehci_export {
    struct idc_export common;
    ehci_connect_fn *connect_cb;
    struct waitset *waitset;
    void *st;
};

/*
 * Export function
 */
extern  errval_t ehci_export(void *st, idc_export_callback_fn *export_cb, ehci_connect_fn *connect_cb, struct waitset *ws, idc_export_flags_t flags);

/*
 * The Binding structure
 */
struct ehci_binding {
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
    ehci_can_send_fn *can_send;
    
    /* register an event for when a message is likely to be able to be sent */
    ehci_register_send_fn *register_send;
    
    /* change the waitset used by a binding */
    ehci_change_waitset_fn *change_waitset;
    
    /* perform control operations */
    ehci_control_fn *control;
    
    /* error handler for any async errors associated with this binding */
    /* must be filled-in by user */
    ehci_error_handler_fn *error_handler;
    
    /* Message send functions (filled in by binding) */
    struct ehci_tx_vtbl tx_vtbl;
    
    /* Incoming message handlers (filled in by user) */
    struct ehci_rx_vtbl rx_vtbl;
    
    /* Private state belonging to the binding implementation */
    union ehci_arg_union tx_union;
    union ehci_arg_union rx_union;
    struct waitset_chanstate register_chanstate;
    struct waitset_chanstate tx_cont_chanstate;
    enum ehci_msg_enum tx_msgnum;
    enum ehci_msg_enum rx_msgnum;
    int tx_msg_fragment;
    int rx_msg_fragment;
    size_t tx_str_pos;
    size_t rx_str_pos;
    size_t tx_str_len;
    size_t rx_str_len;
    struct event_queue_node event_qnode;
    ehci_bind_continuation_fn *bind_cont;
};

/*
 * Generic bind function
 */
extern  errval_t ehci_bind(iref_t i, ehci_bind_continuation_fn *_continuation, void *st, struct waitset *waitset, idc_bind_flags_t flags);

/*
 * Send wrappers
 */
static inline errval_t ehci_map_dev_arr__tx(struct ehci_binding *_binding, struct event_closure _continuation, struct capref ref, uint32_t sz) __attribute__ ((always_inline));
static inline errval_t ehci_map_dev_arr__tx(struct ehci_binding *_binding, struct event_closure _continuation, struct capref ref, uint32_t sz)
{
    return(((_binding->tx_vtbl).map_dev_arr)(_binding, _continuation, ref, sz));
}

static inline errval_t ehci_map_dev_arr_done__tx(struct ehci_binding *_binding, struct event_closure _continuation, uint8_t rsp) __attribute__ ((always_inline));
static inline errval_t ehci_map_dev_arr_done__tx(struct ehci_binding *_binding, struct event_closure _continuation, uint8_t rsp)
{
    return(((_binding->tx_vtbl).map_dev_arr_done)(_binding, _continuation, rsp));
}

static inline errval_t ehci_dctrl_exe__tx(struct ehci_binding *_binding, struct event_closure _continuation, ehci_udr_t usb_req, uint64_t buff_ptr, uint64_t sz, uint8_t device, uint8_t debug, uint32_t id) __attribute__ ((always_inline));
static inline errval_t ehci_dctrl_exe__tx(struct ehci_binding *_binding, struct event_closure _continuation, ehci_udr_t usb_req, uint64_t buff_ptr, uint64_t sz, uint8_t device, uint8_t debug, uint32_t id)
{
    return(((_binding->tx_vtbl).dctrl_exe)(_binding, _continuation, usb_req, buff_ptr, sz, device, debug, id));
}

static inline errval_t ehci_dctrl_done__tx(struct ehci_binding *_binding, struct event_closure _continuation, uint32_t id) __attribute__ ((always_inline));
static inline errval_t ehci_dctrl_done__tx(struct ehci_binding *_binding, struct event_closure _continuation, uint32_t id)
{
    return(((_binding->tx_vtbl).dctrl_done)(_binding, _continuation, id));
}

static inline errval_t ehci_ctrl_exe__tx(struct ehci_binding *_binding, struct event_closure _continuation, ehci_udr_t usb_req, uint8_t device, uint8_t debug, uint32_t id) __attribute__ ((always_inline));
static inline errval_t ehci_ctrl_exe__tx(struct ehci_binding *_binding, struct event_closure _continuation, ehci_udr_t usb_req, uint8_t device, uint8_t debug, uint32_t id)
{
    return(((_binding->tx_vtbl).ctrl_exe)(_binding, _continuation, usb_req, device, debug, id));
}

static inline errval_t ehci_ctrl_done__tx(struct ehci_binding *_binding, struct event_closure _continuation, uint32_t id) __attribute__ ((always_inline));
static inline errval_t ehci_ctrl_done__tx(struct ehci_binding *_binding, struct event_closure _continuation, uint32_t id)
{
    return(((_binding->tx_vtbl).ctrl_done)(_binding, _continuation, id));
}

static inline errval_t ehci_bulk_exe__tx(struct ehci_binding *_binding, struct event_closure _continuation, ehci_pipe_t p, uint64_t buff_ptr, uint32_t len, uint8_t debug, uint32_t id) __attribute__ ((always_inline));
static inline errval_t ehci_bulk_exe__tx(struct ehci_binding *_binding, struct event_closure _continuation, ehci_pipe_t p, uint64_t buff_ptr, uint32_t len, uint8_t debug, uint32_t id)
{
    return(((_binding->tx_vtbl).bulk_exe)(_binding, _continuation, p, buff_ptr, len, debug, id));
}

static inline errval_t ehci_bulk_done__tx(struct ehci_binding *_binding, struct event_closure _continuation, uint32_t id) __attribute__ ((always_inline));
static inline errval_t ehci_bulk_done__tx(struct ehci_binding *_binding, struct event_closure _continuation, uint32_t id)
{
    return(((_binding->tx_vtbl).bulk_done)(_binding, _continuation, id));
}

static inline errval_t ehci_get_core_id__tx(struct ehci_binding *_binding, struct event_closure _continuation) __attribute__ ((always_inline));
static inline errval_t ehci_get_core_id__tx(struct ehci_binding *_binding, struct event_closure _continuation)
{
    return(((_binding->tx_vtbl).get_core_id)(_binding, _continuation));
}

static inline errval_t ehci_get_core_id_resp__tx(struct ehci_binding *_binding, struct event_closure _continuation, uint64_t core) __attribute__ ((always_inline));
static inline errval_t ehci_get_core_id_resp__tx(struct ehci_binding *_binding, struct event_closure _continuation, uint64_t core)
{
    return(((_binding->tx_vtbl).get_core_id_resp)(_binding, _continuation, core));
}


/*
 * Backend-specific includes
 */
#ifdef CONFIG_FLOUNDER_BACKEND_LMP
#include <if/ehci_lmp_defs.h>
#endif // CONFIG_FLOUNDER_BACKEND_LMP
#ifdef CONFIG_FLOUNDER_BACKEND_UMP
#include <if/ehci_ump_defs.h>
#endif // CONFIG_FLOUNDER_BACKEND_UMP
#ifdef CONFIG_FLOUNDER_BACKEND_UMP_IPI
#include <if/ehci_ump_ipi_defs.h>
#endif // CONFIG_FLOUNDER_BACKEND_UMP_IPI
#ifdef CONFIG_FLOUNDER_BACKEND_MULTIHOP
#include <if/ehci_multihop_defs.h>
#endif // CONFIG_FLOUNDER_BACKEND_MULTIHOP
#ifdef CONFIG_FLOUNDER_BACKEND_UDN
#include <if/ehci_udn_defs.h>
#endif // CONFIG_FLOUNDER_BACKEND_UDN
/*
 * And we're done
 */
#endif // __ehci_IF_H
