/*
 * kputchar.h
 *
 *  Created on: Aug 22, 2012
 *      Author: robert
 */

#ifndef KERNEL_ARCH_TILEPRO_KPUTCHAR_H
#define KERNEL_ARCH_TILEPRO_KPUTCHAR_H

void kprintf_begin(void);
void kprintf_end(void);

int kputchar(int c);

#endif /* KERNEL_ARCH_TILEPRO_KPUTCHAR_H */
