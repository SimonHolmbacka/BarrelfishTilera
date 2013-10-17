/*
 * test_endian.c
 *
 *  Created on: Aug 22, 2012
 *      Author: barrelfish
 */

#include "testcase.h"
#include "machine/endian.h"

void test64bits()
{
	uint64_t in = 0x123456789ABCDEF0ull;
	uint64_t expected = 0xF0DEBC9A78563412ull;
	uint64_t result = __bswap64(in);
	assertEquals_64t_in_hex(expected, result);
}

void test32bits()
{
	uint32_t in = 0x12345678;
	uint32_t expected = 0x78563412;
	uint32_t result = __bswap32(in);
	assertEquals_32t_in_hex(expected, result);
}

void test16bits()
{
	__uint16_t in = 0x1234;
	__uint16_t expected = 0x3412;
	__uint16_t result = __bswap16(in);
	assertEquals_16t_in_hex(expected, result);
}

void endian_test()
{
	test64bits();
	test32bits();
	test16bits();
}
