/*
 * test_defines.h
 *
 *  Created on: Sep 18, 2012
 *      Author: robert
 */

#ifndef TEST_MACROS_H_
#define TEST_MACROS_H_

// Tells us, we are in testing mode
#define TESTING_MODE

//#define lpaddr_t uintptr_t

void panic(const char * NTS, ...)
    __attribute__((noreturn, format(printf, 1, 2)));

#endif /* TEST_MACROS_H_ */
