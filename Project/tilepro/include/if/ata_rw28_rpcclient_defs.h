#ifndef __ata_rw28_RPC_CLIENT_H
#define __ata_rw28_RPC_CLIENT_H 1
/*
 * Copyright (c) 2010, ETH Zurich.
 * All rights reserved.
 * 
 * INTERFACE NAME: ata_rw28
 * INTEFACE FILE: /home/andreas/barrelfish/new_test2/if/ata_rw28.if
 * INTERFACE DESCRIPTION: ATA read & write with 28-bit LBA
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

#include <if/ata_rw28_defs.h>

/*
 * Forward declaration of binding type
 */
struct ata_rw28_rpc_client;

/*
 * Function signatures
 */
typedef  errval_t ata_rw28_read_dma__rpc_method_fn(struct ata_rw28_rpc_client *_rpc, uint32_t read_size, uint32_t start_lba, uint8_t **buffer, size_t *buffer_size);
typedef  errval_t ata_rw28_read_dma_block__rpc_method_fn(struct ata_rw28_rpc_client *_rpc, uint32_t lba, uint8_t **buffer, size_t *buffer_size);
typedef  errval_t ata_rw28_write_dma__rpc_method_fn(struct ata_rw28_rpc_client *_rpc, const uint8_t *buffer, size_t buffer_size, uint32_t lba, ata_rw28_errval_t *status);
typedef  errval_t ata_rw28_identify_device__rpc_method_fn(struct ata_rw28_rpc_client *_rpc, uint8_t **buffer, size_t *buffer_size);
typedef  errval_t ata_rw28_flush_cache__rpc_method_fn(struct ata_rw28_rpc_client *_rpc, ata_rw28_errval_t *status);

/*
 * VTable struct definition for the interface
 */
struct ata_rw28_rpc_vtbl {
    ata_rw28_read_dma__rpc_method_fn *read_dma;
    ata_rw28_read_dma_block__rpc_method_fn *read_dma_block;
    ata_rw28_write_dma__rpc_method_fn *write_dma;
    ata_rw28_identify_device__rpc_method_fn *identify_device;
    ata_rw28_flush_cache__rpc_method_fn *flush_cache;
};

/*
 * The Binding structure
 */
struct ata_rw28_rpc_client {
    struct ata_rw28_binding *b;
    struct ata_rw28_rpc_vtbl vtbl;
    bool rpc_in_progress;
    bool reply_present;
    errval_t async_error;
    struct waitset rpc_waitset;
    struct waitset_chanstate dummy_chanstate;
};

/*
 * Function to initialise an RPC client
 */
extern  errval_t ata_rw28_rpc_client_init(struct ata_rw28_rpc_client *rpc, struct ata_rw28_binding *binding);

#endif // __ata_rw28_RPC_CLIENT_H
