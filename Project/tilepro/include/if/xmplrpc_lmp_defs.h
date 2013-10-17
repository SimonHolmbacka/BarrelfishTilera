#ifndef __xmplrpc_LMP_H
#define __xmplrpc_LMP_H 1
/*
 * Copyright (c) 2010, ETH Zurich.
 * All rights reserved.
 * 
 * INTERFACE NAME: xmplrpc
 * INTEFACE FILE: /home/andreas/barrelfish/new_test2/if/xmplrpc.if
 * INTERFACE DESCRIPTION: Example rpc interface
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
 * LMP interconnect driver
 */

#include <barrelfish/lmp_chan.h>

struct xmplrpc_lmp_binding {
    struct xmplrpc_binding b;
    struct lmp_chan chan;
    lmp_send_flags_t flags;
};

extern  void xmplrpc_lmp_init(struct xmplrpc_lmp_binding *b, struct waitset *waitset);
extern  void xmplrpc_lmp_destroy(struct xmplrpc_lmp_binding *b);
extern  errval_t xmplrpc_lmp_bind(struct xmplrpc_lmp_binding *b, iref_t iref, xmplrpc_bind_continuation_fn *_continuation, void *st, struct waitset *waitset, idc_bind_flags_t flags, size_t lmp_buflen);
extern  errval_t xmplrpc_lmp_connect_handler(void *st, size_t buflen_words, struct capref endpoint, struct lmp_chan **retchan);
extern  void xmplrpc_lmp_rx_handler(void *arg);

#endif // __xmplrpc_LMP_H