/*
 * tilepro_core_data.h
 *
 *  Created on: Jan 23, 2013
 *      Author: robert
 */

#ifndef TILEPRO_CORE_DATA_H_
#define TILEPRO_CORE_DATA_H_

#define MAX_PHYSICAL_CORES 256

#define CORE_DATA_ADDRESS 0x270000

struct physical_mem_range {
	lpaddr_t start;
	uint64_t size;
	uint64_t bytes_free;
};

struct single_core_data {
	// no multiboot
    // elf structure is read by the loader
	// cpu module is loaded and address is known (no relocation)
    uint32_t urpc_frame_base;
    uint8_t urpc_frame_bits;
    struct physical_mem_range phys_mem;
    coreid_t src_core_id;
    uint8_t src_arch_id;
    coreid_t dst_core_id;
    uint32_t chan_id;
    uint32_t cmdline;

};

/**
 * \brief Data sent to a newly booted kernel
 *
 */
struct tilepro_core_data {
	uint32_t n_data;
    genpaddr_t monitor_binary;
    genpaddr_t monitor_binary_size;
	struct single_core_data per_core[MAX_PHYSICAL_CORES];
};

#endif /* TILEPRO_CORE_DATA_H_ */
