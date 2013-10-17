/*
 * loader_libc.c
 *
 *  Created on: Dec 12, 2012
 *      Author: robert
 */

#include <stdint.h>
#include <stddef.h>
#include <hv/hypervisor.h>
#include "loader_libc.h"

static size_t strlen(const char *s) {
	size_t i = 0;
	while (*s != '\0') {
		i++;
		s++;
	}
	return i;
}

// Maybe the worst way to implement this, but it's called
// only a couple of times and not accessible for everyone.

// 2nd param must have 11 bytes of space.
static char* uint32_to_string(uint32_t in, char *out) {
	// Max size for 32bits is 10 decimal digits.
	// The eleventh byte will get the termination char.
	out[10] = (char)'\0';

	int i = 9;

	while (in >= 10) {
		out[i--] = (char) (in % 10) + '0';
		in = in / 10;
	}
	out[i] = (char) in + '0';
	return &out[i];
}

static char to_hex_digit(uint32_t digit) {
	assert(digit >= 0);
	assert(digit < 16);
	char c;
	if (digit < 10) {
		c = (char) digit + '0';
	} else {
		c = (char) digit - 10 + 'A';
	}
	return c;
}

// 2nd param must have 11 bytes of space.
static char* uint32_to_hex_string(uint32_t in, char *out) {
	// Max size for 32bits in hex is 8 digits. plus "0x"
	// The eleventh byte will get the termination char.
	out[10] = (char)'\0';

	int i = 9;

	while (in >= 16) {
		out[i--] = to_hex_digit(in % 16);
		in = in / 16;
	}
	out[i--] = to_hex_digit(in);
	out[i--] = 'x';
	out[i] = '0';
	return &out[i];
}

void loader_print(const char* out) {
	int len = strlen(out);
	hv_console_write((HV_VirtAddr) out, len);
}

void loader_print_uint32(uint32_t value) {
	char buff[11];
	char *buff_ptr = uint32_to_string(value, buff);
	loader_print(buff_ptr);
}

void loader_print_xint32(uint32_t value) {
	char buff[11];
	char *buff_ptr = uint32_to_hex_string(value, buff);
	loader_print(buff_ptr);
}

void loader_print_string_uint32(char *start, uint32_t value, char *end) {
	loader_print(start);
	loader_print_uint32(value);
	loader_print(end);
}

void loader_print_string_xint32(char *start, uint32_t value, char *end) {
	loader_print(start);
	loader_print_xint32(value);
	loader_print(end);
}

void __assert_func(const char *file, int line, const char *func, const char *expr, int assertion) {
	if (assertion) {
		loader_print("Assertion \"");
	}
	loader_print(expr);
	if (assertion) {
		loader_print("\" failed in ");
	} else {
		loader_print(" - ");
	}
	loader_print(func);
	loader_print(" at ");
	loader_print(file);
	loader_print(":");
	loader_print_uint32(line);
	loader_print("\n");
	__asm volatile("ill");
}

// Copied from arm_molly
void *
memcpy(void *dst, const void *src, size_t len)
{
    char *d = dst;
    const char *s = src;

    /* check that we don't overlap (should use memmove()) */
    assert((src < dst && src + len <= dst) || (dst < src && dst + len <= src));
    while (len--)
        *d++ = *s++;
    return dst;
}

void *
memset (void *s, int c, size_t n)
{
    uint8_t *p = (uint8_t *)s;
    for (size_t m = 0; m < n; m++) {
        *p++ = c;
    }
    return s;
}
