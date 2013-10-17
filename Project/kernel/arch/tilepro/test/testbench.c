/*
 * testbench.c
 *
 *  Created on: Sep 11, 2012
 *      Author: robert
 */

void endian_test();
void asm_inlines_test();
void memory_test();

int main(void) {
	endian_test();
	asm_inlines_test();
	memory_test();
	return 0;
}

