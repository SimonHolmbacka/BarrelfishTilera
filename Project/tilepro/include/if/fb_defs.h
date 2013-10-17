#ifndef __fb_IF_H
#define __fb_IF_H 1
/*
 * Copyright (c) 2010, ETH Zurich.
 * All rights reserved.
 * 
 * INTERFACE NAME: fb
 * INTEFACE FILE: /home/andreas/barrelfish/new_test2/if/fb.if
 * INTERFACE DESCRIPTION: Framebuffer interface
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
typedef uint64_t fb_genpaddr_t;
typedef uint64_t fb_genvaddr_t;
typedef uint32_t fb_rsrcid_t;
typedef uint32_t fb_errval_t;
typedef uint32_t fb_cycles_t;
typedef uint32_t fb_iref_t;
typedef uint8_t fb_coreid_t;
typedef uint32_t fb_domainid_t;

/*
 * Forward declaration of binding type
 */
struct fb_binding;

/*
 * Contination (callback) and control function types
 */
typedef  void fb_bind_continuation_fn(void *st, errval_t err, struct fb_binding *_binding);
typedef  bool fb_can_send_fn(struct fb_binding *_binding);
typedef  errval_t fb_register_send_fn(struct fb_binding *_binding, struct waitset *ws, struct event_closure _continuation);
typedef  errval_t fb_change_waitset_fn(struct fb_binding *_binding, struct waitset *ws);
typedef  errval_t fb_control_fn(struct fb_binding *_binding, idc_control_t control);
typedef  void fb_error_handler_fn(struct fb_binding *_binding, errval_t err);

/*
 * Enumeration for message numbers
 */
typedef enum fb_msg_enum {
    fb___dummy__msgnum = 0,
    fb_get_framebuffer_call__msgnum = 1,
    fb_get_framebuffer_response__msgnum = 2,
    fb_set_videomode_call__msgnum = 3,
    fb_set_videomode_response__msgnum = 4,
    fb_get_vesamode_call__msgnum = 5,
    fb_get_vesamode_response__msgnum = 6,
    fb_set_vesamode_call__msgnum = 7,
    fb_set_vesamode_response__msgnum = 8,
    fb_save_vesastate_call__msgnum = 9,
    fb_save_vesastate_response__msgnum = 10,
    fb_restore_vesastate_call__msgnum = 11,
    fb_restore_vesastate_response__msgnum = 12,
    fb_vsync_call__msgnum = 13,
    fb_vsync_response__msgnum = 14
} fb_msg_enum;

/*
 * Message type signatures (transmit)
 */
typedef  errval_t fb_get_framebuffer_call__tx_method_fn(struct fb_binding *_binding, struct event_closure _continuation);
typedef  errval_t fb_get_framebuffer_response__tx_method_fn(struct fb_binding *_binding, struct event_closure _continuation, fb_errval_t err, struct capref fbcap, uint32_t offset);
typedef  errval_t fb_set_videomode_call__tx_method_fn(struct fb_binding *_binding, struct event_closure _continuation, uint16_t xres, uint16_t yres, uint8_t bpp);
typedef  errval_t fb_set_videomode_response__tx_method_fn(struct fb_binding *_binding, struct event_closure _continuation, fb_errval_t error);
typedef  errval_t fb_get_vesamode_call__tx_method_fn(struct fb_binding *_binding, struct event_closure _continuation);
typedef  errval_t fb_get_vesamode_response__tx_method_fn(struct fb_binding *_binding, struct event_closure _continuation, uint16_t mode, bool linear, fb_errval_t error);
typedef  errval_t fb_set_vesamode_call__tx_method_fn(struct fb_binding *_binding, struct event_closure _continuation, uint16_t mode, bool linear, bool clear);
typedef  errval_t fb_set_vesamode_response__tx_method_fn(struct fb_binding *_binding, struct event_closure _continuation, fb_errval_t error);
typedef  errval_t fb_save_vesastate_call__tx_method_fn(struct fb_binding *_binding, struct event_closure _continuation);
typedef  errval_t fb_save_vesastate_response__tx_method_fn(struct fb_binding *_binding, struct event_closure _continuation, fb_errval_t error);
typedef  errval_t fb_restore_vesastate_call__tx_method_fn(struct fb_binding *_binding, struct event_closure _continuation);
typedef  errval_t fb_restore_vesastate_response__tx_method_fn(struct fb_binding *_binding, struct event_closure _continuation, fb_errval_t error);
typedef  errval_t fb_vsync_call__tx_method_fn(struct fb_binding *_binding, struct event_closure _continuation);
typedef  errval_t fb_vsync_response__tx_method_fn(struct fb_binding *_binding, struct event_closure _continuation);

/*
 * Message type signatures (receive)
 */
typedef  void fb_get_framebuffer_call__rx_method_fn(struct fb_binding *_binding);
typedef  void fb_get_framebuffer_response__rx_method_fn(struct fb_binding *_binding, fb_errval_t err, struct capref fbcap, uint32_t offset);
typedef  void fb_set_videomode_call__rx_method_fn(struct fb_binding *_binding, uint16_t xres, uint16_t yres, uint8_t bpp);
typedef  void fb_set_videomode_response__rx_method_fn(struct fb_binding *_binding, fb_errval_t error);
typedef  void fb_get_vesamode_call__rx_method_fn(struct fb_binding *_binding);
typedef  void fb_get_vesamode_response__rx_method_fn(struct fb_binding *_binding, uint16_t mode, bool linear, fb_errval_t error);
typedef  void fb_set_vesamode_call__rx_method_fn(struct fb_binding *_binding, uint16_t mode, bool linear, bool clear);
typedef  void fb_set_vesamode_response__rx_method_fn(struct fb_binding *_binding, fb_errval_t error);
typedef  void fb_save_vesastate_call__rx_method_fn(struct fb_binding *_binding);
typedef  void fb_save_vesastate_response__rx_method_fn(struct fb_binding *_binding, fb_errval_t error);
typedef  void fb_restore_vesastate_call__rx_method_fn(struct fb_binding *_binding);
typedef  void fb_restore_vesastate_response__rx_method_fn(struct fb_binding *_binding, fb_errval_t error);
typedef  void fb_vsync_call__rx_method_fn(struct fb_binding *_binding);
typedef  void fb_vsync_response__rx_method_fn(struct fb_binding *_binding);

/*
 * Struct type for holding the args for each msg
 */
struct fb_get_framebuffer_response__args {
    fb_errval_t err;
    struct capref fbcap;
    uint32_t offset;
};
struct fb_set_videomode_call__args {
    uint16_t xres;
    uint16_t yres;
    uint8_t bpp;
};
struct fb_set_videomode_response__args {
    fb_errval_t error;
};
struct fb_get_vesamode_response__args {
    uint16_t mode;
    bool linear;
    fb_errval_t error;
};
struct fb_set_vesamode_call__args {
    uint16_t mode;
    bool linear;
    bool clear;
};
struct fb_set_vesamode_response__args {
    fb_errval_t error;
};
struct fb_save_vesastate_response__args {
    fb_errval_t error;
};
struct fb_restore_vesastate_response__args {
    fb_errval_t error;
};

/*
 * Union type for all message arguments
 */
union fb_arg_union {
    struct fb_get_framebuffer_response__args get_framebuffer_response;
    struct fb_set_videomode_call__args set_videomode_call;
    struct fb_set_videomode_response__args set_videomode_response;
    struct fb_get_vesamode_response__args get_vesamode_response;
    struct fb_set_vesamode_call__args set_vesamode_call;
    struct fb_set_vesamode_response__args set_vesamode_response;
    struct fb_save_vesastate_response__args save_vesastate_response;
    struct fb_restore_vesastate_response__args restore_vesastate_response;
};

/*
 * VTable struct definition for the interface (transmit)
 */
struct fb_tx_vtbl {
    fb_get_framebuffer_call__tx_method_fn *get_framebuffer_call;
    fb_get_framebuffer_response__tx_method_fn *get_framebuffer_response;
    fb_set_videomode_call__tx_method_fn *set_videomode_call;
    fb_set_videomode_response__tx_method_fn *set_videomode_response;
    fb_get_vesamode_call__tx_method_fn *get_vesamode_call;
    fb_get_vesamode_response__tx_method_fn *get_vesamode_response;
    fb_set_vesamode_call__tx_method_fn *set_vesamode_call;
    fb_set_vesamode_response__tx_method_fn *set_vesamode_response;
    fb_save_vesastate_call__tx_method_fn *save_vesastate_call;
    fb_save_vesastate_response__tx_method_fn *save_vesastate_response;
    fb_restore_vesastate_call__tx_method_fn *restore_vesastate_call;
    fb_restore_vesastate_response__tx_method_fn *restore_vesastate_response;
    fb_vsync_call__tx_method_fn *vsync_call;
    fb_vsync_response__tx_method_fn *vsync_response;
};

/*
 * VTable struct definition for the interface (receive)
 */
struct fb_rx_vtbl {
    fb_get_framebuffer_call__rx_method_fn *get_framebuffer_call;
    fb_get_framebuffer_response__rx_method_fn *get_framebuffer_response;
    fb_set_videomode_call__rx_method_fn *set_videomode_call;
    fb_set_videomode_response__rx_method_fn *set_videomode_response;
    fb_get_vesamode_call__rx_method_fn *get_vesamode_call;
    fb_get_vesamode_response__rx_method_fn *get_vesamode_response;
    fb_set_vesamode_call__rx_method_fn *set_vesamode_call;
    fb_set_vesamode_response__rx_method_fn *set_vesamode_response;
    fb_save_vesastate_call__rx_method_fn *save_vesastate_call;
    fb_save_vesastate_response__rx_method_fn *save_vesastate_response;
    fb_restore_vesastate_call__rx_method_fn *restore_vesastate_call;
    fb_restore_vesastate_response__rx_method_fn *restore_vesastate_response;
    fb_vsync_call__rx_method_fn *vsync_call;
    fb_vsync_response__rx_method_fn *vsync_response;
};

/*
 * Incoming connect callback type
 */
typedef  errval_t fb_connect_fn(void *st, struct fb_binding *binding);

/*
 * Export state struct
 */
struct fb_export {
    struct idc_export common;
    fb_connect_fn *connect_cb;
    struct waitset *waitset;
    void *st;
};

/*
 * Export function
 */
extern  errval_t fb_export(void *st, idc_export_callback_fn *export_cb, fb_connect_fn *connect_cb, struct waitset *ws, idc_export_flags_t flags);

/*
 * The Binding structure
 */
struct fb_binding {
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
    fb_can_send_fn *can_send;
    
    /* register an event for when a message is likely to be able to be sent */
    fb_register_send_fn *register_send;
    
    /* change the waitset used by a binding */
    fb_change_waitset_fn *change_waitset;
    
    /* perform control operations */
    fb_control_fn *control;
    
    /* error handler for any async errors associated with this binding */
    /* must be filled-in by user */
    fb_error_handler_fn *error_handler;
    
    /* Message send functions (filled in by binding) */
    struct fb_tx_vtbl tx_vtbl;
    
    /* Incoming message handlers (filled in by user) */
    struct fb_rx_vtbl rx_vtbl;
    
    /* Private state belonging to the binding implementation */
    union fb_arg_union tx_union;
    union fb_arg_union rx_union;
    struct waitset_chanstate register_chanstate;
    struct waitset_chanstate tx_cont_chanstate;
    enum fb_msg_enum tx_msgnum;
    enum fb_msg_enum rx_msgnum;
    int tx_msg_fragment;
    int rx_msg_fragment;
    size_t tx_str_pos;
    size_t rx_str_pos;
    size_t tx_str_len;
    size_t rx_str_len;
    struct event_queue_node event_qnode;
    fb_bind_continuation_fn *bind_cont;
};

/*
 * Generic bind function
 */
extern  errval_t fb_bind(iref_t i, fb_bind_continuation_fn *_continuation, void *st, struct waitset *waitset, idc_bind_flags_t flags);

/*
 * Send wrappers
 */
static inline errval_t fb_get_framebuffer_call__tx(struct fb_binding *_binding, struct event_closure _continuation) __attribute__ ((always_inline));
static inline errval_t fb_get_framebuffer_call__tx(struct fb_binding *_binding, struct event_closure _continuation)
{
    return(((_binding->tx_vtbl).get_framebuffer_call)(_binding, _continuation));
}

static inline errval_t fb_get_framebuffer_response__tx(struct fb_binding *_binding, struct event_closure _continuation, fb_errval_t err, struct capref fbcap, uint32_t offset) __attribute__ ((always_inline));
static inline errval_t fb_get_framebuffer_response__tx(struct fb_binding *_binding, struct event_closure _continuation, fb_errval_t err, struct capref fbcap, uint32_t offset)
{
    return(((_binding->tx_vtbl).get_framebuffer_response)(_binding, _continuation, err, fbcap, offset));
}

static inline errval_t fb_set_videomode_call__tx(struct fb_binding *_binding, struct event_closure _continuation, uint16_t xres, uint16_t yres, uint8_t bpp) __attribute__ ((always_inline));
static inline errval_t fb_set_videomode_call__tx(struct fb_binding *_binding, struct event_closure _continuation, uint16_t xres, uint16_t yres, uint8_t bpp)
{
    return(((_binding->tx_vtbl).set_videomode_call)(_binding, _continuation, xres, yres, bpp));
}

static inline errval_t fb_set_videomode_response__tx(struct fb_binding *_binding, struct event_closure _continuation, fb_errval_t error) __attribute__ ((always_inline));
static inline errval_t fb_set_videomode_response__tx(struct fb_binding *_binding, struct event_closure _continuation, fb_errval_t error)
{
    return(((_binding->tx_vtbl).set_videomode_response)(_binding, _continuation, error));
}

static inline errval_t fb_get_vesamode_call__tx(struct fb_binding *_binding, struct event_closure _continuation) __attribute__ ((always_inline));
static inline errval_t fb_get_vesamode_call__tx(struct fb_binding *_binding, struct event_closure _continuation)
{
    return(((_binding->tx_vtbl).get_vesamode_call)(_binding, _continuation));
}

static inline errval_t fb_get_vesamode_response__tx(struct fb_binding *_binding, struct event_closure _continuation, uint16_t mode, bool linear, fb_errval_t error) __attribute__ ((always_inline));
static inline errval_t fb_get_vesamode_response__tx(struct fb_binding *_binding, struct event_closure _continuation, uint16_t mode, bool linear, fb_errval_t error)
{
    return(((_binding->tx_vtbl).get_vesamode_response)(_binding, _continuation, mode, linear, error));
}

static inline errval_t fb_set_vesamode_call__tx(struct fb_binding *_binding, struct event_closure _continuation, uint16_t mode, bool linear, bool clear) __attribute__ ((always_inline));
static inline errval_t fb_set_vesamode_call__tx(struct fb_binding *_binding, struct event_closure _continuation, uint16_t mode, bool linear, bool clear)
{
    return(((_binding->tx_vtbl).set_vesamode_call)(_binding, _continuation, mode, linear, clear));
}

static inline errval_t fb_set_vesamode_response__tx(struct fb_binding *_binding, struct event_closure _continuation, fb_errval_t error) __attribute__ ((always_inline));
static inline errval_t fb_set_vesamode_response__tx(struct fb_binding *_binding, struct event_closure _continuation, fb_errval_t error)
{
    return(((_binding->tx_vtbl).set_vesamode_response)(_binding, _continuation, error));
}

static inline errval_t fb_save_vesastate_call__tx(struct fb_binding *_binding, struct event_closure _continuation) __attribute__ ((always_inline));
static inline errval_t fb_save_vesastate_call__tx(struct fb_binding *_binding, struct event_closure _continuation)
{
    return(((_binding->tx_vtbl).save_vesastate_call)(_binding, _continuation));
}

static inline errval_t fb_save_vesastate_response__tx(struct fb_binding *_binding, struct event_closure _continuation, fb_errval_t error) __attribute__ ((always_inline));
static inline errval_t fb_save_vesastate_response__tx(struct fb_binding *_binding, struct event_closure _continuation, fb_errval_t error)
{
    return(((_binding->tx_vtbl).save_vesastate_response)(_binding, _continuation, error));
}

static inline errval_t fb_restore_vesastate_call__tx(struct fb_binding *_binding, struct event_closure _continuation) __attribute__ ((always_inline));
static inline errval_t fb_restore_vesastate_call__tx(struct fb_binding *_binding, struct event_closure _continuation)
{
    return(((_binding->tx_vtbl).restore_vesastate_call)(_binding, _continuation));
}

static inline errval_t fb_restore_vesastate_response__tx(struct fb_binding *_binding, struct event_closure _continuation, fb_errval_t error) __attribute__ ((always_inline));
static inline errval_t fb_restore_vesastate_response__tx(struct fb_binding *_binding, struct event_closure _continuation, fb_errval_t error)
{
    return(((_binding->tx_vtbl).restore_vesastate_response)(_binding, _continuation, error));
}

static inline errval_t fb_vsync_call__tx(struct fb_binding *_binding, struct event_closure _continuation) __attribute__ ((always_inline));
static inline errval_t fb_vsync_call__tx(struct fb_binding *_binding, struct event_closure _continuation)
{
    return(((_binding->tx_vtbl).vsync_call)(_binding, _continuation));
}

static inline errval_t fb_vsync_response__tx(struct fb_binding *_binding, struct event_closure _continuation) __attribute__ ((always_inline));
static inline errval_t fb_vsync_response__tx(struct fb_binding *_binding, struct event_closure _continuation)
{
    return(((_binding->tx_vtbl).vsync_response)(_binding, _continuation));
}


/*
 * Backend-specific includes
 */
#ifdef CONFIG_FLOUNDER_BACKEND_LMP
#include <if/fb_lmp_defs.h>
#endif // CONFIG_FLOUNDER_BACKEND_LMP
#ifdef CONFIG_FLOUNDER_BACKEND_UMP
#include <if/fb_ump_defs.h>
#endif // CONFIG_FLOUNDER_BACKEND_UMP
#ifdef CONFIG_FLOUNDER_BACKEND_UMP_IPI
#include <if/fb_ump_ipi_defs.h>
#endif // CONFIG_FLOUNDER_BACKEND_UMP_IPI
#ifdef CONFIG_FLOUNDER_BACKEND_MULTIHOP
#include <if/fb_multihop_defs.h>
#endif // CONFIG_FLOUNDER_BACKEND_MULTIHOP
#ifdef CONFIG_FLOUNDER_BACKEND_UDN
#include <if/fb_udn_defs.h>
#endif // CONFIG_FLOUNDER_BACKEND_UDN
/*
 * And we're done
 */
#endif // __fb_IF_H