/*
 * syscall_arch.h
 *
 *  Created on: Aug 17, 2012
 *      Author: robert
 */

.text
.globl syscall
syscall:
	// r10 should have the syscall number. r0-r9 are the other arguments.
	// r0-r9 are passed directly by the C call according to the ABI.
	// The num-param will be on the stack and lie at sp+8.
	addi r10, sp, 8
	// r10 contains now sp+8, load the value
	lw r10, r10

	// Careful with this. Helpful for debugging, but otherwise this might be
	// reordered after the syscall and create an endless loop.
	//__insn_mtspr(SPR_SIM_CONTROL, SIM_CONTROL_PANIC);

	// issue interrupt
	swint1

swi_done:
	// Returned struct is in r0 & r1 and will also passed again in this.
	// Since we didn't need to touch anything, we don't need now.
	jrp lr
