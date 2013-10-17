/*
 * curdispatcher_arch.h
 *
 *  Created on: Aug 24, 2012
 *      Author: robert
 */

#ifndef ARCH_TILEPRO_BARRELFISH_CURDISPATCHER_H
#define ARCH_TILEPRO_BARRELFISH_CURDISPATCHER_H

//
// Helpers for pasting #defined values into inline assembler.
//
#define STR(x) #x
#define XTR(x) STR(x)

/**
 * \brief Returns pointer to current dispatcher
 */
static inline dispatcher_handle_t curdispatcher(void)
{
	dispatcher_handle_t ret = 0;
	__asm volatile("move %[ret]," XTR(TILE_THREAD_REG) : [ret] "=r" (ret));
	return ret;
}

#endif /* ARCH_TILEPRO_BARRELFISH_CURDISPATCHER_H */
