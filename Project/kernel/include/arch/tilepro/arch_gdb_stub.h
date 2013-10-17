/*
 * arch_gdb_stub.h
 *
 *  Created on: Aug 22, 2012
 *      Author: robert
 */

extern uintptr_t *gdb_arch_registers;

/** Address of saved registers as void * */
#define GDB_ARCH_REGADDR    ((void*)gdb_arch_registers)

/** Number of bytes saved in GDB frame */
#define GDB_ARCH_REGBYTES   (sizeof(uintptr_t) * GDB_TILEPRO_NUM_REGS)
