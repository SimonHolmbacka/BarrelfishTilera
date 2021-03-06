#ifndef __skb_map_UDN_H
#define __skb_map_UDN_H 1
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

/*
 * UDN interconnect driver
 */

#include <barrelfish/udn_chan.h>
#include <flounder/flounder_support_udn.h>

struct skb_map_udn_binding {
    struct skb_map_binding b;
    struct flounder_udn_state udn_state;
    
    /* bind params for the new monitor continuation */
    iref_t iref;
};

extern  void skb_map_udn_destroy(struct skb_map_udn_binding *b);
extern  errval_t skb_map_udn_bind(struct skb_map_udn_binding *b, iref_t iref, skb_map_bind_continuation_fn *_continuation, void *st, struct waitset *waitset, idc_bind_flags_t flags);
extern  errval_t skb_map_udn_connect_handler(void *st, struct monitor_binding *mb, uintptr_t mon_id, struct capref notify_cap, coreid_t dest_core_id, uint32_t out_chan_id, uint8_t dest_asid);
extern  void skb_map_udn_rx_handler(void *arg);
extern  errval_t skb_map_udn_init(struct skb_map_udn_binding *b, struct waitset *waitset, coreid_t dest_core_id, uint32_t out_chan_id, uint8_t dest_asid);

#endif // __skb_map_UDN_H
