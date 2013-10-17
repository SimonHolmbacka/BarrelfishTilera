/*
 * irq.h
 *
 *  Created on: Aug 22, 2012
 *      Author: robert
 */

#ifndef KERNEL_ARCH_TILEPRO_IRQ_H
#define KERNEL_ARCH_TILEPRO_IRQ_H

errval_t irq_table_set(unsigned int nidt, capaddr_t endpoint);
errval_t irq_table_delete(unsigned int nidt);

#endif /* KERNEL_ARCH_TILEPRO_IRQ_H */
