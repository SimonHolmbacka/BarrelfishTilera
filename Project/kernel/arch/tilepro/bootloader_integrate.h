/*
 * boot_elf32_integrate.h
 *
 *  Created on: Dec 12, 2012
 *      Author: robert
 */
// File is needed to be able to use the elfxx.c without having a lot of includes,
// which need other includes.

#ifndef TILEPRO_BOOTLOADER_INTEGRATE_H_
#define TILEPRO_BOOTLOADER_INTEGRATE_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <errors/errno.h>

typedef uint64_t genvaddr_t;
typedef uintptr_t lvaddr_t;
typedef uint64_t lpaddr_t;

#include "loader_libc.h"
#include <elf/elf.h>

// Return if machine is big endian.
static bool is_big_endian(void)
{
#ifdef __tilepro__
	return 0;
#else
#error I do not know...
#endif
}

#endif /* TILEPRO_BOOTLOADER_INTEGRATE_H_ */
