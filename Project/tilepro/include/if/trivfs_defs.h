#ifndef __trivfs_IF_H
#define __trivfs_IF_H 1
/*
 * Copyright (c) 2010, ETH Zurich.
 * All rights reserved.
 * 
 * INTERFACE NAME: trivfs
 * INTEFACE FILE: /home/andreas/barrelfish/new_test2/if/trivfs.if
 * INTERFACE DESCRIPTION: Trivial file system interface
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
typedef uint64_t trivfs_genpaddr_t;
typedef uint64_t trivfs_genvaddr_t;
typedef uint32_t trivfs_rsrcid_t;
typedef uint32_t trivfs_errval_t;
typedef uint32_t trivfs_cycles_t;
typedef uint32_t trivfs_iref_t;
typedef uint8_t trivfs_coreid_t;
typedef uint32_t trivfs_domainid_t;
typedef uint32_t trivfs_fh_t;
typedef uint32_t trivfs_fsize_t;
typedef uint32_t trivfs_offset_t;
typedef uint32_t trivfs_bulkid_t;

/*
 * Forward declaration of binding type
 */
struct trivfs_binding;

/*
 * Contination (callback) and control function types
 */
typedef  void trivfs_bind_continuation_fn(void *st, errval_t err, struct trivfs_binding *_binding);
typedef  bool trivfs_can_send_fn(struct trivfs_binding *_binding);
typedef  errval_t trivfs_register_send_fn(struct trivfs_binding *_binding, struct waitset *ws, struct event_closure _continuation);
typedef  errval_t trivfs_change_waitset_fn(struct trivfs_binding *_binding, struct waitset *ws);
typedef  errval_t trivfs_control_fn(struct trivfs_binding *_binding, idc_control_t control);
typedef  void trivfs_error_handler_fn(struct trivfs_binding *_binding, errval_t err);

/*
 * Enumeration for message numbers
 */
typedef enum trivfs_msg_enum {
    trivfs___dummy__msgnum = 0,
    trivfs_bulk_init_call__msgnum = 1,
    trivfs_bulk_init_response__msgnum = 2,
    trivfs_getroot_call__msgnum = 3,
    trivfs_getroot_response__msgnum = 4,
    trivfs_readdir_call__msgnum = 5,
    trivfs_readdir_response__msgnum = 6,
    trivfs_lookup_call__msgnum = 7,
    trivfs_lookup_response__msgnum = 8,
    trivfs_getattr_call__msgnum = 9,
    trivfs_getattr_response__msgnum = 10,
    trivfs_read_call__msgnum = 11,
    trivfs_read_response__msgnum = 12,
    trivfs_write_call__msgnum = 13,
    trivfs_write_response__msgnum = 14,
    trivfs_read_bulk_call__msgnum = 15,
    trivfs_read_bulk_response__msgnum = 16,
    trivfs_write_bulk_call__msgnum = 17,
    trivfs_write_bulk_response__msgnum = 18,
    trivfs_truncate_call__msgnum = 19,
    trivfs_truncate_response__msgnum = 20,
    trivfs_create_call__msgnum = 21,
    trivfs_create_response__msgnum = 22,
    trivfs_mkdir_call__msgnum = 23,
    trivfs_mkdir_response__msgnum = 24,
    trivfs_delete_call__msgnum = 25,
    trivfs_delete_response__msgnum = 26
} trivfs_msg_enum;

/*
 * Message type signatures (transmit)
 */
typedef  errval_t trivfs_bulk_init_call__tx_method_fn(struct trivfs_binding *_binding, struct event_closure _continuation, struct capref frame);
typedef  errval_t trivfs_bulk_init_response__tx_method_fn(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_errval_t err);
typedef  errval_t trivfs_getroot_call__tx_method_fn(struct trivfs_binding *_binding, struct event_closure _continuation);
typedef  errval_t trivfs_getroot_response__tx_method_fn(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_fh_t rootfh);
typedef  errval_t trivfs_readdir_call__tx_method_fn(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_fh_t dir, uint32_t idx);
typedef  errval_t trivfs_readdir_response__tx_method_fn(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_errval_t err, const char *name, bool isdir, trivfs_fsize_t size);
typedef  errval_t trivfs_lookup_call__tx_method_fn(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_fh_t dir, const char *name);
typedef  errval_t trivfs_lookup_response__tx_method_fn(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_errval_t err, trivfs_fh_t fh, bool isdir);
typedef  errval_t trivfs_getattr_call__tx_method_fn(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_fh_t fh);
typedef  errval_t trivfs_getattr_response__tx_method_fn(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_errval_t err, bool isdir, trivfs_fsize_t size);
typedef  errval_t trivfs_read_call__tx_method_fn(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_fh_t file, trivfs_offset_t offset, trivfs_fsize_t maxlen);
typedef  errval_t trivfs_read_response__tx_method_fn(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_errval_t err, const uint8_t *data, size_t retlen);
typedef  errval_t trivfs_write_call__tx_method_fn(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_fh_t file, trivfs_offset_t offset, const uint8_t *data, size_t len);
typedef  errval_t trivfs_write_response__tx_method_fn(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_errval_t err);
typedef  errval_t trivfs_read_bulk_call__tx_method_fn(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_fh_t file, trivfs_offset_t offset, trivfs_fsize_t maxlen, trivfs_bulkid_t bulkid);
typedef  errval_t trivfs_read_bulk_response__tx_method_fn(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_errval_t err, trivfs_fsize_t retlen);
typedef  errval_t trivfs_write_bulk_call__tx_method_fn(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_fh_t file, trivfs_offset_t offset, trivfs_fsize_t len, trivfs_bulkid_t bulkid);
typedef  errval_t trivfs_write_bulk_response__tx_method_fn(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_errval_t err);
typedef  errval_t trivfs_truncate_call__tx_method_fn(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_fh_t file, trivfs_fsize_t newsize);
typedef  errval_t trivfs_truncate_response__tx_method_fn(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_errval_t err);
typedef  errval_t trivfs_create_call__tx_method_fn(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_fh_t dir, const char *name);
typedef  errval_t trivfs_create_response__tx_method_fn(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_errval_t err, trivfs_fh_t fh);
typedef  errval_t trivfs_mkdir_call__tx_method_fn(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_fh_t dir, const char *name);
typedef  errval_t trivfs_mkdir_response__tx_method_fn(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_errval_t err, trivfs_fh_t fh);
typedef  errval_t trivfs_delete_call__tx_method_fn(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_fh_t fh);
typedef  errval_t trivfs_delete_response__tx_method_fn(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_errval_t err);

/*
 * Message type signatures (receive)
 */
typedef  void trivfs_bulk_init_call__rx_method_fn(struct trivfs_binding *_binding, struct capref frame);
typedef  void trivfs_bulk_init_response__rx_method_fn(struct trivfs_binding *_binding, trivfs_errval_t err);
typedef  void trivfs_getroot_call__rx_method_fn(struct trivfs_binding *_binding);
typedef  void trivfs_getroot_response__rx_method_fn(struct trivfs_binding *_binding, trivfs_fh_t rootfh);
typedef  void trivfs_readdir_call__rx_method_fn(struct trivfs_binding *_binding, trivfs_fh_t dir, uint32_t idx);
typedef  void trivfs_readdir_response__rx_method_fn(struct trivfs_binding *_binding, trivfs_errval_t err, char *name, bool isdir, trivfs_fsize_t size);
typedef  void trivfs_lookup_call__rx_method_fn(struct trivfs_binding *_binding, trivfs_fh_t dir, char *name);
typedef  void trivfs_lookup_response__rx_method_fn(struct trivfs_binding *_binding, trivfs_errval_t err, trivfs_fh_t fh, bool isdir);
typedef  void trivfs_getattr_call__rx_method_fn(struct trivfs_binding *_binding, trivfs_fh_t fh);
typedef  void trivfs_getattr_response__rx_method_fn(struct trivfs_binding *_binding, trivfs_errval_t err, bool isdir, trivfs_fsize_t size);
typedef  void trivfs_read_call__rx_method_fn(struct trivfs_binding *_binding, trivfs_fh_t file, trivfs_offset_t offset, trivfs_fsize_t maxlen);
typedef  void trivfs_read_response__rx_method_fn(struct trivfs_binding *_binding, trivfs_errval_t err, uint8_t *data, size_t retlen);
typedef  void trivfs_write_call__rx_method_fn(struct trivfs_binding *_binding, trivfs_fh_t file, trivfs_offset_t offset, uint8_t *data, size_t len);
typedef  void trivfs_write_response__rx_method_fn(struct trivfs_binding *_binding, trivfs_errval_t err);
typedef  void trivfs_read_bulk_call__rx_method_fn(struct trivfs_binding *_binding, trivfs_fh_t file, trivfs_offset_t offset, trivfs_fsize_t maxlen, trivfs_bulkid_t bulkid);
typedef  void trivfs_read_bulk_response__rx_method_fn(struct trivfs_binding *_binding, trivfs_errval_t err, trivfs_fsize_t retlen);
typedef  void trivfs_write_bulk_call__rx_method_fn(struct trivfs_binding *_binding, trivfs_fh_t file, trivfs_offset_t offset, trivfs_fsize_t len, trivfs_bulkid_t bulkid);
typedef  void trivfs_write_bulk_response__rx_method_fn(struct trivfs_binding *_binding, trivfs_errval_t err);
typedef  void trivfs_truncate_call__rx_method_fn(struct trivfs_binding *_binding, trivfs_fh_t file, trivfs_fsize_t newsize);
typedef  void trivfs_truncate_response__rx_method_fn(struct trivfs_binding *_binding, trivfs_errval_t err);
typedef  void trivfs_create_call__rx_method_fn(struct trivfs_binding *_binding, trivfs_fh_t dir, char *name);
typedef  void trivfs_create_response__rx_method_fn(struct trivfs_binding *_binding, trivfs_errval_t err, trivfs_fh_t fh);
typedef  void trivfs_mkdir_call__rx_method_fn(struct trivfs_binding *_binding, trivfs_fh_t dir, char *name);
typedef  void trivfs_mkdir_response__rx_method_fn(struct trivfs_binding *_binding, trivfs_errval_t err, trivfs_fh_t fh);
typedef  void trivfs_delete_call__rx_method_fn(struct trivfs_binding *_binding, trivfs_fh_t fh);
typedef  void trivfs_delete_response__rx_method_fn(struct trivfs_binding *_binding, trivfs_errval_t err);

/*
 * Struct type for holding the args for each msg
 */
struct trivfs_bulk_init_call__args {
    struct capref frame;
};
struct trivfs_bulk_init_response__args {
    trivfs_errval_t err;
};
struct trivfs_getroot_response__args {
    trivfs_fh_t rootfh;
};
struct trivfs_readdir_call__args {
    trivfs_fh_t dir;
    uint32_t idx;
};
struct trivfs_readdir_response__args {
    trivfs_errval_t err;
    char *name;
    bool isdir;
    trivfs_fsize_t size;
};
struct trivfs_lookup_call__args {
    trivfs_fh_t dir;
    char *name;
};
struct trivfs_lookup_response__args {
    trivfs_errval_t err;
    trivfs_fh_t fh;
    bool isdir;
};
struct trivfs_getattr_call__args {
    trivfs_fh_t fh;
};
struct trivfs_getattr_response__args {
    trivfs_errval_t err;
    bool isdir;
    trivfs_fsize_t size;
};
struct trivfs_read_call__args {
    trivfs_fh_t file;
    trivfs_offset_t offset;
    trivfs_fsize_t maxlen;
};
struct trivfs_read_response__args {
    trivfs_errval_t err;
    uint8_t *data;
    size_t retlen;
};
struct trivfs_write_call__args {
    trivfs_fh_t file;
    trivfs_offset_t offset;
    uint8_t *data;
    size_t len;
};
struct trivfs_write_response__args {
    trivfs_errval_t err;
};
struct trivfs_read_bulk_call__args {
    trivfs_fh_t file;
    trivfs_offset_t offset;
    trivfs_fsize_t maxlen;
    trivfs_bulkid_t bulkid;
};
struct trivfs_read_bulk_response__args {
    trivfs_errval_t err;
    trivfs_fsize_t retlen;
};
struct trivfs_write_bulk_call__args {
    trivfs_fh_t file;
    trivfs_offset_t offset;
    trivfs_fsize_t len;
    trivfs_bulkid_t bulkid;
};
struct trivfs_write_bulk_response__args {
    trivfs_errval_t err;
};
struct trivfs_truncate_call__args {
    trivfs_fh_t file;
    trivfs_fsize_t newsize;
};
struct trivfs_truncate_response__args {
    trivfs_errval_t err;
};
struct trivfs_create_call__args {
    trivfs_fh_t dir;
    char *name;
};
struct trivfs_create_response__args {
    trivfs_errval_t err;
    trivfs_fh_t fh;
};
struct trivfs_mkdir_call__args {
    trivfs_fh_t dir;
    char *name;
};
struct trivfs_mkdir_response__args {
    trivfs_errval_t err;
    trivfs_fh_t fh;
};
struct trivfs_delete_call__args {
    trivfs_fh_t fh;
};
struct trivfs_delete_response__args {
    trivfs_errval_t err;
};

/*
 * Union type for all message arguments
 */
union trivfs_arg_union {
    struct trivfs_bulk_init_call__args bulk_init_call;
    struct trivfs_bulk_init_response__args bulk_init_response;
    struct trivfs_getroot_response__args getroot_response;
    struct trivfs_readdir_call__args readdir_call;
    struct trivfs_readdir_response__args readdir_response;
    struct trivfs_lookup_call__args lookup_call;
    struct trivfs_lookup_response__args lookup_response;
    struct trivfs_getattr_call__args getattr_call;
    struct trivfs_getattr_response__args getattr_response;
    struct trivfs_read_call__args read_call;
    struct trivfs_read_response__args read_response;
    struct trivfs_write_call__args write_call;
    struct trivfs_write_response__args write_response;
    struct trivfs_read_bulk_call__args read_bulk_call;
    struct trivfs_read_bulk_response__args read_bulk_response;
    struct trivfs_write_bulk_call__args write_bulk_call;
    struct trivfs_write_bulk_response__args write_bulk_response;
    struct trivfs_truncate_call__args truncate_call;
    struct trivfs_truncate_response__args truncate_response;
    struct trivfs_create_call__args create_call;
    struct trivfs_create_response__args create_response;
    struct trivfs_mkdir_call__args mkdir_call;
    struct trivfs_mkdir_response__args mkdir_response;
    struct trivfs_delete_call__args delete_call;
    struct trivfs_delete_response__args delete_response;
};

/*
 * VTable struct definition for the interface (transmit)
 */
struct trivfs_tx_vtbl {
    trivfs_bulk_init_call__tx_method_fn *bulk_init_call;
    trivfs_bulk_init_response__tx_method_fn *bulk_init_response;
    trivfs_getroot_call__tx_method_fn *getroot_call;
    trivfs_getroot_response__tx_method_fn *getroot_response;
    trivfs_readdir_call__tx_method_fn *readdir_call;
    trivfs_readdir_response__tx_method_fn *readdir_response;
    trivfs_lookup_call__tx_method_fn *lookup_call;
    trivfs_lookup_response__tx_method_fn *lookup_response;
    trivfs_getattr_call__tx_method_fn *getattr_call;
    trivfs_getattr_response__tx_method_fn *getattr_response;
    trivfs_read_call__tx_method_fn *read_call;
    trivfs_read_response__tx_method_fn *read_response;
    trivfs_write_call__tx_method_fn *write_call;
    trivfs_write_response__tx_method_fn *write_response;
    trivfs_read_bulk_call__tx_method_fn *read_bulk_call;
    trivfs_read_bulk_response__tx_method_fn *read_bulk_response;
    trivfs_write_bulk_call__tx_method_fn *write_bulk_call;
    trivfs_write_bulk_response__tx_method_fn *write_bulk_response;
    trivfs_truncate_call__tx_method_fn *truncate_call;
    trivfs_truncate_response__tx_method_fn *truncate_response;
    trivfs_create_call__tx_method_fn *create_call;
    trivfs_create_response__tx_method_fn *create_response;
    trivfs_mkdir_call__tx_method_fn *mkdir_call;
    trivfs_mkdir_response__tx_method_fn *mkdir_response;
    trivfs_delete_call__tx_method_fn *delete_call;
    trivfs_delete_response__tx_method_fn *delete_response;
};

/*
 * VTable struct definition for the interface (receive)
 */
struct trivfs_rx_vtbl {
    trivfs_bulk_init_call__rx_method_fn *bulk_init_call;
    trivfs_bulk_init_response__rx_method_fn *bulk_init_response;
    trivfs_getroot_call__rx_method_fn *getroot_call;
    trivfs_getroot_response__rx_method_fn *getroot_response;
    trivfs_readdir_call__rx_method_fn *readdir_call;
    trivfs_readdir_response__rx_method_fn *readdir_response;
    trivfs_lookup_call__rx_method_fn *lookup_call;
    trivfs_lookup_response__rx_method_fn *lookup_response;
    trivfs_getattr_call__rx_method_fn *getattr_call;
    trivfs_getattr_response__rx_method_fn *getattr_response;
    trivfs_read_call__rx_method_fn *read_call;
    trivfs_read_response__rx_method_fn *read_response;
    trivfs_write_call__rx_method_fn *write_call;
    trivfs_write_response__rx_method_fn *write_response;
    trivfs_read_bulk_call__rx_method_fn *read_bulk_call;
    trivfs_read_bulk_response__rx_method_fn *read_bulk_response;
    trivfs_write_bulk_call__rx_method_fn *write_bulk_call;
    trivfs_write_bulk_response__rx_method_fn *write_bulk_response;
    trivfs_truncate_call__rx_method_fn *truncate_call;
    trivfs_truncate_response__rx_method_fn *truncate_response;
    trivfs_create_call__rx_method_fn *create_call;
    trivfs_create_response__rx_method_fn *create_response;
    trivfs_mkdir_call__rx_method_fn *mkdir_call;
    trivfs_mkdir_response__rx_method_fn *mkdir_response;
    trivfs_delete_call__rx_method_fn *delete_call;
    trivfs_delete_response__rx_method_fn *delete_response;
};

/*
 * Incoming connect callback type
 */
typedef  errval_t trivfs_connect_fn(void *st, struct trivfs_binding *binding);

/*
 * Export state struct
 */
struct trivfs_export {
    struct idc_export common;
    trivfs_connect_fn *connect_cb;
    struct waitset *waitset;
    void *st;
};

/*
 * Export function
 */
extern  errval_t trivfs_export(void *st, idc_export_callback_fn *export_cb, trivfs_connect_fn *connect_cb, struct waitset *ws, idc_export_flags_t flags);

/*
 * The Binding structure
 */
struct trivfs_binding {
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
    trivfs_can_send_fn *can_send;
    
    /* register an event for when a message is likely to be able to be sent */
    trivfs_register_send_fn *register_send;
    
    /* change the waitset used by a binding */
    trivfs_change_waitset_fn *change_waitset;
    
    /* perform control operations */
    trivfs_control_fn *control;
    
    /* error handler for any async errors associated with this binding */
    /* must be filled-in by user */
    trivfs_error_handler_fn *error_handler;
    
    /* Message send functions (filled in by binding) */
    struct trivfs_tx_vtbl tx_vtbl;
    
    /* Incoming message handlers (filled in by user) */
    struct trivfs_rx_vtbl rx_vtbl;
    
    /* Private state belonging to the binding implementation */
    union trivfs_arg_union tx_union;
    union trivfs_arg_union rx_union;
    struct waitset_chanstate register_chanstate;
    struct waitset_chanstate tx_cont_chanstate;
    enum trivfs_msg_enum tx_msgnum;
    enum trivfs_msg_enum rx_msgnum;
    int tx_msg_fragment;
    int rx_msg_fragment;
    size_t tx_str_pos;
    size_t rx_str_pos;
    size_t tx_str_len;
    size_t rx_str_len;
    struct event_queue_node event_qnode;
    trivfs_bind_continuation_fn *bind_cont;
};

/*
 * Generic bind function
 */
extern  errval_t trivfs_bind(iref_t i, trivfs_bind_continuation_fn *_continuation, void *st, struct waitset *waitset, idc_bind_flags_t flags);

/*
 * Send wrappers
 */
static inline errval_t trivfs_bulk_init_call__tx(struct trivfs_binding *_binding, struct event_closure _continuation, struct capref frame) __attribute__ ((always_inline));
static inline errval_t trivfs_bulk_init_call__tx(struct trivfs_binding *_binding, struct event_closure _continuation, struct capref frame)
{
    return(((_binding->tx_vtbl).bulk_init_call)(_binding, _continuation, frame));
}

static inline errval_t trivfs_bulk_init_response__tx(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_errval_t err) __attribute__ ((always_inline));
static inline errval_t trivfs_bulk_init_response__tx(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_errval_t err)
{
    return(((_binding->tx_vtbl).bulk_init_response)(_binding, _continuation, err));
}

static inline errval_t trivfs_getroot_call__tx(struct trivfs_binding *_binding, struct event_closure _continuation) __attribute__ ((always_inline));
static inline errval_t trivfs_getroot_call__tx(struct trivfs_binding *_binding, struct event_closure _continuation)
{
    return(((_binding->tx_vtbl).getroot_call)(_binding, _continuation));
}

static inline errval_t trivfs_getroot_response__tx(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_fh_t rootfh) __attribute__ ((always_inline));
static inline errval_t trivfs_getroot_response__tx(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_fh_t rootfh)
{
    return(((_binding->tx_vtbl).getroot_response)(_binding, _continuation, rootfh));
}

static inline errval_t trivfs_readdir_call__tx(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_fh_t dir, uint32_t idx) __attribute__ ((always_inline));
static inline errval_t trivfs_readdir_call__tx(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_fh_t dir, uint32_t idx)
{
    return(((_binding->tx_vtbl).readdir_call)(_binding, _continuation, dir, idx));
}

static inline errval_t trivfs_readdir_response__tx(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_errval_t err, const char *name, bool isdir, trivfs_fsize_t size) __attribute__ ((always_inline));
static inline errval_t trivfs_readdir_response__tx(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_errval_t err, const char *name, bool isdir, trivfs_fsize_t size)
{
    return(((_binding->tx_vtbl).readdir_response)(_binding, _continuation, err, name, isdir, size));
}

static inline errval_t trivfs_lookup_call__tx(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_fh_t dir, const char *name) __attribute__ ((always_inline));
static inline errval_t trivfs_lookup_call__tx(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_fh_t dir, const char *name)
{
    return(((_binding->tx_vtbl).lookup_call)(_binding, _continuation, dir, name));
}

static inline errval_t trivfs_lookup_response__tx(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_errval_t err, trivfs_fh_t fh, bool isdir) __attribute__ ((always_inline));
static inline errval_t trivfs_lookup_response__tx(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_errval_t err, trivfs_fh_t fh, bool isdir)
{
    return(((_binding->tx_vtbl).lookup_response)(_binding, _continuation, err, fh, isdir));
}

static inline errval_t trivfs_getattr_call__tx(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_fh_t fh) __attribute__ ((always_inline));
static inline errval_t trivfs_getattr_call__tx(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_fh_t fh)
{
    return(((_binding->tx_vtbl).getattr_call)(_binding, _continuation, fh));
}

static inline errval_t trivfs_getattr_response__tx(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_errval_t err, bool isdir, trivfs_fsize_t size) __attribute__ ((always_inline));
static inline errval_t trivfs_getattr_response__tx(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_errval_t err, bool isdir, trivfs_fsize_t size)
{
    return(((_binding->tx_vtbl).getattr_response)(_binding, _continuation, err, isdir, size));
}

static inline errval_t trivfs_read_call__tx(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_fh_t file, trivfs_offset_t offset, trivfs_fsize_t maxlen) __attribute__ ((always_inline));
static inline errval_t trivfs_read_call__tx(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_fh_t file, trivfs_offset_t offset, trivfs_fsize_t maxlen)
{
    return(((_binding->tx_vtbl).read_call)(_binding, _continuation, file, offset, maxlen));
}

static inline errval_t trivfs_read_response__tx(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_errval_t err, const uint8_t *data, size_t retlen) __attribute__ ((always_inline));
static inline errval_t trivfs_read_response__tx(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_errval_t err, const uint8_t *data, size_t retlen)
{
    return(((_binding->tx_vtbl).read_response)(_binding, _continuation, err, data, retlen));
}

static inline errval_t trivfs_write_call__tx(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_fh_t file, trivfs_offset_t offset, const uint8_t *data, size_t len) __attribute__ ((always_inline));
static inline errval_t trivfs_write_call__tx(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_fh_t file, trivfs_offset_t offset, const uint8_t *data, size_t len)
{
    return(((_binding->tx_vtbl).write_call)(_binding, _continuation, file, offset, data, len));
}

static inline errval_t trivfs_write_response__tx(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_errval_t err) __attribute__ ((always_inline));
static inline errval_t trivfs_write_response__tx(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_errval_t err)
{
    return(((_binding->tx_vtbl).write_response)(_binding, _continuation, err));
}

static inline errval_t trivfs_read_bulk_call__tx(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_fh_t file, trivfs_offset_t offset, trivfs_fsize_t maxlen, trivfs_bulkid_t bulkid) __attribute__ ((always_inline));
static inline errval_t trivfs_read_bulk_call__tx(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_fh_t file, trivfs_offset_t offset, trivfs_fsize_t maxlen, trivfs_bulkid_t bulkid)
{
    return(((_binding->tx_vtbl).read_bulk_call)(_binding, _continuation, file, offset, maxlen, bulkid));
}

static inline errval_t trivfs_read_bulk_response__tx(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_errval_t err, trivfs_fsize_t retlen) __attribute__ ((always_inline));
static inline errval_t trivfs_read_bulk_response__tx(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_errval_t err, trivfs_fsize_t retlen)
{
    return(((_binding->tx_vtbl).read_bulk_response)(_binding, _continuation, err, retlen));
}

static inline errval_t trivfs_write_bulk_call__tx(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_fh_t file, trivfs_offset_t offset, trivfs_fsize_t len, trivfs_bulkid_t bulkid) __attribute__ ((always_inline));
static inline errval_t trivfs_write_bulk_call__tx(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_fh_t file, trivfs_offset_t offset, trivfs_fsize_t len, trivfs_bulkid_t bulkid)
{
    return(((_binding->tx_vtbl).write_bulk_call)(_binding, _continuation, file, offset, len, bulkid));
}

static inline errval_t trivfs_write_bulk_response__tx(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_errval_t err) __attribute__ ((always_inline));
static inline errval_t trivfs_write_bulk_response__tx(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_errval_t err)
{
    return(((_binding->tx_vtbl).write_bulk_response)(_binding, _continuation, err));
}

static inline errval_t trivfs_truncate_call__tx(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_fh_t file, trivfs_fsize_t newsize) __attribute__ ((always_inline));
static inline errval_t trivfs_truncate_call__tx(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_fh_t file, trivfs_fsize_t newsize)
{
    return(((_binding->tx_vtbl).truncate_call)(_binding, _continuation, file, newsize));
}

static inline errval_t trivfs_truncate_response__tx(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_errval_t err) __attribute__ ((always_inline));
static inline errval_t trivfs_truncate_response__tx(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_errval_t err)
{
    return(((_binding->tx_vtbl).truncate_response)(_binding, _continuation, err));
}

static inline errval_t trivfs_create_call__tx(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_fh_t dir, const char *name) __attribute__ ((always_inline));
static inline errval_t trivfs_create_call__tx(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_fh_t dir, const char *name)
{
    return(((_binding->tx_vtbl).create_call)(_binding, _continuation, dir, name));
}

static inline errval_t trivfs_create_response__tx(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_errval_t err, trivfs_fh_t fh) __attribute__ ((always_inline));
static inline errval_t trivfs_create_response__tx(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_errval_t err, trivfs_fh_t fh)
{
    return(((_binding->tx_vtbl).create_response)(_binding, _continuation, err, fh));
}

static inline errval_t trivfs_mkdir_call__tx(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_fh_t dir, const char *name) __attribute__ ((always_inline));
static inline errval_t trivfs_mkdir_call__tx(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_fh_t dir, const char *name)
{
    return(((_binding->tx_vtbl).mkdir_call)(_binding, _continuation, dir, name));
}

static inline errval_t trivfs_mkdir_response__tx(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_errval_t err, trivfs_fh_t fh) __attribute__ ((always_inline));
static inline errval_t trivfs_mkdir_response__tx(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_errval_t err, trivfs_fh_t fh)
{
    return(((_binding->tx_vtbl).mkdir_response)(_binding, _continuation, err, fh));
}

static inline errval_t trivfs_delete_call__tx(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_fh_t fh) __attribute__ ((always_inline));
static inline errval_t trivfs_delete_call__tx(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_fh_t fh)
{
    return(((_binding->tx_vtbl).delete_call)(_binding, _continuation, fh));
}

static inline errval_t trivfs_delete_response__tx(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_errval_t err) __attribute__ ((always_inline));
static inline errval_t trivfs_delete_response__tx(struct trivfs_binding *_binding, struct event_closure _continuation, trivfs_errval_t err)
{
    return(((_binding->tx_vtbl).delete_response)(_binding, _continuation, err));
}


/*
 * Backend-specific includes
 */
#ifdef CONFIG_FLOUNDER_BACKEND_LMP
#include <if/trivfs_lmp_defs.h>
#endif // CONFIG_FLOUNDER_BACKEND_LMP
#ifdef CONFIG_FLOUNDER_BACKEND_UMP
#include <if/trivfs_ump_defs.h>
#endif // CONFIG_FLOUNDER_BACKEND_UMP
#ifdef CONFIG_FLOUNDER_BACKEND_UMP_IPI
#include <if/trivfs_ump_ipi_defs.h>
#endif // CONFIG_FLOUNDER_BACKEND_UMP_IPI
#ifdef CONFIG_FLOUNDER_BACKEND_MULTIHOP
#include <if/trivfs_multihop_defs.h>
#endif // CONFIG_FLOUNDER_BACKEND_MULTIHOP
#ifdef CONFIG_FLOUNDER_BACKEND_UDN
#include <if/trivfs_udn_defs.h>
#endif // CONFIG_FLOUNDER_BACKEND_UDN
/*
 * And we're done
 */
#endif // __trivfs_IF_H
