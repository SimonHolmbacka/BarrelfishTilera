/*
 * memory.h
 *
 *  Created on: Sep 18, 2012
 *      Author: robert
 */

#ifndef ARCH_TILEPRO_VIRT_MEMORY_H_
#define ARCH_TILEPRO_VIRT_MEMORY_H_

#include "mem_loc.h"

void init_virt_mem(void);

uint8_t get_next_asid(void);

#endif /* ARCH_TILEPRO_VIRT_MEMORY_H_ */
