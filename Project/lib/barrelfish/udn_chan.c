/*
 * udn_chan.c
 *
 *  Created on: Feb 1, 2013
 *      Author: robert
 */

#include <string.h>
#include <barrelfish/barrelfish.h>
#include <barrelfish/udn_chan.h>
#include <barrelfish/idc_export.h>
#include <if/monitor_defs.h>

/*
 * Initialize the channel
 */
errval_t udn_chan_init(struct udn_chan *chan, coreid_t dest_core_id, uint32_t out_chan_id, uint8_t dest_asid)
{
    assert(chan != NULL);
    errval_t err;

    // in
    err = udn_endpoint_init(&chan->endpoint, DEFAULT_UDN_MSG_NUM);
    if(err_is_fail(err)) {
        return err;
    }

    // out
    printf("\n udn channel init, dest_core_id: %d, out_chan_id: %d dest_asid: %d \n",(int)dest_core_id, (int)out_chan_id, (int)dest_asid);
    udn_out_chan_state_init(&chan->send_chan, dest_core_id, out_chan_id, dest_asid);

    chan->max_send_msgs = DEFAULT_UDN_MSG_NUM;
    chan->max_recv_msgs = DEFAULT_UDN_MSG_NUM;
    memset(&chan->cap_handlers, 0, sizeof(chan->cap_handlers)); /* TODO */
    chan->iref = 0;
    chan->monitor_binding = get_monitor_binding();

    return SYS_ERR_OK;
}

void udn_chan_destroy(struct udn_chan *chan)
{
	udn_endpoint_destroy(&chan->endpoint);
}

/// Handler for UDN bind reply messages from the Monitor
/*
 * this function is on client side to accept the reply from remote server,
 * this is the last step to finish the binding.
 */
static void bind_udn_reply_handler(struct monitor_binding *b,
		                                uintptr_t mon_id,
		                                uintptr_t conn_id,
		                                errval_t success,
		                                struct capref notify,
		                                uintptr_t server_core_id,
		                                uintptr_t server_in_chan_id,
		                                uintptr_t server_asid)
{
    struct udn_chan *chan = (void *)conn_id;

    assert(chan->connstate == UDN_BIND_WAIT);
    assert(server_core_id >= 0);
    assert(server_in_chan_id >= 0);
    assert(server_asid > 0);
    printf("client received channel id %lu and destination core id %lu from the server\n", server_in_chan_id, server_core_id);

    if (err_is_ok(success)) { /* bind succeeded */
    	// if bind succeeds, then continue to initialize the client output channel
    	udn_out_chan_state_init(&chan->send_chan, server_core_id, server_in_chan_id, server_asid);
        chan->connstate = UDN_CONNECTED;
        chan->monitor_id = mon_id;
    } else { /* bind failed */
        chan->connstate = UDN_DISCONNECTED;
        /* TODO: delete endpoint, destroy local_cap */
    }

    /* either way, tell the user what happened */
    assert(chan->bind_continuation.handler != NULL);
    chan->bind_continuation.handler(chan->bind_continuation.st, success, chan, notify);
}

struct bind_udn_reply_state {
    struct monitor_binding *b;
    struct udn_chan *chan;
    struct monitor_bind_udn_reply_monitor__args args;
    struct event_queue_node qnode;
};

static void send_bind_reply(void *arg)
{
    struct bind_udn_reply_state *st = arg;
    struct monitor_binding *b = st->b;
    errval_t err;

    // get the current core id
    coreid_t current_core_id = 255;  // fake value
    current_core_id = disp_get_core_id();
    assert(current_core_id < 64);
    //printf("server generated new channel %lu on core %d, sending to the client.\n", st->chan->endpoint.in_chan.chan_id, current_core_id);

    // get the current asid
    uint8_t current_asid = 0;
    current_asid = get_asid();
    assert(current_asid > 0);

    // send back a bind success/failure message to the monitor
    err = st->b->tx_vtbl.bind_udn_reply_monitor(st->b, NOP_CONT, st->args.mon_id, st->args.conn_id, st->args.err, st->args.notify, current_core_id, st->chan->endpoint.in_chan.chan_id, current_asid);
    if(err_is_ok(err)) {
    	event_mutex_unlock(&b->mutex);
        free(st);
    } else if(err_no(err) == FLOUNDER_ERR_TX_BUSY) {
        err = st->b->register_send(st->b, st->b->waitset, MKCONT(send_bind_reply, st));
        assert(err_is_ok(err)); // shouldn't fail, as we have the mutex
    } else {
        event_mutex_unlock(&b->mutex);
        USER_PANIC_ERR(err, "failed sending back reply to UDN bind request; request dropped!");
        if(st->chan != NULL) {
        	udn_chan_destroy(st->chan);
            // FIXME: how do we tell the binding about this!?
        }
        free(st);
    }
}

/// Handler for UDN bind request messages from the Monitor
/*
 * this function is to pass a client request to this local monitor.
 * here we pass client core id, input channel id and client asid to this monitor.
 */
static void bind_udn_service_request_handler(struct monitor_binding *b,
                                             	   uintptr_t service_id,
                                             	   uintptr_t mon_id,
                                             	   struct capref notify_cap,
                                             	   uintptr_t client_core_id,
                                             	   uintptr_t client_in_chan_id,
                                             	   uintptr_t client_asid)
{
    struct idc_export *e = (void *)service_id;
    errval_t err;

    // call the binding's connect handler
    if(e->udn_connect_callback != NULL) {
    	// this connect handler will go to backend code to initialize the server.
        err = e->udn_connect_callback(e->connect_cb_st, b, mon_id, notify_cap, client_core_id, client_in_chan_id, client_asid);
    } else {
        err = LIB_ERR_NO_UDN_BIND_HANDLER;
    }

    if(err_is_fail(err)) {
        udn_chan_send_bind_reply(b, NULL, err, mon_id, NULL_CAP);
    } else {
        // binding is responsible for sending reply
    }
}

/*
 * this function is called after udn_chan_accept().
 * it is to send a reply to the client.
 */
void udn_chan_send_bind_reply(struct monitor_binding *mb, struct udn_chan *chan, errval_t err, uintptr_t monitor_id, struct capref notify_cap)
{
	struct bind_udn_reply_state *st = malloc(sizeof(struct bind_udn_reply_state));
    assert(st != NULL);

    if(err_is_ok(err)) {
        assert(chan != NULL);
    } else {
        assert(chan == NULL);
    }

    st->b = mb;
    st->chan = chan;
    st->args.err = err;
    st->args.mon_id = monitor_id;
    st->args.conn_id = err_is_ok(err) ? (uintptr_t)chan : 0;
    st->args.notify = notify_cap;

    // wait for the ability to use the monitor binding
    event_mutex_enqueue_lock(&mb->mutex, &st->qnode, MKCLOSURE(send_bind_reply, st));
}

/*
 * this function is to start a inter-core binding request.
 */
static void send_bind_cont(void *arg)
{
    struct udn_chan *chan = arg;
    struct monitor_binding *b = chan->monitor_binding;
    errval_t err;

    // get the current core id
    coreid_t current_core_id = 255;  // fake value
    current_core_id = disp_get_core_id();
    assert(current_core_id < 64);

    // get its own asid
    uint8_t current_asid = 0;
    current_asid = get_asid();
    assert(current_asid > 0);

    /* Send bind request to the monitor */
    assert(chan->monitor_binding == b);
    assert(b->tx_vtbl.bind_udn_client_request);
    //printf("client generated new channel %lu on core %d, sending them to the server.\n", chan->endpoint.in_chan.chan_id, current_core_id);

    // here we send binding request to our monitor, which will be in charge of sending it to another core's monitor
    // in our binding request, we are sending our core id and our input channel id to another core.
    err = b->tx_vtbl.bind_udn_client_request(b, NOP_CONT, chan->iref, (uintptr_t)chan, chan->notify_cap, current_core_id, chan->endpoint.in_chan.chan_id, current_asid);
    if(err_is_ok(err)) { // request sent ok
        event_mutex_unlock(&b->mutex);
    } else if(err_no(err) == FLOUNDER_ERR_TX_BUSY) {
        // register to retry
        err = b->register_send(b, b->waitset, MKCONT(send_bind_cont, chan));
        assert(err_is_ok(err)); // we hold the monitor binding mutex
    } else { // permanent failure sending message
        event_mutex_unlock(&b->mutex);
        chan->bind_continuation.handler(chan->bind_continuation.st, err_push(err, LIB_ERR_BIND_UDN_REQ), NULL, NULL_CAP);
    }
}

/*
 * Initialize the client's side, after the client receive the exported service from the server
 */
errval_t udn_chan_bind(struct udn_chan *chan,
						 struct udn_bind_continuation cont,
						 struct event_queue_node *qnode,
						 iref_t iref,
						 struct monitor_binding *monitor_binding,
						 struct capref notify_cap)
{
    errval_t err;

    // initialize input channel state to generate input channel id
    // because at this moment, we cannot know the serve's core id and input_channel id,
    // so we will get them later in server's reply
    err = udn_endpoint_init(&chan->endpoint, DEFAULT_UDN_MSG_NUM);
    if(err_is_fail(err)) {
        return err;
    }

    chan->max_send_msgs = DEFAULT_UDN_MSG_NUM;
    chan->max_recv_msgs = DEFAULT_UDN_MSG_NUM;
    memset(&chan->cap_handlers, 0, sizeof(chan->cap_handlers)); /* TODO */

    // store bind args
    chan->bind_continuation = cont;
    chan->monitor_binding = monitor_binding;
    chan->iref = iref;
    chan->notify_cap = notify_cap;

    // wait for the ability to use the monitor binding
    chan->connstate = UDN_BIND_WAIT;

    // then go to send_bind_cont function to continue inter-core binding
    event_mutex_enqueue_lock(&monitor_binding->mutex, qnode, MKCLOSURE(send_bind_cont, chan));

    return SYS_ERR_OK;
}

/**
 * Initialise a new UDN channel to accept an incoming binding request from client
 * It works in server side.
 *
 * \param chan  		Storage for channel state
 * \param mon_id 		Monitor's connection ID for this channel
 * @param dest_core_id	This is the client core id, which the server want to talk to
 * @param out_chan_id	This is the client input channel id
 * @param dest_asid     This is the client asid
 */
errval_t udn_chan_accept(struct udn_chan *chan, uintptr_t mon_id, coreid_t dest_core_id, uint32_t out_chan_id, uint8_t dest_asid)
{
    errval_t err;
    chan->monitor_id = mon_id;

    assert(dest_core_id >= 0);
    assert(out_chan_id >= 0);
    printf("server received channel id %lu and destination core id %u from the client\n", out_chan_id, dest_core_id);

    // initialise server channel state
    err = udn_chan_init(chan, dest_core_id, out_chan_id, dest_asid);
    if (err_is_fail(err)) {
        return err;
    }

    chan->connstate = UDN_CONNECTED;
    return SYS_ERR_OK;
}

/// Initialise the UDN channel driver
void udn_init(void) {
    struct monitor_binding *mcb = get_monitor_binding();
    mcb->rx_vtbl.bind_udn_reply_client = bind_udn_reply_handler;
    mcb->rx_vtbl.bind_udn_service_request = bind_udn_service_request_handler;
    udn_impl_init();
}
