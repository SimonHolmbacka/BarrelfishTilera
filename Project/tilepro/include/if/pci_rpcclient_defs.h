#ifndef __pci_RPC_CLIENT_H
#define __pci_RPC_CLIENT_H 1
/*
 * Copyright (c) 2010, ETH Zurich.
 * All rights reserved.
 * 
 * INTERFACE NAME: pci
 * INTEFACE FILE: /home/andreas/barrelfish/new_test2/if/pci.if
 * INTERFACE DESCRIPTION: The PCI Interface
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
 * RPC client
 */

#include <if/pci_defs.h>

/*
 * Forward declaration of binding type
 */
struct pci_rpc_client;

/*
 * Function signatures
 */
typedef  errval_t pci_init_pci_device__rpc_method_fn(struct pci_rpc_client *_rpc, uint32_t class_code, uint32_t sub_class, uint32_t prog_if, uint32_t vendor_id, uint32_t device_id, uint32_t bus, uint32_t dev, uint32_t fun, uint8_t coreid, uint32_t vector, pci_errval_t *err, uint8_t *nr_allocated_bars, pci_caps_per_bar_t **caps_per_bar);
typedef  errval_t pci_init_legacy_device__rpc_method_fn(struct pci_rpc_client *_rpc, uint16_t iomin, uint16_t iomax, uint8_t irq, uint8_t coreid, uint32_t vector, pci_errval_t *err, struct capref *iocap);
typedef  errval_t pci_get_cap__rpc_method_fn(struct pci_rpc_client *_rpc, uint32_t idx, uint32_t cap_nr, pci_errval_t *err, struct capref *cap, uint8_t *type);

/*
 * VTable struct definition for the interface
 */
struct pci_rpc_vtbl {
    pci_init_pci_device__rpc_method_fn *init_pci_device;
    pci_init_legacy_device__rpc_method_fn *init_legacy_device;
    pci_get_cap__rpc_method_fn *get_cap;
};

/*
 * The Binding structure
 */
struct pci_rpc_client {
    struct pci_binding *b;
    struct pci_rpc_vtbl vtbl;
    bool rpc_in_progress;
    bool reply_present;
    errval_t async_error;
    struct waitset rpc_waitset;
    struct waitset_chanstate dummy_chanstate;
};

/*
 * Function to initialise an RPC client
 */
extern  errval_t pci_rpc_client_init(struct pci_rpc_client *rpc, struct pci_binding *binding);

#endif // __pci_RPC_CLIENT_H
