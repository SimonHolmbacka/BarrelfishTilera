/**
 * \file
 * \brief Arch-specific inter-monitor communication
 */

/*
 * Copyright (c) 2009, 2010, ETH Zurich.
 * All rights reserved.
 *
 * This file is distributed under the terms in the attached LICENSE file.
 * If you do not find this file, copies can be found by writing to:
 * ETH Zurich D-INFK, Haldeneggsteig 4, CH-8092 Zurich. Attn: Systems Group.
 */

#include <inttypes.h>
#include "monitor.h"
#include <trace/trace.h>
#include <barrelfish/domain.h>
#include <assert.h>


struct bind_monitor_reply_state {
    struct intermon_msg_queue_elem elem;
    struct intermon_binding *orig_binding;
    errval_t err;
};

static void send_bind_monitor_reply(struct intermon_binding *b, errval_t err);

static void send_bind_monitor_reply_cont(struct intermon_binding *b,
                                         struct intermon_msg_queue_elem *e)
{
    struct bind_monitor_reply_state *st = (struct bind_monitor_reply_state *)e;
    send_bind_monitor_reply(st->orig_binding, st->err);
    free(st);
}

static void send_bind_monitor_reply(struct intermon_binding *b, errval_t err)
{
    errval_t err2 = b->tx_vtbl.bind_monitor_reply(b, NOP_CONT, err);
    if (err_is_fail(err2)) {
        if (err_no(err2) == FLOUNDER_ERR_TX_BUSY) {
            struct intermon_state *is = b->st;
            struct bind_monitor_reply_state *st = malloc(sizeof(*st));
            assert(st != NULL);

            st->orig_binding = b;
            st->elem.cont = send_bind_monitor_reply_cont;
            st->err = err;

            err2 = intermon_enqueue_send(b, &is->queue,
                                         get_default_waitset(), &st->elem.queue);
            assert(err_is_ok(err2));

        } else {
            DEBUG_ERR(err2, "reply failed");
        }
    }
}




/**
 * \brief A monitor receives request to setup a connection
 * with another newly booted monitor from a third monitor
 */
//work in progress
static void bind_monitor_request(struct intermon_binding *b,
                                 coreid_t core_id,
                                 intermon_caprep_t caprep)
{
	printf("\ninter.c bind_monitor_request my_core_id:%d core_id:%d\n",(int)my_core_id,(int)core_id);

	errval_t err;

	// Create the cap
	struct capability cap_raw;
	caprep_to_capability(&caprep, &cap_raw);
	if (cap_raw.type != ObjType_Frame) {
		err = MON_ERR_WRONG_CAP_TYPE;
	    goto error;
	}

	struct capref frame;
	err = slot_alloc(&frame);
	if (err_is_fail(err)) {
		goto error;
	}

	err = monitor_cap_create(frame, &cap_raw, core_id);
	if (err_is_fail(err)) {
		goto error;
	}

	// Setup the connection
	void *buf;
	err = vspace_map_one_frame(&buf, MON_URPC_SIZE, frame, NULL, NULL);
	if (err_is_fail(err)) {
		err = err_push(err, LIB_ERR_VSPACE_MAP);
		goto error;
	}




////////////////////////////////////////////////////////////////////////////////////////////////////
	//UDN part not completed

#if defined (CONFIG_INTERCONNECT_DRIVER_UDN)


	// setup our side of the binding
	struct intermon_udn_binding *udnb;
	udnb = malloc(sizeof(struct intermon_udn_binding));
	assert(udnb != NULL);

	uint8_t dest_mon_asid=2;
    coreid_t src_core_id = core_id; //test value not sure how to fix ///// core_id
    uint32_t out_chan_id = get_new_in_chan_id(); //test value not sure if this works correctly



    printf("2my_core_id: %d get_new_in_chan: %d\n",(int)my_core_id,(int)get_new_in_chan_id());

/*
    if(my_core_id==2){
    	if(core_id==1){
    		out_chan_id = 2;
    	}
    	if(core_id==3){
    		out_chan_id = 4;
    	}
    }
    if(my_core_id==3){
    	if(core_id==1){
    	    out_chan_id = 4;
    	}
    	if(core_id==2){
    		out_chan_id = 4;
    	}
    }*/
    printf("bindcores2 core_id:%d my_core_id:%d out_chan_id:%d\n",(int)core_id,(int)my_core_id,(int)out_chan_id);



//err = intermon_udn_init(udn_b, get_default_waitset(), src_core_id, out_chan_id, dest_mon_asid);


	err = intermon_udn_init(udnb, get_default_waitset(),src_core_id, out_chan_id, dest_mon_asid);
	assert(err_is_ok(err));

	err= intermon_init(&udnb->b,core_id);
	/* Send reply */



	//UMP part
#elif defined (CONFIG_INTERCONNECT_DRIVER_UMP)
    // setup our side of the binding
    struct intermon_ump_binding *umpb;
    umpb = malloc(sizeof(struct intermon_ump_binding));
    assert(umpb != NULL);

    err = intermon_ump_init(umpb, get_default_waitset(),
                            (char *)buf + MON_URPC_CHANNEL_LEN,
                            MON_URPC_CHANNEL_LEN,
                            buf, MON_URPC_CHANNEL_LEN);
    assert(err_is_ok(err));

    // Identify UMP frame for tracing
    umpb->ump_state.chan.sendid = (uintptr_t)cap_raw.u.frame.base;
    umpb->ump_state.chan.recvid =
        (uintptr_t)(cap_raw.u.frame.base + MON_URPC_CHANNEL_LEN);

    // connect it to our request handlers
    err = intermon_init(&umpb->b, core_id);

#endif // UDN / UMP


	assert(err_is_ok(err));

	reply:
	    send_bind_monitor_reply(b,err);
	    return;

	error:
	    // FIXME: cleanup!
	    goto reply;



	//assert(!"implement me");
	//core_id =
	//caprep =



}

/**
 * \brief The monitor that proxied the request for one monitor to
 * setup a connection with another monitor gets the reply
 */
//work in progress, runs only on monitor.0
static void bind_monitor_reply(struct intermon_binding *closure, errval_t err)
{
	printf("\ninter.c bind_monitor_reply my_core_id:%d seen_connections: %d\n",(int)my_core_id,(int)seen_connections);

    if (err_is_fail(err)) {
        DEBUG_ERR(err, "Got error in bind monitor reply");
    }
    seen_connections++;
	//assert(!"implement me\n");
}

/**
 * \brief A monitor asks this monitor to proxy
 * its request to bind to another monitor
 */

//work in progress

/* ---------------------- BIND_MONITOR_PROXY CODE START --------------------- */
struct bind_monitor_proxy_state {
    struct intermon_msg_queue_elem elem;
    struct intermon_binding *orig_binding;
    coreid_t dst_core_id;
    intermon_caprep_t caprep;
};

static void bind_monitor_proxy(struct intermon_binding *b,
                               coreid_t dst_core_id,
                               intermon_caprep_t caprep);

static void bind_monitor_proxy_cont(struct intermon_binding *b,
                                    struct intermon_msg_queue_elem *e)
{
    struct bind_monitor_proxy_state *st = (struct bind_monitor_proxy_state*)e;
    bind_monitor_proxy(st->orig_binding, st->dst_core_id, st->caprep);
    free(st);
}



//change *st to *b
static void bind_monitor_proxy(struct intermon_binding *b,
                               coreid_t dst_core_id,
                               intermon_caprep_t caprep)
{
	printf("\ninter.c bind_monitor_proxy my_core_id:%d\n",(int)my_core_id);


	errval_t err;
	/* Get source monitor's core id */
	coreid_t src_core_id = ((struct intermon_state *)b->st)->core_id;

	 /* Get destination monitor */
	struct intermon_binding *dst_binding = NULL;
	err = intermon_binding_get(dst_core_id, &dst_binding);
	if (err_is_fail(err)) {
		DEBUG_ERR(err, "intermon_binding_get failed");
	}

	// Proxy the request
	err = dst_binding->tx_vtbl.
	bind_monitor_request(dst_binding, NOP_CONT, src_core_id,caprep);

	if (err_is_fail(err)) {
		if (err_no(err) == FLOUNDER_ERR_TX_BUSY) {
			struct intermon_state *is = dst_binding->st;
	        struct bind_monitor_proxy_state *st = malloc(sizeof(struct bind_monitor_proxy_state));
	        assert(st);

	        st->orig_binding = b;
	        st->elem.cont = bind_monitor_proxy_cont;
	        st->dst_core_id = dst_core_id;
	        st->caprep = caprep;

	        err = intermon_enqueue_send(dst_binding,&is->queue,get_default_waitset(), &st->elem.queue);
	        assert(err_is_ok(err));

	    } else {
	    	DEBUG_ERR(err, "forwarding bind request failed");
	    }
	}


	//assert(!"implement me\n"); //yes
}

/**
 * \brief Notification of a newly booted monitor.
 *  Setup our connection and request the sender to proxy
 *  the bind request to the monitor
 */
//work in progress
static void new_monitor_notify(struct intermon_binding *st,
                               coreid_t core_id)
{
	printf("inter.c new_monitor_notify my_core_id:%d function got core_id:%d\n",(int)my_core_id,(int)core_id);

	//new code start
	errval_t err;

	/* Setup the connection */
	struct capref frame;

    err = frame_alloc(&frame, MON_URPC_SIZE, NULL);
    if (err_is_fail(err)) {
        DEBUG_ERR(err, "frame_alloc failed");
        return; // FIXME: cleanup
    }

#if defined (CONFIG_INTERCONNECT_DRIVER_UDN)
    // init our end of the binding and channel
    struct intermon_udn_binding *udn_b = malloc(sizeof(struct intermon_udn_binding));
    assert(udn_b != NULL);

    //missing variable set
    //printf("1my_core_id: %d get_new_in_chan: %d\n",(int)my_core_id,(int)get_new_in_chan_id());

    uint32_t out_chan_id = get_new_in_chan_id();
/*
    if(my_core_id==1){
    	if(core_id==2){
    		out_chan_id = 2;
    	}
    	if(core_id==3){
    		out_chan_id = 4;
    	}
    }
    if(my_core_id==2){
    	if(core_id==3){
    		out_chan_id = 4;
    	}
    }*/

	coreid_t src_core_id = my_core_id; //test value not sure how to fix
    //uint32_t out_chan_id = 2; //test value not sure how to fix

    uint8_t dest_mon_asid=2;

    printf("bindcores1 core_id:%d my_core_id:%d out_chan_id:%d\n",(int)core_id,(int)my_core_id,(int)out_chan_id);

	err = intermon_udn_init(udn_b, get_default_waitset(), src_core_id, out_chan_id, dest_mon_asid);

	assert(err_is_ok(err));
	/*
	if(err_is_fail(err)) {
		return err_push(err, LIB_ERR_UDN_CHAN_BIND);
	}*/
	/*
    struct capref ep, my_notify_cap;
    struct lmp_endpoint *iep;

	int chanid;
	err = endpoint_create(LMP_RECV_LENGTH, &ep, &iep);
	assert(err_is_ok(err));
	err = notification_allocate(ep, &chanid);
	assert(err == SYS_ERR_OK);
	err = notification_create_cap(chanid, my_core_id, &my_notify_cap);
	assert(err == SYS_ERR_OK);

	printf("inter.c chanid_:%d",chanid);

	*/


    //missing block frame for tracing maybe missing :) ?


    err = intermon_init(&udn_b->b, core_id);
    assert(err_is_ok(err));

    /* Identify the frame cap */
    struct capability frame_cap;
    err = monitor_cap_identify(frame, &frame_cap);
    if (err_is_fail(err)) {
        DEBUG_ERR(err, "monitor_cap_identify failed");
        return; // FIXME: cleanup
    }

    intermon_caprep_t caprep;
    capability_to_caprep(&frame_cap, &caprep);


	//last block
	/* reply to the sending monitor to proxy request */
	err = st->tx_vtbl.bind_monitor_proxy(st, NOP_CONT, core_id, caprep);
	if (err_is_fail(err)) {
		DEBUG_ERR(err, "bind proxy request failed");
	}


#elif defined (CONFIG_INTERCONNECT_DRIVER_UMP)
    //new code end
	//assert(!"implement me\n");

    errval_t err;

    /* Setup the connection */
    struct capref frame;
    err = frame_alloc(&frame, MON_URPC_SIZE, NULL);
    if (err_is_fail(err)) {
        DEBUG_ERR(err, "frame_alloc failed");
        return; // FIXME: cleanup
    }

	    void *buf;
	    err = vspace_map_one_frame_attr(&buf, MON_URPC_SIZE, frame, VREGION_FLAGS_READ_WRITE_NOCACHE, NULL, NULL);
	    if (err_is_fail(err)) {
	        DEBUG_ERR(err, "vspace_map_one_frame failed");
	        assert(buf); // XXX
	    }


	    // init our end of the binding and channel
	    struct intermon_ump_ipi_binding *ump_binding = malloc(sizeof(struct intermon_ump_ipi_binding));
	    assert(ump_binding != NULL);
	    err = intermon_ump_init(ump_binding, get_default_waitset(),
	                            buf, MON_URPC_CHANNEL_LEN,
	                            (char *)buf + MON_URPC_CHANNEL_LEN,
	                            MON_URPC_CHANNEL_LEN);
	    assert(err_is_ok(err));


	    // Identify UMP frame for tracing
	    struct frame_identity umpid = { .base = 0, .bits = 0 };
	    err = invoke_frame_identify(frame, &umpid);
	    assert(err_is_ok(err));
	    ump_binding->ump_state.chan.recvid = (uintptr_t)umpid.base;
	    ump_binding->ump_state.chan.sendid =
	        (uintptr_t)(umpid.base + MON_URPC_CHANNEL_LEN);

	    err = intermon_init(&ump_binding->b, core_id);
	    assert(err_is_ok(err));

	    // Identify the frame cap
	    struct capability frame_cap;
	    err = monitor_cap_identify(frame, &frame_cap);
	    if (err_is_fail(err)) {
	        DEBUG_ERR(err, "monitor_cap_identify failed");
	        return; // FIXME: cleanup
	    }

	    intermon_caprep_t caprep;
	    capability_to_caprep(&frame_cap, &caprep);

	    // reply to the sending monitor to proxy request
	    err = b->tx_vtbl.bind_monitor_proxy_scc(b, NOP_CONT, core_id,
	                                            caprep, chanid, my_core_id);
	    if (err_is_fail(err)) {
	        DEBUG_ERR(err, "bind proxy request failed");
	    }


#endif // UDN / UMP

}

errval_t arch_intermon_init(struct intermon_binding *b)
{
	printf("\ninter.c arch_intermon_init my_core_id:%d\n",(int)my_core_id);

    b->rx_vtbl.bind_monitor_request = bind_monitor_request;
    b->rx_vtbl.bind_monitor_reply = bind_monitor_reply;
    b->rx_vtbl.bind_monitor_proxy = bind_monitor_proxy;
    b->rx_vtbl.new_monitor_notify = new_monitor_notify;

    return SYS_ERR_OK;
}
