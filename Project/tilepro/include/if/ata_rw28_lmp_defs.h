#ifndef __ata_rw28_LMP_H
#define __ata_rw28_LMP_H 1
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
 * LMP interconnect driver
 */

#include <barrelfish/lmp_chan.h>

struct ata_rw28_lmp_binding {
    struct ata_rw28_binding b;
    struct lmp_chan chan;
    lmp_send_flags_t flags;
};

extern  void ata_rw28_lmp_init(struct ata_rw28_lmp_binding *b, struct waitset *waitset);
extern  void ata_rw28_lmp_destroy(struct ata_rw28_lmp_binding *b);
extern  errval_t ata_rw28_lmp_bind(struct ata_rw28_lmp_binding *b, iref_t iref, ata_rw28_bind_continuation_fn *_continuation, void *st, struct waitset *waitset, idc_bind_flags_t flags, size_t lmp_buflen);
extern  errval_t ata_rw28_lmp_connect_handler(void *st, size_t buflen_words, struct capref endpoint, struct lmp_chan **retchan);
extern  void ata_rw28_lmp_rx_handler(void *arg);

#endif // __ata_rw28_LMP_H
