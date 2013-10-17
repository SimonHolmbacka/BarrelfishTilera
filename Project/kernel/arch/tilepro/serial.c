/*
 * serial.c
 *
 *  Created on: Sep 3, 2012
 *      Author: robert
 */

#include <serial.h>
#include <assert.h>
#include "kputchar.h"
#include "hv/hypervisor.h"

unsigned serial_console_port = 0;
unsigned serial_debug_port = 0;
const unsigned serial_num_physical_ports = 1;

/*
 * Initialize a physical serial port
 */
errval_t serial_init(unsigned port)
{
	    return SYS_ERR_OK;
}
errval_t serial_early_init(unsigned port)
{
	    return SYS_ERR_OK;
}

void serial_putchar(unsigned port, char c)
{
	assert(port == 0);
	kputchar(c);
}

char serial_getchar(unsigned port)
{
	assert(port == 0);
	int read = -1;
	while (read < 0 || read > 255) {
		read = hv_console_read_if_ready();
	}
	return (char)read;
}


