/*
 * test_panic.c
 *
 *  Created on: Sep 18, 2012
 *      Author: robert
 */

#include "test_panic.h"

int panic_there_was = 0;

void panic(const char *msg, ...)
{
	panic_there_was = 1;
}

int panic_occured()
{
	return panic_there_was;
}

void reset_panic()
{
	panic_there_was = 0;
}
