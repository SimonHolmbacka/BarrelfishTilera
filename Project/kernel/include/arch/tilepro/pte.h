/*
 * pte.h
 *
 *  Created on: Jul 24, 2012
 *      Author: robert
 */

#ifndef PTE_H_
#define PTE_H_

/*
 * These macros are used for static definitions in data sections
 */

#define PAGE_TABLE_ENTRY_16M(start, va, cpa, highword, lowword) \
	/* Advance from start of page table by index of entry multiplied by the size of one entry */ \
	.org start + HV_L1_INDEX(va) * HV_PTE_SIZE; \
	/* The entry is valid (present); a large page; global (as it belongs to the supervisor);
	 * accessed (accessed for the first time); dirty (as written for the first time) &
	 * page is only held in L1 / L2 or memory, not in L3. */ \
	 .word (HV_PTE_PRESENT | HV_PTE_PAGE | HV_PTE_GLOBAL | \
			HV_PTE_ACCESSED |	HV_PTE_DIRTY | \
			(HV_PTE_MODE_CACHE_NO_L3 << HV_PTE_INDEX_MODE) | lowword); \
	/** The access bytes moved by 32, because this is a new word directive */ \
	.word (highword >> 32 | (HV_CPA_TO_PFN(cpa) << (HV_PTE_INDEX_PFN - 32)));

#define PAGE_TABLE_ENTRY_LINK_TO_L2(start, va, l2_pa) \
	/* Advance from start of page table by index of entry multiplied by the size of one entry */ \
	.org start + HV_L1_INDEX(va) * HV_PTE_SIZE; \
	/* The entry is valid (present); no page (i.e. a link);
	 page is only held in L1 / L2 or memory, not in L3. */ \
	 .word (HV_PTE_PRESENT | (HV_PTE_MODE_CACHE_NO_L3 << HV_PTE_INDEX_MODE)); \
	/** The access bytes moved by 32, because this is a new word directive */ \
	.word ((HV_CPA_TO_PTFN(l2_pa) << (HV_PTE_INDEX_PTFN - 32)));

#define PAGE_TABLE_ENTRY_64K(start, sub_va, cpa, highword, lowword) \
	/* Advance from start of page table by index of entry multiplied by the size of one entry */ \
	/* If the upper-most 8 bit (24-31) are right can't be detected here, but they are ignored, so don't cause a problem _here_ */ \
	/* The user is responsible himself for using the right L2-table  */ \
	.org start + HV_L2_INDEX(sub_va) * HV_PTE_SIZE; \
	/* The entry is valid (present); global (as it belongs to the supervisor);
	 * accessed (accessed for the first time); dirty (as written for the first time) &
	 * page is only held in L1 / L2 or memory, not in L3. */ \
	 .word (HV_PTE_PRESENT | HV_PTE_GLOBAL | \
			HV_PTE_ACCESSED |	HV_PTE_DIRTY | \
			(HV_PTE_MODE_CACHE_NO_L3 << HV_PTE_INDEX_MODE) | lowword); \
	/** The access bytes moved by 32, because this is a new word directive */ \
	.word (highword >> 32 | (HV_CPA_TO_PFN(cpa) << (HV_PTE_INDEX_PFN - 32)));

#ifndef __ASSEMBLER__
/*
 * A page table entry (64-bit)
 * used in dynamic definitions
 */
union Tile_PTE {
	uint64_t raw;

	struct {
		uint32_t least;
		uint32_t most;
	} wordwise;

    struct {
    	uint64_t present			:  1;  // (bit 0) PTE is valid?
    	uint64_t migrating			:  1;  // (bit 1) page is migrating?
    	uint64_t client_0			:  1;  // (bit 2) page client state 0
    	uint64_t client_1			:  1;  // (bit 3) page client state 1
    	uint64_t nc					:  1;  // (bit 4) L1 & L2 caches incoherent with L3 cache
    	uint64_t no_alloc_l1		:  1;  // (bit 5) page is uncached in local L1 cache
    	uint64_t no_alloc_l2		:  1;  // (bit 6) page is uncached in local L2 cache
    	uint64_t cached_priority	:  1;  // (bit 7) page is priority cached
    	uint64_t page				:  1;  // (bit 8) PTE is a page?
    	uint64_t global				:  1;  // (bit 9) page is global?
    	uint64_t user				:  1;  // (bit 10) page is user-accessible
    	uint64_t accessed			:  1;  // (bit 11) page has been accessed?
    	uint64_t dirty				:  1;  // (bit 12) page has been written
    	uint64_t reserved			:  2;  // (bit 13-14) two reserved bits
    	uint64_t super				:  1;  // (bit 15) does this PTE implicitly reference multiple pages?
    	uint64_t mode				:  3;  // (bit 16-18) page mode
    	uint64_t client_2			:  1;  // (bit 19) page client state 2
    	uint64_t lotar				: 12;  // (bit 20-31) page's lotar (location overriden target)
    	uint64_t readable			:  1;  // (bit 32) page is readable?
    	uint64_t writable			:  1;  // (bit 33) page is writable?
    	uint64_t executable			:  1;  // (bit 34) page is executable?
    	uint64_t ptfn_lower_part	:  5;  // (bit 35-39) page's PTFN (page table frame number)
    	uint64_t pfn				: 24;  // (bit 40-63) page's PFN (page frame number)
    } pte;
};
#endif /* __ASSEMBLER__ */

#endif /* PTE_H_ */
