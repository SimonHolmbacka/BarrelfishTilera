/**
 * \file
 * \brief Code for handling booting additional cores
 */

/*
 * Copyright (c) 2010, ETH Zurich.
 * All rights reserved.
 *
 * This file is distributed under the terms in the attached LICENSE file.
 * If you do not find this file, copies can be found by writing to:
 * ETH Zurich D-INFK, Haldeneggsteig 4, CH-8092 Zurich. Attn: Systems Group.
 */

#include "monitor.h"
#include <inttypes.h>
#include <elf/elf.h>
#include <barrelfish_kpi/tilepro_core_data.h>

errval_t spawn_xcore_monitor(coreid_t coreid, int hwid, enum cpu_type cpu_type, const char* cmdline, struct intermon_binding **ret_binding)
{
	errval_t err;

	// Get my arch ID
	uintptr_t my_arch_id = 0;
	err = invoke_monitor_get_arch_id(&my_arch_id);
	assert(err == SYS_ERR_OK);

	printf("\n aa my_core_id: %d coreid:%d asid:%d\n",(int)my_core_id,(int)coreid,(int)get_asid());

    struct tilepro_core_data *core_data = (struct tilepro_core_data*)CORE_DATA_ADDRESS;
    assert(core_data->n_data >= (coreid + 1));
	core_data->per_core[coreid].src_core_id       = my_core_id;
	core_data->per_core[coreid].src_arch_id       = my_arch_id;
	core_data->per_core[coreid].dst_core_id       = coreid;

	//TILEmpower boot changes:
	//struct mem_region *monitor_region = multiboot_find_module(bi, "monitor");
	struct mem_region *monitor_region = multiboot_find_module(bi, "tilepro/sbin/monitor");
	core_data->monitor_binary = monitor_region->mr_base;
	core_data->monitor_binary_size = monitor_region->mrmod_size;

	core_data->per_core[coreid].chan_id           = -2; // Something to detect errors.

#if defined (CONFIG_INTERCONNECT_DRIVER_UDN)

	// Setup new inter-monitor connection to ourselves, using udn network
	struct intermon_udn_binding *udn_binding = malloc(sizeof(struct intermon_udn_binding));
	assert(udn_binding != NULL);

	// The app_core_in_chan_id is 0, because the other core will have a newly created UDN.
	uint32_t app_core_in_chan_id = 0;
	uint32_t bsp_core_out_chan_id = app_core_in_chan_id;
	uint8_t dest_mon_asid = 1;  // here we know the asid of monitor on other core is 1.
	assert(get_asid() == 2);  // we know the asid of monitor on the first core is 2, sanity check.


	printf("aa intermon udn init on core: %d\n",(int)my_core_id);

	err = intermon_udn_init(udn_binding, get_default_waitset(), coreid, bsp_core_out_chan_id, dest_mon_asid);
    if(err_is_fail(err)) {
    	return err_push(err, LIB_ERR_UDN_CHAN_BIND);
    }
    struct flounder_udn_state udn_state = udn_binding->udn_state;
    uint32_t bsp_core_in_chan = udn_state.chan.endpoint.in_chan.chan_id;
    uint32_t app_core_out_chan = bsp_core_in_chan;
    core_data->per_core[coreid].chan_id = app_core_out_chan;

    *ret_binding = &udn_binding->b;

#elif defined (CONFIG_INTERCONNECT_DRIVER_UMP)
    // This code is a stub and not working.
    assert(!"Implement in a real way\n");

    struct intermon_ump_binding *ump_binding = malloc(sizeof(struct intermon_ump_binding));

    assert(ump_binding != NULL);

    printf("Alloc first frame\n");
	// compute size of frame needed and allocate it
	struct capref frame;
	size_t framesize = MON_URPC_CHANNEL_LEN * 2;
	err = frame_alloc(&frame, framesize, &framesize);
	if (err_is_fail(err)) {
		return err_push(err, LIB_ERR_FRAME_ALLOC);
	}

	// Mark it remote
	bool has_descendants;
	err = monitor_cap_remote(frame, true, &has_descendants);
	if (err_is_fail(err)) {
		return err;
	}

	printf("Map it\n");
	// map it in
	void *buf;
	err = vspace_map_one_frame(&buf, framesize, frame, NULL, NULL);
	if (err_is_fail(err)) {
		cap_destroy(frame);
		return err_push(err, LIB_ERR_VSPACE_MAP);
	}

	printf("intermon_ump_init\n");
    err = intermon_ump_init(ump_binding, get_default_waitset(),
                            buf, MON_URPC_CHANNEL_LEN,
                            (char *)buf + MON_URPC_CHANNEL_LEN,
                            MON_URPC_CHANNEL_LEN);

    if (err_is_fail(err)) {
		cap_destroy(frame);
		return err_push(err, LIB_ERR_UMP_CHAN_BIND);
	}

	*ret_binding = &ump_binding->b;

	// Identify UMP frame for tracing
	struct frame_identity umpid;
	err = invoke_frame_identify(frame, &umpid);
	assert(err_is_ok(err));
	ump_binding->ump_state.chan.recvid = (uintptr_t)umpid.base;
	ump_binding->ump_state.chan.sendid =
		(uintptr_t)(umpid.base + MON_URPC_CHANNEL_LEN);

	/* Memory for core_data */
    size_t core_data_size = BASE_PAGE_SIZE;
    struct capref core_data_cap;

    /* Currently, the app kernel can only be loaded in the first 4GB
       of memory. Further, it must not overlap the integer
       boundaries, i.e. 0-1, 1-2, 2-3, or 3-4. */
    printf("frame_alloc loop\n");
    uint64_t old_minbase;
    uint64_t old_maxlimit;
    ram_get_affinity(&old_minbase, &old_maxlimit);

    for (uint64_t minbase = 0, maxlimit = (uint64_t)1 << 30;
         minbase < (uint64_t)4 << 30;
         minbase += (uint64_t)1 << 30, maxlimit += (uint64_t)1 << 30) {

        ram_set_affinity(minbase, maxlimit);
        err = frame_alloc(&core_data_cap, core_data_size, &core_data_size);
        if (err_is_fail(err)) {
            continue;
        } else {
            goto done;
        }
    }

    USER_PANIC("No memory in the first 4GB, cannot continue booting cores");

 done:
 	 printf("cap_remote\n");
    // Mark memory as remote
    err = monitor_cap_remote(core_data_cap, true, &has_descendants);
    if (err_is_fail(err)) {
        return err;
    }
    printf("Map it\n");
    void *core_data_buf;
    err = vspace_map_one_frame(&core_data_buf, core_data_size, core_data_cap, NULL, NULL);
    if(err_is_fail(err)) {
        return err_push(err, LIB_ERR_VSPACE_MAP);
    }
    printf("Look up\n");
	/* Look up information on the urpc_frame cap */
	struct frame_identity urpc_frame_id;
	err = invoke_frame_identify(frame, &urpc_frame_id);
	if (err_is_fail(err)) {
		USER_PANIC_ERR(err, "frame_identify failed");
	}

    core_data->per_core[coreid].urpc_frame_base = urpc_frame_id.base;
	core_data->per_core[coreid].urpc_frame_bits = urpc_frame_id.bits;


#endif // UDN / UMP


    /* Invoke kernel capability to boot new core */
    // here the third argument is entry point of monitor,
    // but we set it 0, because we don't use it in our case,
    // this syscall is just to send a udn message to start up according cores,
    // the entry point will be set up in startup_tilepro.c
    err = invoke_monitor_spawn_core(coreid, cpu_type, (forvaddr_t)0);
    if(err_is_fail(err)) {
    	return err_push(err, MON_ERR_SPAWN_CORE);
    }

#if defined (CONFIG_INTERCONNECT_DRIVER_UMP)
    /* Clean up */ // XXX: Should not delete the remote cap
    err = cap_destroy(core_data_cap);
    if (err_is_fail(err)) {
        USER_PANIC_ERR(err, "cap_destroy failed");
    }
#endif // CONFIG_INTERCONNECT_DRIVER_UMP

    return SYS_ERR_OK;
}

/**
 * \brief Initialize monitor running on app cores
 */
errval_t boot_arch_app_core(int argc, char *argv[], coreid_t *ret_parent_coreid, struct intermon_binding **ret_binding)
{
	errval_t err;
	assert(argc == 4);

    printf("boot_arch_app_core argc value: %d\n",argc);
	printf("monitor: invoked as:");
    for (int i = 0; i < argc; i++) {
        printf(" %s", argv[i]);
    }
    printf("\n");

	// core_id of the core that booted this core
	//coreid_t src_core_id = strtol(argv[1], NULL, 10);

	int offset1=strlen("monitor ");
	coreid_t src_core_id=atoi(argv[1] + offset1);

	*ret_parent_coreid = src_core_id;


	printf("aa core id of the core that booted: %d \n",(int)src_core_id);

#if defined(CONFIG_INTERCONNECT_DRIVER_UDN)

	struct intermon_udn_binding *udn_b = malloc(sizeof(struct intermon_udn_binding));
	assert(udn_b != NULL);

	//uint32_t out_chan_id = strtol(argv[2], NULL, 10);
	//in this case strtol() dosent work the correct way

	int offset2=strlen("chanid=");
	uint32_t out_chan_id=atoi(argv[2] + offset2);

	printf("outchan_id %d\n",(int)out_chan_id);



	uint8_t dest_mon_asid = 2;  // we know the asid of monitor on the first core is 2
	assert(get_asid() == 1);  // we know the asid of monitor on app cores is 1, sanity check.

	printf("aa intermon udn init2 on core: %d src_core_id: %d out_chan_id:%d\n",(int)my_core_id,(int)src_core_id,(int)out_chan_id);

	err = intermon_udn_init(udn_b, get_default_waitset(), src_core_id, out_chan_id, dest_mon_asid);
	if(err_is_fail(err)) {
		return err_push(err, LIB_ERR_UDN_CHAN_BIND);
	}

	*ret_binding = &udn_b->b;

#elif defined (CONFIG_INTERCONNECT_DRIVER_UMP)
   // check that the frame is big enough
	struct capref frame = {
		.cnode = cnode_task,
		.slot  = TASKCN_SLOT_MON_URPC,
	};
	struct frame_identity frameid;
	err = invoke_frame_identify(frame, &frameid);
	if (err_is_fail(err)) {
		err = err_push(err, LIB_ERR_FRAME_IDENTIFY);
		return err;
	}

	size_t framesize = ((uintptr_t)1) << frameid.bits;
	if (framesize < 2 * MON_URPC_CHANNEL_LEN) {
		return LIB_ERR_UMP_FRAME_OVERFLOW;
	}

    // map it in
    void *buf;
    err = vspace_map_one_frame(&buf, framesize, frame, NULL, NULL);
    if (err_is_fail(err)) {
        err = err_push(err, LIB_ERR_VSPACE_MAP);
        return err;
    }
    struct intermon_ump_binding *umpb;
    umpb = malloc(sizeof(struct intermon_ump_binding));
    assert(umpb != NULL);

    err = intermon_ump_init(umpb, get_default_waitset(),
                            (char *)buf + MON_URPC_CHANNEL_LEN,
                            MON_URPC_CHANNEL_LEN,
                            buf, MON_URPC_CHANNEL_LEN);

    if (err_is_fail(err)) {
        err = err_push(err, LIB_ERR_UMP_CHAN_BIND);
        return err;
    }

    // Identify UMP frame for tracing
    umpb->ump_state.chan.sendid = (uintptr_t)frameid.base;
    umpb->ump_state.chan.recvid =
        (uintptr_t)(frameid.base + MON_URPC_CHANNEL_LEN);

    *ret_binding = &umpb->b;

    return SYS_ERR_OK;
#endif // UDN / UMP

    return SYS_ERR_OK;
}
