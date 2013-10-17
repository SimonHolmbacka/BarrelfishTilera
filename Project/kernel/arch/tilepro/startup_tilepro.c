/*
 * run.c
 *
 *  Created on: Aug 8, 2012
 *      Author: robert
 */

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <kernel.h>
#include <hv/hypervisor.h>
#include <arch/spr_def.h>
#include <paging_kernel_arch.h>
#include <barrelfish_kpi/init.h>
#include <barrelfish_kpi/paging_arch.h>
#include <barrelfish_kpi/spinlocks_arch.h>
#include <startup.h>
#include <dispatch.h>
#include <elf/elf.h>
#include <getopt/getopt.h>
#include <tile_timer.h>

#include <exec.h>

#include <barrelfish_kpi/tilepro_core_data.h>
#include <mem_loc.h>
#include <phys_mem.h>
#include <virt_mem.h>
#include <page_table.h>

#include <arch/sn.h>
#include <arch/udn.h>
#include <arch/idn.h>


#define CNODE(cte) (cte)->cap.u.cnode.cnode

#define bit64_as_2_bit32(x) (uint32_t)((x) >> 32), (uint32_t)(x)
	//TILEmpower boot changes:
/*
#define CPU_MODULE_NAME	    "./cpu"
#define INIT_MODULE_NAME	"./init"
#define MONITOR_MODULE_NAME "./monitor"

*/

#define CPU_MODULE_NAME	    "./tilepro/sbin/cpu"
#define INIT_MODULE_NAME	"./tilepro/sbin/init"
#define MONITOR_MODULE_NAME "./tilepro/sbin/monitor"


//boot changes end

/*
 * reserve one large page size for bootinfo, arguments and dispatcher data.
 * init's text section is defined starting at 0x01000000.
 */
#define INIT_VBASE              0x00010000
#define INIT_BOOTINFO_VBASE     (INIT_VBASE)
// in our case BOOTINFO_SIZE = 256KB, partially defined by BF
#define INIT_ARGS_VBASE         (INIT_BOOTINFO_VBASE + BOOTINFO_SIZE)
// ARGS_SIZE = 128KB, defined by BF
#define INIT_DISPATCHER_VBASE   (INIT_ARGS_VBASE + ARGS_SIZE)
// DISPATCHER_SIZE here is 2MB
#define INIT_DISPATCHER_END 	(INIT_DISPATCHER_VBASE + DISPATCHER_SIZE)

/**
 * One L2 page table maps the amount of a large page (mapped in a number of small pages).
 * To be able to map the whole available user vspace, we need this amount of L2 page tables.
 */
#define BASE_PAGE_TABLE_NUM ((MEM_DYN_USER_END_V - MEM_DYN_USER_START_V) / LARGE_PAGE_SIZE)

struct page_table_rounded_up {
	union Tile_PTE pt[HV_L2_ENTRIES]; // HV_L1_ENTRIES == HV_L2_ENTRIES
	uint8_t reserve[BASE_PAGE_SIZE - (HV_L2_ENTRIES * sizeof(union Tile_PTE))];
};

struct init_page_table_t {
	struct page_table_rounded_up l1_page_table;
	struct page_table_rounded_up l2_page_tables[BASE_PAGE_TABLE_NUM];
};

struct tilepro_elf_module {
	lvaddr_t binary_addr;
	uint32_t binary_size;
};

static struct init_page_table_t *init_page_table;
static struct spawn_state spawn_state;
static char cpu_args[128];

void print_stack_pointer(int tile_num);
void start_kernel(void);
struct dcb* start_first_core(HV_Topology topo, struct physical_mem_range *range);
void create_page_table_caps(void);
struct tilepro_elf_module create_module_caps(struct bootinfo* bi);
void tokenize_menulist_line(char* line, char* module, char* parameters);
void create_phys_caps(struct bootinfo* bi, struct physical_mem_range *range);


static inline void module_file_name(const char module_name[], char out_buffer[], int output_size) {
	//TILEmpower boot changes:
	//char module_start[] = "./";

	char module_start[] = "./tilepro/sbin/";
	int start_len = strlen(module_start) - 1; // No termination char
	assert(start_len <= output_size);
	strncpy(out_buffer, module_start, start_len);

	int mod_len = strlen(module_name);
	assert(mod_len <= output_size - start_len);
	strncpy(&out_buffer[start_len], module_name, mod_len);
}

void print_stack_pointer(int tile_num) {
	uint32_t stack_pointer;
	uint32_t link_register;

	__asm __volatile("move %[stack_pointer], sp"
			: [stack_pointer] "=r" (stack_pointer));
	__asm __volatile("move %[link_register], lr"
			: [link_register] "=r" (link_register));

	printf("[Tile %d initial SP] : 0x%.8lX\n", tile_num, stack_pointer);
	printf("[Tile %d initial LR] : 0x%.8lX\n", tile_num, link_register);
}

/*
 * map page tables under page cnode
 */
void create_page_table_caps(void) {
	int pagecn_num = 0;

	// create capability for L1 page table under pagecn
	lvaddr_t l1_addr = (lvaddr_t) init_page_table;
	caps_create_new(ObjType_VNode_tilepro_l1, mem_to_local_phys(l1_addr),
			vnode_objbits(ObjType_VNode_tilepro_l1), 0,
			caps_locate_slot(CNODE(spawn_state.pagecn), pagecn_num++));

	// create capability for L2 page tables under pagecn
	int size_l1_table = sizeof(init_page_table->l1_page_table);
	int size_l2_table = sizeof(init_page_table->l2_page_tables[0]);
	assert(size_l1_table == BASE_PAGE_SIZE);
	assert(size_l2_table == BASE_PAGE_SIZE);
	lvaddr_t l2_start_addr = l1_addr + size_l1_table;

	assert(!(MEM_DYN_USER_START_V & (LARGE_PAGE_SIZE - 1)));
	int init_l1_index_base = MEM_DYN_USER_START_V >> LARGE_PAGE_BITS;

	// An alternative would be to create one VNode for a L1 page table plus the L2 page tables
	for (int i = 0; i < BASE_PAGE_TABLE_NUM; i++) {
		// The address of the 'i'th L2 page table
		lvaddr_t l2_vaddr = l2_start_addr + size_l2_table * i;
		lpaddr_t l2_paddr = mem_to_local_phys(l2_vaddr);
		caps_create_new(ObjType_VNode_tilepro_l2, l2_paddr,
				vnode_objbits(ObjType_VNode_tilepro_l2), 0,
				caps_locate_slot(CNODE(spawn_state.pagecn), pagecn_num++));

		// map L2 page table into L1 page table entries
		//printf("Address of l2 page table (%d) to map: 0x%.8lX (va: 0x%.8lX)\n", i, l2_paddr, l2_vaddr);
		union Tile_PTE pte = create_l1_link_to_l2_pte(l2_paddr);
		// sanity test
		assert(init_page_table->l1_page_table.pt[init_l1_index_base+i].raw == 0);
		// Map it in the L1 page table
		init_page_table->l1_page_table.pt[init_l1_index_base + i] = pte;
	}
}

/**
 * Returns the inode for a file, fetched from the bootrom. Panic if it doesn't exist.
 *
 * @param file_name the name of the file to be resolved.
 * @param file_info output - Pointer to the structure, containing the size afterwards
 * @return the inode
 */
static int get_file_inode(char* file_name, HV_FS_StatInfo* file_info) {
	int inode = hv_fs_findfile((HV_VirtAddr) file_name);
	if (inode == HV_ENOENT) {
		tilepro_panic("ERROR! There is no such file: %s\n", file_name);
	} else if (inode == HV_EFAULT) {
		tilepro_panic("ERROR! Bad filename pointer: %s\n", file_name);
	} else {
		*file_info = hv_fs_fstat(inode);
	}
	return inode;
}

/**
 * Reads a file into a buffer. Panics on error.
 *
 * @param inode the inode to be read
 * @param file_info The structure, containing the size.
 * @param buffer output - Pointer to the buffer. Must be able to hold the whole size, described by file_info.size.
 */
static void read_file(int inode, HV_FS_StatInfo file_info, char* buffer) {
	int err = hv_fs_pread(inode, (HV_VirtAddr) buffer, file_info.size, 0);
	if (err == HV_EBADF) {
		tilepro_panic("ERROR! Reading file on a bad node: %u\n", inode);
	} else if (err == HV_EFAULT) {
		tilepro_panic("ERROR! Reading file to a bad buffer: 0x%.8lX\n",
				(uint32_t) buffer);
	}
	assert(file_info.size == err);
}

/*
 * Tokenize each menu.lst line, return module and parameters.
 *
 * @param line the line to be tokenized
 * @param module output - containing the module string. size must be >= of the size of line
 * @param parameters output - containing all parameters. size must be >= of the size of line
 */
void tokenize_menulist_line(char* line, char* module, char* parameters) {
	char* space = strchr(line, ' ');
	if (space == NULL) {
		strcpy(module, line);
		parameters[0] = '\0';
	} else {
		strncpy(module, line, (space - line));
		module[space - line] = '\0';
		strcpy(parameters, (space + 1));
	}
}

/*
 * read menu.lst and modules from bootrom file
 * for each of them, create a cnode under the module cnode
 *
 * @param bi, bootinfo for boot OS, need to be initialized somehow here
 * @return, the virtual address of "init" program, which will be executed first after kernel is ready
 */
struct tilepro_elf_module create_module_caps(struct bootinfo* bi) {
	// create capability for menu list
	lpaddr_t menulist_pa = tilepro_alloc_phys(BASE_PAGE_SIZE);

	printf("menulist_va : 0x%.8lX\n", local_phys_to_mem(menulist_pa));
	memset((void*) local_phys_to_mem(menulist_pa), 0, BASE_PAGE_SIZE);

	//printf("module cnode PA : 0x%.8lX\n", CNODE(spawn_state.modulecn));

	// Create cap for menu list in first slot of modulecn
	int err = caps_create_new(ObjType_Frame, menulist_pa, BASE_PAGE_BITS,
			BASE_PAGE_BITS,
			caps_locate_slot(CNODE(spawn_state.modulecn),
					spawn_state.modulecn_slot++));
	assert(err_is_ok(err));

	// find menu list file from bootrom
	printf("Reading menu list...\n");
	char menulist_file[] = "./tilepro/menu.lst.modules";
	HV_FS_StatInfo file_info;
	int inode = get_file_inode(menulist_file, &file_info);

	assert(file_info.size < BASE_PAGE_SIZE);
	lvaddr_t menulist_va = local_phys_to_mem(menulist_pa);

	// read menu list file from bootrom
	read_file(inode, file_info, (char*) menulist_va);
	printf("Menu list content : \n");
	printf("%s\n", (char*) menulist_va);

	((char*) menulist_va)[file_info.size] = '\0';

	// skip the first line, since we have created a capability for menu list
	strtok((char*) menulist_va, "\r\n");

	struct tilepro_elf_module init_module = {0, 0};

	char* line = NULL;
	// now we go through all modules, parsing the menu list line by line
	while ((line = strtok(NULL, "\t\n")) != 0) {

		//printf("%s\n", line);
		char module[strlen(line)];
		char parameters[strlen(line)];
		// tokenize each line of menu list, return module and parameters
		tokenize_menulist_line(line, module, parameters);

		printf("Loading module: %s %s\n", module, parameters);
		inode = get_file_inode(module, &file_info);

		// get how many small pages each module requests
		uint32_t small_pages = (file_info.size + BASE_PAGE_SIZE - 1)
				/ BASE_PAGE_SIZE;
		// allocate physical memory for each module
		lpaddr_t module_pa = tilepro_alloc_phys(file_info.size);
		lvaddr_t module_va = local_phys_to_mem(module_pa);

		// for each module, initialize mem_region
		struct mem_region* region = &bi->regions[bi->regions_length++];
		region->mr_base = module_pa;
		region->mr_type = RegionType_Module;
		region->mrmod_slot = spawn_state.modulecn_slot;
		region->mrmod_size = file_info.size;
		region->mrmod_data = (lvaddr_t)line - menulist_va;

		// check if this is "init" program
		if (!strcmp(module, INIT_MODULE_NAME)) {
			init_module.binary_addr = module_va;
			init_module.binary_size = file_info.size;
		}
		if (!strcmp(module, CPU_MODULE_NAME)) {
			int len = min(strlen(parameters), sizeof(cpu_args));
			memcpy(cpu_args, parameters, len);
			// End the string with a null. Matters, if cpu_args < parameters
			cpu_args[sizeof(cpu_args) - 1] = '\0';
		}

		read_file(inode, file_info, (char*) module_va);

		// create capability for each module
		for (int i = 0; i < small_pages; i++) {
			err = caps_create_new(ObjType_DevFrame, module_pa, BASE_PAGE_BITS,
					BASE_PAGE_BITS,
					caps_locate_slot(CNODE(spawn_state.modulecn),
							spawn_state.modulecn_slot++));
			assert(err_is_ok(err));
			module_pa += BASE_PAGE_SIZE;
		}
	}  // end of while-loop
	assert(init_module.binary_addr);
	assert(init_module.binary_size);
	return init_module;

}

/*
 * create unused ram capabilities.
 *
 * @param bi, bootinfo for boot OS, need to be initialized somehow here.
 */
void create_phys_caps(struct bootinfo* bi, struct physical_mem_range *range) {
	create_caps_to_cnode(range->start + range->size - range->bytes_free,
						 range->bytes_free,
						 RegionType_Empty,
						 &spawn_state,
						 bi);
}

/*
 * round down the number to the lower interval
 */
uint32_t round_down(uint32_t base, const uint32_t interval);
uint32_t round_down(uint32_t base, const uint32_t interval) {
	return (base / interval) * interval;
}

/*
 * round up the number to the upper interval
 */
uint32_t round_up(uint32_t base, const uint32_t interval);
uint32_t round_up(uint32_t base, const uint32_t interval) {
	return ((base + interval) / interval) * interval;
}

/*
 * this function is to creating mapping in init's L2 page tables.
 *
 * @param init_page_table, point to init's page tables.
 * @param va_base, virtual address to map.
 * @param pa_base, physical address to map.
 * @param size, size to map.
 * @param l2_flag, all properties of PTE to map.
 */
void init_user_space_l2_map(struct init_page_table_t *init_page_tables,
		lvaddr_t va_base, lpaddr_t pa_base, uint32_t size, uint32_t l2_flags);
void init_user_space_l2_map(struct init_page_table_t *init_page_tables,
		lvaddr_t va_base, lpaddr_t pa_base, uint32_t size, uint32_t l2_flags) {
	assert(va_base >= MEM_DYN_USER_START_V);
	assert(va_base < MEM_DYN_USER_END_V);
	assert((size % BASE_PAGE_SIZE) == 0);
	assert((va_base % BASE_PAGE_SIZE) == 0);

	for (int i = 0; i < (size / BASE_PAGE_SIZE); i++) {
		lpaddr_t pa = pa_base + i * BASE_PAGE_SIZE;
		union Tile_PTE pte = create_pte_according_to_elf_flags(pa, l2_flags);

		// va_entry is basically the virtual address / BASE_PAGE_SIZE
		int va_entry = i + ((va_base - MEM_DYN_USER_START_V) / BASE_PAGE_SIZE);
		// This behavior could also be done, by having a
		// union for l2_page_table, so that addressing with
		// va_entry would also be possible.
		int page_table_n = va_entry / HV_L2_ENTRIES;
		int pte_n = va_entry & (HV_L2_ENTRIES - 1);
		union Tile_PTE before =
				init_page_tables->l2_page_tables[page_table_n].pt[pte_n];
		assert(!before.raw);
		init_page_tables->l2_page_tables[page_table_n].pt[pte_n] = pte;
	}
	//print_page_mappings(init_page_tables->l1_page_table.pt);
}

static void init_bf_internal_l2_map(lvaddr_t va_base, lpaddr_t pa_base, uint32_t size, uint32_t l2_flags) {
	assert(va_base >= MEM_BF_INTERNAL_LARGE_V);
	assert(va_base < MEM_BF_INTERNAL_LARGE_V + LARGE_PAGE_SIZE);
	assert((size % BASE_PAGE_SIZE) == 0);
	assert((va_base % BASE_PAGE_SIZE) == 0);

	for (int i = 0; i < (size / BASE_PAGE_SIZE); i++) {
		lpaddr_t pa = pa_base + i * BASE_PAGE_SIZE;
		union Tile_PTE pte = create_pte_according_to_elf_flags(pa, l2_flags);

		// va_entry is basically the virtual address / BASE_PAGE_SIZE
		int va_entry = i + ((va_base - MEM_BF_INTERNAL_LARGE_V) / BASE_PAGE_SIZE);
		int cpu_id = __insn_mfspr(SPR_SYSTEM_SAVE_1_0);
		union Tile_PTE *l2 = (union Tile_PTE*)MEM_L2_PAGE_TABLE(cpu_id);
		assert(va_entry < HV_L2_ENTRIES);
		union Tile_PTE before = l2[va_entry];
		assert(!before.raw);
		l2[va_entry] = pte;
	}
	//print_page_mappings(init_page_table->l1_page_table.pt);
}

/*
 * allocate memory for init program when loading its ELF into RAM
 *
 * @param state, this pointer is to point a space to use later.
 * @param base, this is virtual address, where the mapping begins
 * @param size, the size of "init" program.
 * @param flag, setting for writing PTE.
 * @return start_va, return the virtual address where one init program starts.
 */
errval_t startup_alloc_init(void *state, genvaddr_t base, size_t size,
		uint32_t flags, void **init_start_va);
errval_t startup_alloc_init(void *state, genvaddr_t base, size_t size,
		uint32_t flags, void **init_start_va) {
	// get the init's L2 page tables base address pointer
	struct init_page_table_t *page_tables = (struct init_page_table_t*) state;

	// create a whole small-page-aligned area for "init" program
	lvaddr_t base_down = round_down((lvaddr_t) base, BASE_PAGE_SIZE);
	uint32_t offset = (lvaddr_t) base - base_down;
	lvaddr_t base_up = round_up((lvaddr_t) base + size, BASE_PAGE_SIZE);
	uint32_t init_size = base_up - base_down;
	assert(init_size > 0);

	// allocate the real physical memory space to the "init" program image
	lpaddr_t init_image_pa = tilepro_alloc_phys(init_size);

	// map to init L2 page table
	init_user_space_l2_map(page_tables, base_down, init_image_pa, init_size, flags);

	// then go to the real starting point of "init" program
	*init_start_va = (void*) (base_down + offset);

	return SYS_ERR_OK;
}

/*
 * load "init" program (or monitor) into RAM
 *
 * @param module_info, virtual base address and size of module after creating module capabilities.
 * @param init_entry_point, init entry point to return
 */
void load_init_image(struct tilepro_elf_module module, genvaddr_t* init_entry_point);
void load_init_image(struct tilepro_elf_module module, genvaddr_t* init_entry_point) {

	// load init into RAM
	errval_t err = elf_load(EM_TILEPRO, startup_alloc_init, init_page_table,
			module.binary_addr, module.binary_size, init_entry_point);
	printf("init image load is done\n");
	assert((uint32_t)init_entry_point >= MEM_DYN_USER_START_V);
	assert(err == 0);
}

void setup_init_page_table(void);
void setup_init_page_table(void) {
	// here to set up page tables for "init" program
	// allocate memory space for L1 and L2 page tables for "init"
	int pt_size = sizeof(struct init_page_table_t);
	lvaddr_t page_table_addr = local_phys_to_mem(tilepro_alloc_phys(pt_size));
	init_page_table = (struct init_page_table_t*) (page_table_addr);
	memset(init_page_table, 0, pt_size);
	printf("Virtual address of page tables for init : 0x%.8lX\n",
			(uint32_t) init_page_table);
}

static struct dcb *spawn_init_common(struct spawn_state *st, const char *name,
                                     int argc, const char *argv[],
                                     lpaddr_t bootinfo_phys,
                                     alloc_phys_func alloc_phys);
static struct dcb *spawn_init_common(struct spawn_state *st, const char *name,
                                     int argc, const char *argv[],
                                     lpaddr_t bootinfo_phys,
                                     alloc_phys_func alloc_phys) {
	printf("Calling spawn_module(...)...\n");
	lvaddr_t paramaddr;
	struct dcb *init_dcb = spawn_module(st, name, argc, argv, bootinfo_phys,
			 INIT_ARGS_VBASE, alloc_phys, &paramaddr);
	printf("spawn_module(...) completed.\n");
	printf("Creating page table caps...\n");
	create_page_table_caps();
	printf("Page table caps are done.\n");

	// initialize DCB's vspace attribute
	init_dcb->vspace = mem_to_local_phys((lvaddr_t) init_page_table);

	// generic part of dispatcher, architecture-independent
	struct dispatcher_shared_generic *disp_generic =
			get_dispatcher_shared_generic(init_dcb->disp);
	assert(disp_generic != NULL);
	// set up stack pointer
	disp_generic->udisp = INIT_DISPATCHER_VBASE;

	// architecture-dependent part of dispatcher
	struct dispatcher_shared_tilepro *disp_tilepro =
			get_dispatcher_shared_tilepro(init_dcb->disp);
	assert(disp_tilepro != NULL);

	// switch to the init L1 page table as the current working page table.
	// after this init's page tables will be officially available,
	// and the boot-up page table will be given up.
	paging_context_switch(mem_to_local_phys((lvaddr_t)init_page_table), init_dcb);

	/* then we map 3 parts, which is necessary for system to boot. */
	// map bootinfo
	init_bf_internal_l2_map(INIT_BOOTINFO_VBASE, bootinfo_phys, BOOTINFO_SIZE, 0);

	// map argument section
	init_bf_internal_l2_map(INIT_ARGS_VBASE, spawn_state.args_page, ARGS_SIZE, 0);

	// map dispatcher
	init_bf_internal_l2_map(INIT_DISPATCHER_VBASE, mem_to_local_phys(init_dcb->disp), DISPATCHER_SIZE, 0);

	// initialize enabled save area
	disp_tilepro->enabled_save_area.r0 = paramaddr;
	disp_tilepro->disabled_save_area.sp = MEM_USER_STACK_TOP;
	disp_tilepro->disabled_save_area.tp = INIT_DISPATCHER_VBASE;

	return init_dcb;
}

struct dcb* spawn_app_monitor(struct tilepro_core_data *tilepro_core_data, const char *name, alloc_phys_func alloc_phys);
struct dcb* spawn_app_monitor(struct tilepro_core_data *tilepro_core_data, const char *name, alloc_phys_func alloc_phys) {
	int cpu_id = __insn_mfspr(SPR_SYSTEM_SAVE_1_0);
	//struct single_core_data *core_data = &tilepro_core_data->per_core[cpu_id];
	//init_phys_mem_app_core(&tilepro_core_data->per_core[0].phys_mem, &core_data->phys_mem);

	//printf("cpu_id:%d chanid::%d\n",cpu_id,(int)core_data->chan_id);

	/* Construct cmdline args */
	// Core id of the core that booted this core
	//char coreidchar[10];
	//snprintf(coreidchar, sizeof(coreidchar), "%d", core_data->src_core_id);

	///aaa debug test
	for(int i=0;i<4;i++){
		struct single_core_data *core_data2 = &tilepro_core_data->per_core[i];
		printf("forloop i:%d cpu_id:%d chanid::%d\n",i,cpu_id,(int)core_data2->chan_id);

	}

	struct single_core_data *core_data = &tilepro_core_data->per_core[cpu_id];
	init_phys_mem_app_core(&tilepro_core_data->per_core[0].phys_mem, &core_data->phys_mem);

	printf("cpu_id:%d chanid::%d\n",cpu_id,(int)core_data->chan_id);

	char coreidchar[10];
	snprintf(coreidchar, sizeof(coreidchar), "%d", core_data->src_core_id);



	// IPI channel id of core that booted this core
	char chanidchar[30];
	snprintf(chanidchar, sizeof(chanidchar), "chanid=%"PRIu32,
			core_data->chan_id);

	// Arch id of the core that booted this core
	char archidchar[30];
	snprintf(archidchar, sizeof(archidchar), "archid=%d",
			core_data->src_arch_id);

	const char *argv[5] = {"monitor", coreidchar, chanidchar, archidchar };
	int argc = 4;

	setup_init_page_table();
	struct dcb *init_dcb = spawn_init_common(&spawn_state, name, argc, argv, 0, alloc_phys);

	genvaddr_t entry_point;
	printf("load_init_image\n");
	lvaddr_t monitor_va = local_phys_to_mem(gen_phys_to_local_phys(tilepro_core_data->monitor_binary));
	struct tilepro_elf_module monitor_module = {.binary_addr = monitor_va, .binary_size = tilepro_core_data->monitor_binary_size };
	load_init_image(monitor_module, &entry_point);
	printf("done\n");

	// allocate the stack for monitor
	lpaddr_t init_stack_pa = tilepro_alloc_phys(MEM_USER_STACK_SIZE);
	// map stack for init
	init_user_space_l2_map(init_page_table, MEM_USER_STACK_TOP - MEM_USER_STACK_SIZE, init_stack_pa, MEM_USER_STACK_SIZE, 0);

	struct dispatcher_shared_tilepro *init_disp_tilepro =
			get_dispatcher_shared_tilepro(init_dcb->disp);
	init_disp_tilepro->disabled_save_area.pc = entry_point;

	return init_dcb;
}

/*
 * this is the function to set up everything necessary for "init" program to run
 */
struct dcb* spawn_init(struct physical_mem_range *range);
struct dcb* spawn_init(struct physical_mem_range *range) {
	printf("Tile 0 is starting init...\n");
	setup_init_page_table();

	/* Allocate bootinfo */
	lpaddr_t bootinfo_phys = tilepro_alloc_phys(BOOTINFO_SIZE);
	memset((void *) local_phys_to_mem(bootinfo_phys), 0, BOOTINFO_SIZE);

	// Construct cmdline args
	char bootinfochar[16];
	snprintf(bootinfochar, sizeof(bootinfochar), "%u", INIT_BOOTINFO_VBASE);
	const char *argv[] = { "init", bootinfochar };

	struct dcb *init_dcb = spawn_init_common(&spawn_state, INIT_MODULE_NAME, ARRAY_LENGTH(argv), argv,
		                                             bootinfo_phys, tilepro_alloc_phys);

	// initialize bootinfo
	struct bootinfo* boot_info = (struct bootinfo*) INIT_BOOTINFO_VBASE;
	boot_info->regions_length = 0;

	struct tilepro_elf_module init_module;
	// create module capabilities based on menu list
	printf("Creating module caps...\n");
	init_module = create_module_caps(boot_info);
	printf("Module caps are done.\n");
	printf("init virtual address : 0x%.8lX\n", init_module.binary_addr);
	printf("Got command line: %s\n", cpu_args);
	static struct cmdarg cmdargs[] = {
	    { "loglevel",       ArgType_Int, { .integer = &kernel_loglevel }},
	    { "logmask",        ArgType_Int, { .integer = &kernel_log_subsystem_mask }},
	    {NULL, 0, {NULL}}
	};
	parse_commandline(cpu_args, cmdargs);

	// load init image
	genvaddr_t init_ip;
	load_init_image(init_module, &init_ip);
	assert(init_ip < (1LL << 32));
	printf("init entry point: 0x%.8lX\n", (lvaddr_t) init_ip);

	// allocate the stack for init
	lpaddr_t init_stack_pa = tilepro_alloc_phys(MEM_USER_STACK_SIZE);
	// map stack for init
	init_user_space_l2_map(init_page_table, MEM_USER_STACK_TOP - MEM_USER_STACK_SIZE, init_stack_pa, MEM_USER_STACK_SIZE, 0);

	//print_page_mappings(init_page_table->l1_page_table);

	struct dispatcher_shared_tilepro *disp_tilepro =
			get_dispatcher_shared_tilepro(init_dcb->disp);
	disp_tilepro->disabled_save_area.pc = init_ip;

	printf("Creating unused ram caps...\n");
	create_phys_caps(boot_info, range);
	printf("Unused RAM caps are done.\n");

	return init_dcb;
}

struct dcb* start_first_core(HV_Topology topo, struct physical_mem_range *first_core_range) {
	printf("Tile 0 is starting...\n");
	printf("[Tile width]  : %d\n", topo.width);
	printf("[Tile height] : %d\n", topo.height);

	struct dcb *init_dcb = spawn_init(first_core_range);
	printf("init kernel is ready.\n");

	return init_dcb;
}


void start_kernel(void) {
	HV_Topology topo = hv_inquire_topology();
	// get the tile ID
	int cpu_id = topo.coord.y * topo.width + topo.coord.x;
	assert(cpu_id == __insn_mfspr(SPR_SYSTEM_SAVE_1_0));
	my_core_id = cpu_id;

	printf("[Tile %d coordinate; BSP: %d] : (%d,%d)\n", cpu_id, arch_core_is_bsp(), topo.coord.x,
			topo.coord.y);
	print_stack_pointer(cpu_id);

	// assert that the information in mem_loc is the same like in this file
	// Needed because mem_loc doesn't rely on the barrelfish imports.
	assert(INIT_DISPATCHER_END == MEM_BF_INT_BARRELFISH_PART_END_V);

	init_virt_mem();

	// initialize time stamp counter (cycle counter), setting it able to be accessed in user-mode
	__insn_mtspr(SPR_MPL_WORLD_ACCESS_SET_0, 1);

    // enable special purpose register to use in user code
	__insn_mtspr(SPR_MPL_INTCTRL_0_SET_0, 1);

	__insn_mtspr(SPR_MPL_UDN_ACCESS_SET_0, 1);  // enable UDN network
	__insn_mtspr(SPR_UDN_TAG_0, STARTUP_CORE_TAG);  // enable tag 0 for spawning other cores

	// We have different imports (one in the non-kernel section) and need to ensure, these are matching.
	assert(MEM_BF_INT_MULTIBOOT_DATA_V == CORE_DATA_ADDRESS);
	struct tilepro_core_data *core_data = (struct tilepro_core_data*)MEM_BF_INT_MULTIBOOT_DATA_V;

	struct dcb *first_dcb;
	if (cpu_id == 0) {
		struct physical_mem_range *first_core_range = NULL;
		*core_data = init_phys_mem_bsp_core(&first_core_range);

		first_dcb = start_first_core(topo, first_core_range);

		// start up other cores
		hv_start_all_tiles();
	} else {
		printf("Core %d waiting for startup\n", cpu_id);
		uint32_t msg = udn0_receive();
		assert(((msg >> 24) & 0xFF) == 'b');
		assert(((msg >> 16) & 0xFF) == 'o');
		assert(((msg >>  8) & 0xFF) == 'o');
		assert(((msg >>  0) & 0xFF) == 't');
		uint32_t entry_most = udn0_receive();
		uint32_t entry_least = udn0_receive();
		assert(entry_most == 0);
		assert(entry_least == 0);
		int len = 2; // tag + answer
		uint32_t header = fill_dynamic_header(0 /* x */, 0 /* y */, 0, len);
		udn_send(header);
		udn_send(STARTUP_CORE_TAG);
		printf("Core %d received startup: %.8lX\n", cpu_id, msg);
		uint32_t answer = 'o' << 24 | 'k' << 16 | cpu_id;
		udn_send(answer);

		if (core_data->n_data <= cpu_id) {
			printf("No memory for core %d!\n", cpu_id);
			halt();
		}
		first_dcb = spawn_app_monitor(core_data, "monitor", tilepro_alloc_phys);
	}

	// Timer doesn't work at the moment :(
	// init_tile_timer();
	printf("dispatch it baby!\n");
	dispatch(first_dcb);

	panic("dispatch(first_dcb) returned!\n");
}

