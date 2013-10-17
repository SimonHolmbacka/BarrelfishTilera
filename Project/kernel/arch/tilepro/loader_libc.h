/*
 * loader_libc.h
 *
 *  Created on: Dec 12, 2012
 *      Author: robert
 */

#ifndef TILEPRO_LOADER_LIBC_H_
#define TILEPRO_LOADER_LIBC_H_

#include <stddef.h>
#include <hv/hypervisor.h>

#define assert(__e) ((__e) ? (void)0 : __assert_func(__FILE__, __LINE__, __func__, #__e, 1))
#define panic(__e) __assert_func(__FILE__, __LINE__, __func__, #__e, 0)

void loader_print(const char* out);

void loader_print_uint32(uint32_t value);

void loader_print_string_uint32(char *start, uint32_t value, char *end);

void loader_print_xint32(uint32_t value);

void loader_print_string_xint32(char *start, uint32_t value, char *end);

void __assert_func(const char *file, int line, const char *func, const char *expr, int assertion);

void *
memcpy(void *dst, const void *src, size_t len);

void *
memset (void *s, int c, size_t n);

#endif /* TILEPRO_LOADER_LIBC_H_ */
