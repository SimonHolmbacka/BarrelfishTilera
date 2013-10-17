/*
 * testcase.c
 *
 *  Created on: Sep 18, 2012
 *      Author: robert
 */

#include "testcase.h"

void XassertEquals_64t_in_hex(uint64_t expected, uint64_t actual, char file[], int line)
{
	printf("[%s:%d] Expecting: 0x%" PRIx64 " == 0x%" PRIx64 "\n",file, line , expected, actual);
	assert(expected == actual);
}

void XassertEquals_32t_in_hex(uint32_t expected, uint32_t actual, char file[], int line)
{
	printf("[%s:%d] Expecting: 0x%" PRIx32 " == 0x%" PRIx32 "\n",file, line , expected, actual);
	assert(expected == actual);
}

void XassertEquals_16t_in_hex(uint16_t expected, uint16_t actual, char file[], int line)
{
	printf("[%s:%d] Expecting: 0x%" PRIx16 " == 0x%" PRIx16 "\n",file, line , expected, actual);
	assert(expected == actual);
}

void XassertNotEquals_64t_in_hex(uint64_t expected, uint64_t actual, char file[], int line)
{
	printf("[%s:%d] Expecting: 0x%" PRIx64 " != 0x%" PRIx64 "\n",file, line , expected, actual);
	assert(expected != actual);
}

void XassertSmallerThan_u64t(uint64_t left, uint64_t right, char file[], int line)
{
	printf("[%s:%d] Expecting: %" PRIu64 " < %" PRIu64 "\n",file, line , left, right);
	assert(left < right);
}
