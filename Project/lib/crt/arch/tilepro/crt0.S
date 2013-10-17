/**
 * \libc startup code.
 */

#include <barrelfish/dispatch.h>
#include <barrelfish_kpi/registers_arch.h>

#include <arch/abi.h>
#include <arch/chip.h>

	.text
	.globl  _start, _start_init

_start:
        // Entry for processes other than init
        // Prepare arg1 init_dom_arg (false)
        move r0, TILE_THREAD_REG
        movei r1, 0

_start_generic:
		// set up stack here
		addi sp, sp, -C_ABI_SAVE_AREA_SIZE

		// Call barrelfish_init_disabled(struct dispatcher* d, bool init_dom_arg)
	    j barrelfish_init_disabled

_start_init:
        // Entry for the init process
        // Prepare arg1 init_dom_arg (true)
        move r0, TILE_THREAD_REG
        movei r1, 1
        j _start_generic
