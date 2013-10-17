/*
 * asm_inlines_arch.h
 *
 *  Created on: Sep 11, 2012
 *      Author: robert
 */

#ifndef ARCH_TILEPRO_BARRELFISH_KPI_ASM_INLINE_H
#define ARCH_TILEPRO_BARRELFISH_KPI_ASM_INLINE_H

#ifndef __ASSEMBLER__

/** \brief This code reads the cycle counter */
static inline uint64_t rdtsc(void)
{
    uint32_t cycle_high;
    uint32_t cycle_low;

    __asm __volatile(
    		"mfspr %[cycle_high], CYCLE_HIGH	\n\t"
    		"mfspr %[cycle_low], CYCLE_LOW		\n\t"
    		: [cycle_high] "=r" (cycle_high),
    		  [cycle_low] "=r" (cycle_low)
    );

    return ((uint64_t)cycle_high << 32) | (uint64_t)cycle_low;
}

//static inline void init_rdtsc(void) {
//	uint32_t in = 1;
//    __asm __volatile(
//    		"mtspr MPL_WORLD_ACCESS_SET_0, %[in]	\n\t"
//    		:
//    		: [in] "r" (in)
//    );
//}

static inline void mfence(void)
{
    __asm volatile("mf");
}

#endif /* __ASSEMBLER__ */

#endif /* ARCH_TILEPRO_BARRELFISH_KPI_ASM_INLINE_H */
