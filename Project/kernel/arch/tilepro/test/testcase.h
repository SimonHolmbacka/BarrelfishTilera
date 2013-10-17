/*
 * testcase.h
 *
 *  Created on: Sep 18, 2012
 *      Author: robert
 */

#ifndef TESTCASE_H_
#define TESTCASE_H_

#include <inttypes.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <bits/types.h>

void XassertEquals_64t_in_hex(uint64_t expected, uint64_t actual, char file[], int line);

void XassertEquals_32t_in_hex(uint32_t expected, uint32_t actual, char file[], int line);

void XassertEquals_16t_in_hex(uint16_t expected, uint16_t actual, char file[], int line);

void XassertNotEquals_64t_in_hex(uint64_t expected, uint64_t actual, char file[], int line);

void XassertSmallerThan_u64t(uint64_t left, uint64_t right, char file[], int line);

#define assertEquals_64t_in_hex(e, a) XassertEquals_64t_in_hex(e, a, __FILE__, __LINE__)
#define assertEquals_32t_in_hex(e, a) XassertEquals_32t_in_hex(e, a, __FILE__, __LINE__)
#define assertEquals_16t_in_hex(e, a) XassertEquals_16t_in_hex(e, a, __FILE__, __LINE__)
#define assertNotEquals_64t_in_hex(e, a) XassertNotEquals_64t_in_hex(e, a, __FILE__, __LINE__)
#define assertSmallerThan_u64t(l, r) XassertSmallerThan_u64t(l, r, __FILE__, __LINE__)

#endif /* TESTCASE_H_ */
