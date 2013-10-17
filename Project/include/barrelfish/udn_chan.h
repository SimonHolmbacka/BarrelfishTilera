/*
 * udn_chan.h
 *
 *  Created on: Feb 1, 2013
 *      Author: robert
 */

#ifndef BARRELFISH_UDN_CHAN_H_
#define BARRELFISH_UDN_CHAN_H_

#include <barrelfish/udn_endpoint.h>
#include <barrelfish/monitor_client.h>

// default udn message number, we use it to create message buffer.
#define DEFAULT_UDN_MSG_NUM 10

struct udn_chan;
struct monitor_binding;

struct udn_bind_continuation {
    /**
     * \brief Handler which runs when a binding succeeds or fails
     * \param st State pointer set in closure
     * \param err Success/failure of binding
     * \param chan On success, contains pointer to channel
     * \param notify On success, contains (optional) notification capability
     */
    void (*handler)(void *st, errval_t err, struct udn_chan *chan,
    		        struct capref notify);
    void *st;
};

struct udn_chan {
	struct monitor_cap_handlers cap_handlers;   /* XXX: must be first */ // ???

	struct udn_out_chan_state send_chan;   ///< Outgoing UDN channel state
    struct udn_endpoint endpoint;          ///< Incoming UDN endpoint

    /// connection state
    enum {
    	UDN_DISCONNECTED,     ///< Disconnected
    	UDN_BIND_WAIT,        ///< Waiting for bind reply
    	UDN_CONNECTED,        ///< Connection established
    } connstate;

    udn_index_t max_send_msgs;  ///< Number of messages that fit in the send channel
    udn_index_t max_recv_msgs;  ///< Number of messages that fit in the recv channel
    uintptr_t monitor_id;       ///< Local monitor's connection ID for this channel
    struct monitor_binding *monitor_binding; ///< Monitor binding used for cap xfer

    /* Arguments for an ongoing bind attempt */
    iref_t iref;                ///< IREF to which we bound
    struct capref notify_cap;
    struct udn_bind_continuation bind_continuation; ///< Continuation for bind
};

struct event_queue_node;

errval_t udn_chan_init(struct udn_chan *chan, coreid_t dest_core_id, uint32_t out_chan_id, uint8_t dest_asid);

errval_t udn_chan_bind(struct udn_chan *chan,
						 struct udn_bind_continuation cont,
						 struct event_queue_node *qnode,
						 iref_t iref,
						 struct monitor_binding *monitor_binding,
						 struct capref notify_cap);

errval_t udn_chan_accept(struct udn_chan *chan, uintptr_t mon_id, coreid_t dest_core_id, uint32_t out_chan_id, uint8_t dest_asid);

void udn_chan_send_bind_reply(struct monitor_binding *mb,
		                          struct udn_chan *chan,
		                          errval_t err,
		                          uintptr_t monitor_id,
		                          struct capref notify_cap);

void udn_chan_destroy(struct udn_chan *chan);

void udn_init(void);

/**
 * \brief Register an event handler to be notified when messages can be received
 *
 * In the future, call the closure on the given waitset when it is likely that
 * a message can be received on the channel. A channel may only be registered
 * with a single receive event handler on a single waitset at any one time.
 *
 * \param chan UDN channel
 * \param ws Waitset
 * \param closure Event handler
 */
static inline errval_t udn_chan_register_recv(struct udn_chan *chan,
                                              struct waitset *ws,
                                              struct event_closure closure)
{
    return udn_endpoint_register(&chan->endpoint, ws, closure);
}

/**
 * \brief Cancel an event registration made with udn_chan_register_recv()
 *
 * \param chan UDN channel
 */
static inline errval_t udn_chan_deregister_recv(struct udn_chan *chan)
{
    return udn_endpoint_deregister(&chan->endpoint);
}

static inline errval_t udn_chan_recv(struct udn_chan *chan,
                                     struct udn_message **msg)
{
    assert(msg != NULL);
    return udn_endpoint_recv(&chan->endpoint, msg);
}

/*
static inline volatile struct udn_message *udn_chan_get_next(
                                struct udn_chan *chan, struct udn_control *ctrl)
{
	assert(!"implement me!");
    //return udn_impl_get_next(&chan->send_chan, ctrl);
}
*/

#endif /* BARRELFISH_UDN_CHAN_H_ */
