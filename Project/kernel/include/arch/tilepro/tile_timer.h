/*
 * timer.h
 *
 *  Created on: Feb 11, 2013
 *      Author: robert
 */

#ifndef TILETIMER_H_
#define TILETIMER_H_

void init_tile_timer(void);

int handle_tile_timer_irq(int vecnum, char vecname[], int unused, arch_registers_state_t* save_area);


#endif /* TILETIMER_H_ */
