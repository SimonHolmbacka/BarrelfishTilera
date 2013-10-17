
.section .rodata, "a"
halt_msg:
	.ascii  "Called halt() on core "
.Lhalt_msg:

end_of_msg:
	.ascii  ".\n"
.Lend_of_msg:

.macro	console_print label
	moveli  r0, lo16(\label)
	auli    r0, r0, ha16(\label)
	moveli r1, .L\()\label - \label
	jal    hv_console_write
.endm

.data
char:
.word 0x0

.text
.global halt

/**
 * extern "C" void halt(void) __attribute__((noreturn))
 */
halt:
	.align 8
	// Way of printing + halting without any stack needed (only stack from the hypervisor).

	console_print halt_msg

	// Put cpu-id in r1
	mfspr	r1, SYSTEM_SAVE_1_0
	// Create char for cpu_id (looks bad, if greater >= 10)
	addi	r1, r1, '0'
	// Move addr of char to r0
	moveli	r0, lo16(char)
	auli	r0, r0, ha16(char)
	// store cpu-id there
	sw		r0, r1
	moveli	r1, 1
	// print that char
	jal		hv_console_write
	// print end of msg (newline)
	console_print end_of_msg
	// evil idle
    j       .

.end
