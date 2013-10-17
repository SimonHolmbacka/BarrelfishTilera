/*
 * syscall_arch.h
 *
 *  Created on: Aug 17, 2012
 *      Author: robert
 */

#ifndef ARCH_TILEPRO_BARRELFISH_SYSCALL_H
#define ARCH_TILEPRO_BARRELFISH_SYSCALL_H

/*
 * Syscall function will go to the assembly code syscall.S.
 * In our case, we have 10 registers (r0-r9) used to pass arguments for syscalls.
 *
 * Here is the syscall convention for our arch, for any syscallx(),
 * 1. arg0 for syscall number, which will be put into register r10.
 * 2. arg1 will be put into the register r0 (32 bit), it contains several variables,
 *    arg1[7:0]   cmd type;
 *    arg1[15:8]  invoke bits;
 *    arg1[23:16] send bit, if message passing;
 *    arg1[27:24] flags for message passing;
 *    arg1[31:28] word length for message passing;
 *    (length_words << 28) | ((flags & 0xf) << 24) | (invoke_bits << 16) | (send_bits << 8).
 *    We need to do this way, because we need to save more registers for message passing, which also uses syscall.
 * 3.
 *
 */
extern struct sysret
syscall(uintptr_t b, uintptr_t c, uintptr_t d, uintptr_t e,
        uintptr_t f, uintptr_t g, uintptr_t h, uintptr_t i,
        uintptr_t j, uintptr_t k, uintptr_t num);

#define syscallx(num,b,c,d,e,f,g,h,i,j,k)                       \
    syscall(b,c,d,e,f,g,h,i,j,k,num)

#define syscall11(_num, _b, _c, _d, _e, _f, _g, _h, _i, _j, _k)	\
	syscallx(_num, _b, _c, _d, _e, _f, _g, _h, _i, _j, _k)
#define syscall10(_num, _b, _c, _d, _e, _f, _g, _h, _i, _j)    	\
    syscall11(_num, _b, _c, _d, _e, _f, _g, _h, _i, _j, 0)
#define syscall9(_num, _b, _c, _d, _e, _f, _g, _h, _i)    		\
    syscall10(_num, _b, _c, _d, _e, _f, _g, _h, _i, 0)
#define syscall8(_num, _b, _c, _d, _e, _f, _g, _h)    			\
    syscall9(_num, _b, _c, _d, _e, _f, _g, _h, 0)
#define syscall7(_num, _b, _c, _d, _e, _f, _g)    				\
    syscall8(_num, _b, _c, _d, _e, _f, _g, 0)
#define syscall6(_num, _b, _c, _d, _e, _f)        				\
    syscall7(_num, _b, _c, _d, _e, _f, 0)
#define syscall5(_num, _b, _c, _d, _e)            				\
    syscall6(_num, _b, _c, _d, _e, 0)
#define syscall4(_num, _b, _c, _d)                				\
    syscall5(_num, _b, _c, _d, 0)
#define syscall3(_num, _b, _c)                    				\
    syscall4(_num, _b, _c, 0)
#define syscall2(_num, _b)                        				\
    syscall3(_num, _b, 0)
#define syscall1(_num)                            				\
    syscall2(_num, 0)

#endif /* ARCH_TILEPRO_BARRELFISH_SYSCALL_H */
