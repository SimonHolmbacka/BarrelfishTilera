#ifndef __ata_rw28_AHCI_IF_H
#define __ata_rw28_AHCI_IF_H 1
/*
 * Copyright (c) 2010, ETH Zurich.
 * All rights reserved.
 * 
 * INTERFACE NAME: ahci_ata_rw28
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

#include <ahci/ahci.h>
#include <if/ata_rw28_defs.h>

/*
 * Forward declaration of binding type
 */
struct ahci_ata_rw28_binding;

/*
 * The binding structure
 */
struct ahci_ata_rw28_binding {
    /* Binding supertype */
    struct ata_rw28_binding b;
    
    /* Binding to libahci */
    struct ahci_binding *b_lib;
    
};

/*
 * Function to initialize an AHCI client
 */
extern  errval_t ahci_ata_rw28_init(struct ahci_ata_rw28_binding *binding, struct waitset *waitset, struct ahci_binding *ahci_binding);

#endif // __ata_rw28_AHCI_IF_H
