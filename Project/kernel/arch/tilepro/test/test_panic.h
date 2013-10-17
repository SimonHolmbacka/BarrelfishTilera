/*
 * test_panic.h
 *
 *  Created on: Sep 18, 2012
 *      Author: robert
 */

#ifndef TEST_PANIC_H_
#define TEST_PANIC_H_

void panic(const char *msg, ...);
int panic_occured();
void reset_panic();

#endif /* TEST_PANIC_H_ */
