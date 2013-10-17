/*
 * timer.c
 *
 *  Created on: Feb 11, 2013
 *      Author: robert
 */

#include <kernel.h>
#include <dispatch.h>
#include <tile_timer.h>
#include <wakeup.h>
#include <arch/abi.h>
#include <arch/interrupts.h>
#include <arch/spr_def.h>

static inline void interrupt_mask_reset(int interrupt_number) {
	if (interrupt_number < 32) {
		int mask = 1 << interrupt_number;
		__insn_mtspr(SPR_INTERRUPT_MASK_RESET_1_0, mask);
	} else {
		int mask = 1 << (interrupt_number - 32);
		__insn_mtspr(SPR_INTERRUPT_MASK_RESET_1_1, mask);
	}
}

static inline void interrupt_mask_set(int interrupt_number) {
	if (interrupt_number < 32) {
		int mask = 1 << interrupt_number;
		__insn_mtspr(SPR_INTERRUPT_MASK_SET_1_0, mask);
	} else {
		int mask = 1 << (interrupt_number - 32);
		__insn_mtspr(SPR_INTERRUPT_MASK_SET_1_1, mask);
	}
}

static inline void setup_timer(void) {
	// TODO There should be a "reasonable" value here, and not a guessed one.
    //__insn_mtspr(SPR_TILE_TIMER_CONTROL, 0x7FFFFFFF);
    __insn_mtspr(SPR_TILE_TIMER_CONTROL, 0x00FFFFFF);
}

void init_tile_timer(void) {
    printf("mask before: %lX\n", __insn_mfspr(SPR_INTERRUPT_MASK_1_0));
    interrupt_mask_reset(INT_TILE_TIMER);
    printf("mask after:  %lX\n", __insn_mfspr(SPR_INTERRUPT_MASK_1_0));
    setup_timer();
    printf("inited tile timer\n");
}

int handle_tile_timer_irq(int vecnum, char vecname[], int unused, arch_registers_state_t* save_area) {
	// XXX Timer don't work due to some problems, when the interrupt is handed via a downcall from the hypervisor.
	// Or at least, when the fault_pc is in the hypervisor glue code (0xFD01xxxx)

	// If the rest of the implementation works is unknown, because we weren't able to test it.
	interrupt_mask_set(INT_TILE_TIMER);
	uint32_t fault_pc = __insn_mfspr(SPR_EX_CONTEXT_1_0);
	printf("handle_tile_timer_irq [%s]; pc at: 0x%.8lX\n", vecname, fault_pc);
	printf("kernel_timeslice: %d\n", kernel_timeslice);

    if (dcb_current != NULL) {
    	dispatcher_handle_t handle = dcb_current->disp;
		if (save_area == dispatcher_get_disabled_save_area(handle)) {
			assert(dispatcher_is_disabled_ip(handle, fault_pc));
			dcb_current->disabled = true;
		} else {
			assert(save_area == dispatcher_get_enabled_save_area(handle));
			assert(!dispatcher_is_disabled_ip(handle, fault_pc));
			dcb_current->disabled = false;
		}
    }

    //assert(kernel_ticks_enabled);
    kernel_now += kernel_timeslice; //kernel_timeslice needs to be defined or something
    wakeup_check(kernel_now);
    setup_timer(); // or before wakeup_check?
    interrupt_mask_reset(INT_TILE_TIMER);
    dispatch(schedule());
    return 0;
}

