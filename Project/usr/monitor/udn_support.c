/*
 * udn_support.c
 *
 *  Created on: Feb 11, 2013
 *      Author: jackie
 */

#include "monitor.h"
#include <barrelfish/udn_impl.h>

static void bind_udn_reply_client_handler(struct monitor_binding *b, struct monitor_msg_queue_elem *e);
static void bind_udn_reply_handler(struct intermon_binding *b, struct intermon_msg_queue_elem *e);
static void bind_udn_service_request_handler(struct monitor_binding *b, struct monitor_msg_queue_elem *e);
static void bind_udn_request_handler(struct intermon_binding *b, struct intermon_msg_queue_elem *e);

static void monitor_bind_udn_client_request_error(struct monitor_binding *b, uintptr_t conn_id, uintptr_t domain_id, errval_t err)
{
    errval_t err2;

    if(conn_id != 0) {
    	err2 = remote_conn_free(conn_id);
        if(err_is_fail(err2)) {
        	USER_PANIC_ERR(err2, "remote_conn_free failed");
        }
    }

    // in this case, it fails to send request to remote server, so just send wrong numbers back
    err2 = b->tx_vtbl.bind_udn_reply_client(b, NOP_CONT, 0, domain_id, err, NULL_CAP, -1, -1, -1);
    if(err_is_fail(err2)) {
    	USER_PANIC_ERR(err2, "error reply failed");
    }
}

struct bind_udn_request_state {
	struct intermon_msg_queue_elem elem;
    struct intermon_bind_udn_request__args args;
    struct capability capability;
    struct monitor_binding *mb;
    uintptr_t domain_id;
};

static void bind_udn_request_cont(struct intermon_binding *intermon_binding,
		                              iref_t iref, uintptr_t conn_id,
		                              struct capability capability,
		                              struct monitor_binding *mb,
		                              uintptr_t domain_id,
		                              coreid_t core_id,
		                              uint32_t chan_id,
		                              uint8_t asid)
{
    errval_t err;

    intermon_caprep_t caprep;
    capability_to_caprep(&capability, &caprep);

    /* Send the request to the monitor on the server's core */
    // here we send our client core id and input channel id to the server's monitor on another core
    err = intermon_binding->tx_vtbl.bind_udn_request(intermon_binding, NOP_CONT, iref, conn_id, caprep, core_id, chan_id, asid);
    if(err_is_fail(err)) {
    	if(err_no(err) == FLOUNDER_ERR_TX_BUSY) {
            struct bind_udn_request_state *me = malloc(sizeof(struct bind_udn_request_state));
            struct intermon_state *ist = intermon_binding->st;

            me->args.iref = iref;
            me->args.mon_id = conn_id;
            me->args.core_id = core_id;
            me->args.chan_id = chan_id;
            me->args.asid = asid;
            me->capability = capability;
            me->mb = mb;
            me->domain_id = domain_id;
            me->elem.cont = bind_udn_request_handler;

            err = intermon_enqueue_send(intermon_binding, &ist->queue, get_default_waitset(), &me->elem.queue);
            assert(err_is_ok(err));
            return;
        }

        USER_PANIC_ERR(err, "failed forwarding UDN bind request");
        monitor_bind_udn_client_request_error(mb, conn_id, domain_id, err);
    }
}

static void bind_udn_request_handler(struct intermon_binding *b, struct intermon_msg_queue_elem *e)
{
	struct bind_udn_request_state *st = (struct bind_udn_request_state *)e;
    bind_udn_request_cont(b, st->args.iref, st->args.mon_id, st->capability, st->mb, st->domain_id, st->args.core_id, st->args.chan_id, st->args.asid);
    free(e);
}

/*
 * this function is to send client's binding request to the local monitor,
 * here we want to send client's core id and input channel id to the remote server.
 */
static void monitor_bind_udn_client_request(struct monitor_binding *mb,
												  iref_t iref,
												  uintptr_t domain_id,
												  struct capref notify,
												  uintptr_t client_core_id,
												  uintptr_t client_in_chan_id,
												  uintptr_t client_asid)
{
    uint8_t core_id;
    uintptr_t conn_id = 0;
    errval_t err;
    struct remote_conn_state *conn = NULL;

	// Get the core id
	err = iref_get_core_id(iref, &core_id);
	if(err_is_fail(err)) {
		debug_err(__FILE__, __func__, __LINE__, err, "iref_get_core_id failed");
		monitor_bind_udn_client_request_error(mb, conn_id, domain_id, err);
		return;
	}

	if(core_id == my_core_id) {
		USER_PANIC("Same-core UDN binding NYI");
	}

	// Identify notify cap
	struct capability capability;
	err = monitor_cap_identify(notify, &capability);
	if(err_is_fail(err)) {
		USER_PANIC_ERR(err, "monitor_cap_identify failed, ignored");
		return;
	}
	assert(capability.type == ObjType_Notify_IPI || capability.type == ObjType_Null);
	/* assert(capability.u.notify.coreid == my_core_id); */

	/* Forward request to the corresponding monitor */
	// Create local state
	err = remote_conn_alloc(&conn, &conn_id, REMOTE_CONN_UDN);
	if(err_is_fail(err)) {
		debug_err(__FILE__, __func__, __LINE__, err, "remote_conn_alloc failed");
		monitor_bind_udn_client_request_error(mb, conn_id, domain_id, err);
		return;
	}

    // Track data
    conn->domain_id = domain_id;
    conn->domain_binding = mb;
    conn->core_id = core_id;
    conn->x.udn.core_id = client_core_id;
    conn->x.udn.chan_id = client_in_chan_id;
    conn->x.udn.asid = client_asid;

    // Get connection to the monitor to forward request to
	struct intermon_binding *intermon_binding;
	err = intermon_binding_get(core_id, &intermon_binding);
	if(err_is_fail(err)) {
		debug_err(__FILE__, __func__, __LINE__, err, "intermon_binding_get failed");
		monitor_bind_udn_client_request_error(mb, conn_id, domain_id, err);
		return;
	}

	bind_udn_request_cont(intermon_binding, iref, conn_id, capability, mb, domain_id, client_core_id, client_in_chan_id, client_asid);
}

struct bind_udn_reply_state {
    struct intermon_msg_queue_elem elem;
    struct intermon_bind_udn_reply__args args;
    struct capability capability;
};

static void bind_udn_reply_cont(struct intermon_binding *mon_binding,
		                            uintptr_t your_mon_id,
		                            uintptr_t my_mon_id,
		                            uintptr_t msgerr,
		                            struct capability capability,
		                            uintptr_t server_core_id,
		                            uintptr_t server_in_chan_id,
		                            uintptr_t server_asid)
{
    errval_t err;

    intermon_caprep_t caprep;
    capability_to_caprep(&capability, &caprep);

    err = mon_binding->tx_vtbl.bind_udn_reply(mon_binding, NOP_CONT, your_mon_id, my_mon_id, msgerr, caprep, server_core_id, server_in_chan_id, server_asid);
    if(err_is_fail(err)) {
        if(err_no(err) == FLOUNDER_ERR_TX_BUSY) {
            struct bind_udn_reply_state *me = malloc(sizeof(struct bind_udn_reply_state));
            struct intermon_state *ist = mon_binding->st;
            me->args.con_id = your_mon_id;
            me->args.mon_id = my_mon_id;
            me->args.err = msgerr;
            me->args.core_id = server_core_id;
            me->args.chan_id = server_in_chan_id;
            me->args.asid = server_asid;
            me->capability = capability;
            me->elem.cont = bind_udn_reply_handler;

            err = intermon_enqueue_send(mon_binding, &ist->queue, get_default_waitset(), &me->elem.queue);
            assert(err_is_ok(err));
            return;
        }

        USER_PANIC_ERR(err, "failed forwarding UDN bind reply");
        // cleanup
        if(err_is_ok(msgerr)) {
            err = remote_conn_free(my_mon_id);
            assert(err_is_ok(err));
        }
    }
}

static void bind_udn_reply_handler(struct intermon_binding *b, struct intermon_msg_queue_elem *e)
{
    struct bind_udn_reply_state *st = (struct bind_udn_reply_state *)e;
    bind_udn_reply_cont(b, st->args.con_id, st->args.mon_id, st->args.err, st->capability, st->args.core_id, st->args.chan_id, st->args.asid);
    free(e);
}

/*
 * this function is to send back a reply from the server to client
 * it is called on server side.
 *
 */
static void monitor_bind_udn_reply(struct monitor_binding *dom_binding,
		                                uintptr_t my_mon_id,
		                                uintptr_t domain_id,
		                                errval_t msgerr,
		                                struct capref notify,
		                                uintptr_t server_core_id,
		                                uintptr_t server_in_chan_id,
		                                uintptr_t server_asid)
{
    errval_t err;

    struct remote_conn_state *conn = remote_conn_lookup(my_mon_id);
    if(conn == NULL) {
        USER_PANIC("invalid mon_id in UDN bind reply");
        return;
    }

    uintptr_t your_mon_id = conn->mon_id;
    struct intermon_binding *mon_binding = conn->mon_binding;

    if(err_is_ok(msgerr)) {
        /* Connection accepted */
        conn->domain_id = domain_id;
        conn->domain_binding = dom_binding;
    } else {
    	//error:
        err = remote_conn_free(my_mon_id);
        assert(err_is_ok(err));
    }

    // Identify notify cap
    struct capability capability;
    err = monitor_cap_identify(notify, &capability);
    if(err_is_fail(err)) {
        USER_PANIC_ERR(err, "monitor_cap_identify failed, ignored");
        return;
    }
    assert(capability.type == ObjType_Notify_IPI || capability.type == ObjType_Null);
    /* assert(capability.u.notify.coreid == my_core_id); */

    bind_udn_reply_cont(mon_binding, your_mon_id, my_mon_id, msgerr, capability, server_core_id, server_in_chan_id, server_asid);
}

struct bind_udn_service_request_state {
	struct monitor_msg_queue_elem elem;
    struct monitor_bind_udn_service_request__args args;
    struct intermon_binding *binding;
    uintptr_t your_mon_id;
};

static void bind_udn_service_request_cont(struct monitor_binding *domain_binding,
											    uintptr_t service_id,
											    con_id_t my_mon_id,
											    struct capref notify_cap,
											    struct intermon_binding *binding,
											    con_id_t your_mon_id,
											    coreid_t client_core_id,
											    uint32_t client_in_chan_id,
											    uint8_t client_asid)
{
	errval_t err, err2;

    /* Proxy the request */
    err = domain_binding->tx_vtbl.bind_udn_service_request(domain_binding, NOP_CONT, service_id, my_mon_id, notify_cap, client_core_id, client_in_chan_id, client_asid);
    if(err_is_fail(err)) {
    	if(err_no(err) == FLOUNDER_ERR_TX_BUSY) {
    		struct bind_udn_service_request_state *me = malloc(sizeof(struct bind_udn_service_request_state));
            struct monitor_state *ist = domain_binding->st;
            me->args.service_id = service_id;
            me->args.mon_id = my_mon_id;
            me->args.core_id = client_core_id;
            me->args.chan_id = client_in_chan_id;
            me->args.asid = client_asid;
            me->args.notify = notify_cap;
            me->binding = binding;
            me->your_mon_id = your_mon_id;
            me->elem.cont = bind_udn_service_request_handler;

            err = monitor_enqueue_send(domain_binding, &ist->queue, get_default_waitset(), &me->elem.queue);
            assert(err_is_ok(err));
            return;
        }

        err2 = remote_conn_free(my_mon_id);
        if (err_is_fail(err2)) {
            USER_PANIC_ERR(err2, "remote_conn_free failed");
        }

        // in this case, it fails, so reply a wrong number for core id and channel id
        intermon_caprep_t nullcap = {0,0,0,0};
        err2 = binding->tx_vtbl.bind_udn_reply(binding, NOP_CONT, your_mon_id, 0, err, nullcap, -1, -1, -1);
        if (err_is_fail(err2)) {
            USER_PANIC_ERR(err2, "Sending bind_udn_reply1 failed");
        }
    }
}

static void bind_udn_service_request_handler(struct monitor_binding *b, struct monitor_msg_queue_elem *e)
{
	struct bind_udn_service_request_state *st = (struct bind_udn_service_request_state *)e;
    bind_udn_service_request_cont(b, st->args.service_id, st->args.mon_id, st->args.notify, st->binding, st->your_mon_id, st->args.core_id, st->args.chan_id, st->args.asid);
    free(e);
}

/*
 * this function is for the client to connect to remote server monitor on another core.
 * this function is called after a local client request.
 * we need to pass our client core id and client input channel id to the remote server.
 */
static void intermon_bind_udn_request(struct intermon_binding *ib,
										   iref_t iref,
										   con_id_t your_mon_id,
										   intermon_caprep_t caprep,
										   coreid_t client_core_id,
										   uint32_t client_in_chan_id,
										   uint8_t client_asid)
{
	errval_t err;

    // Get client's core_id
    struct intermon_state *ist = ib->st;
    assert(ist != NULL);
    coreid_t core_id = ist->core_id;

    printf("aaa udn_support get core id: %d \n",(int)core_id);

    // Construct the notify cap
    struct capref notify_cap = NULL_CAP;
    struct capability capability;
    caprep_to_capability(&caprep, &capability);
    if(capability.type != ObjType_Null) {
        err = slot_alloc(&notify_cap);
        if (err_is_fail(err)) {
            USER_PANIC_ERR(err, "Failed to allocate slot from channel_alloc");
        }
        err = monitor_cap_create(notify_cap, &capability, core_id);
        if (err_is_fail(err)) {
            USER_PANIC_ERR(err, "monitor_cap_create failed");
        }
    }

    /* Get the server's connection */
	struct monitor_binding *domain_binding = NULL;
	err = iref_get_binding(iref, &domain_binding);
	assert(err_is_ok(err));

	/* Get the service id */
	uintptr_t service_id = 0;
	err = iref_get_service_id(iref, &service_id);
	assert(err_is_ok(err));

	/* Create a new connection state */
	uintptr_t my_mon_id;
	struct remote_conn_state *con;
	err = remote_conn_alloc(&con, &my_mon_id, REMOTE_CONN_UDN);
	assert(err_is_ok(err));

	// Set the monitor portion of it
	con->mon_id = your_mon_id;
	con->mon_binding = ib;
	con->core_id = core_id;
	con->x.udn.core_id = client_core_id;
	con->x.udn.chan_id = client_in_chan_id;
	con->x.udn.asid = client_asid;


	bind_udn_service_request_cont(domain_binding, service_id, my_mon_id, notify_cap, ib, your_mon_id, client_core_id, client_in_chan_id, client_asid);
}

struct bind_udn_reply_client_state {
    struct monitor_msg_queue_elem elem;
    struct monitor_bind_udn_reply_client__args args;
};

static void bind_udn_reply_client_cont(struct monitor_binding *domain_binding,
		                                    uintptr_t my_mon_id,
		                                    uintptr_t domain_id,
		                                    errval_t msgerr,
		                                    struct capref notify_cap,
		                                    coreid_t server_core_id,
		                                    uint32_t server_in_chan_id,
		                                    uint8_t server_asid)
{
    errval_t err;

    err = domain_binding->tx_vtbl.bind_udn_reply_client(domain_binding, NOP_CONT, my_mon_id, domain_id, msgerr, notify_cap, server_core_id, server_in_chan_id, server_asid);
    if(err_is_fail(err)) {
    	if(err_no(err) == FLOUNDER_ERR_TX_BUSY) {
            struct bind_udn_reply_client_state *me = malloc(sizeof(struct bind_udn_reply_client_state));
            struct monitor_state *ist = domain_binding->st;
            me->args.mon_id = my_mon_id;
            me->args.conn_id = domain_id;
            me->args.err = msgerr;
            me->args.notify = notify_cap;
            me->args.core_id = server_core_id;
            me->args.chan_id = server_in_chan_id;
            me->args.asid = server_asid;
            me->elem.cont = bind_udn_reply_client_handler;

            err = monitor_enqueue_send(domain_binding, &ist->queue, get_default_waitset(), &me->elem.queue);
            assert(err_is_ok(err));
            return;
        }

        USER_PANIC_ERR(err, "UDN bind reply failed");
        // cleanup
        err = remote_conn_free(my_mon_id);
        if(err_is_fail(err)) {
            USER_PANIC_ERR(err, "remote_conn_free failed");
        }
    }
}

static void bind_udn_reply_client_handler(struct monitor_binding *b, struct monitor_msg_queue_elem *e)
{
    struct bind_udn_reply_client_state *st = (struct bind_udn_reply_client_state *)e;
    bind_udn_reply_client_cont(b, st->args.mon_id, st->args.conn_id, st->args.err, st->args.notify, st->args.core_id, st->args.chan_id, st->args.asid);
    free(e);
}

/*
 * this function is to pass a remote server reply back to client side
 */
static void intermon_bind_udn_reply(struct intermon_binding *ib,
		                                 uint64_t my_mon_id,
		                                 uint64_t your_mon_id,
		                                 errval_t msgerr,
		                                 intermon_caprep_t caprep,
		                                 coreid_t server_core_id,
		                                 uint32_t server_in_chan_id,
		                                 uint8_t server_asid)
{
    errval_t err;

    struct remote_conn_state *con = remote_conn_lookup(my_mon_id);
    if(con == NULL) {
    	USER_PANIC_ERR(0, "unknown mon_id in UDN bind reply");
        return;
    }

    printf("aaa intermon_bind_udn_reply my_core_id:%d server_core_id:%d\n",(int)my_core_id,(int)server_core_id);

    uintptr_t domain_id = con->domain_id;
    struct monitor_binding *domain_binding = con->domain_binding;
    struct capref notify_cap = NULL_CAP;

    if(err_is_ok(msgerr)) { /* bind succeeded */
        con->mon_id = your_mon_id;
        con->mon_binding = ib;

        struct capability capability;
        caprep_to_capability(&caprep, &capability);

        if(capability.type != ObjType_Null) {
        	// Get core id of sender
            coreid_t core_id = ((struct intermon_state*)ib->st)->core_id;

            // Construct the notify cap
            err = slot_alloc(&notify_cap);
            if(err_is_fail(err)) {
                USER_PANIC_ERR(err, "Failed to allocate slot from channel_alloc");
            }

            err = monitor_cap_create(notify_cap, &capability, core_id);
            if (err_is_fail(err)) {
                USER_PANIC_ERR(err, "monitor_cap_create failed");
            }
        }
    } else { /* bind refused */
        err = remote_conn_free(my_mon_id);
        assert(err_is_ok(err));
    }

    bind_udn_reply_client_cont(domain_binding, my_mon_id, domain_id, msgerr, notify_cap, server_core_id, server_in_chan_id, server_asid);
}

errval_t udn_monitor_init(struct monitor_binding *mb)
{
	mb->rx_vtbl.bind_udn_client_request = monitor_bind_udn_client_request;
	mb->rx_vtbl.bind_udn_reply_monitor = monitor_bind_udn_reply;
	return SYS_ERR_OK;
}

errval_t udn_intermon_init(struct intermon_binding *ib)
{
    ib->rx_vtbl.bind_udn_request = intermon_bind_udn_request;
    ib->rx_vtbl.bind_udn_reply = intermon_bind_udn_reply;
	return SYS_ERR_OK;
}
