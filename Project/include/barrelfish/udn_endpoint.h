/*
 * udn_endpoint.h
 *
 *  Created on: Feb 1, 2013
 *      Author: robert
 */

#ifndef LIBBARRELFISH_UDN_ENDPOINT_H_
#define LIBBARRELFISH_UDN_ENDPOINT_H_

#include <barrelfish/waitset.h>
#include <barrelfish/udn_impl.h>


/// Incoming UDN endpoint
struct udn_endpoint {
    struct waitset_chanstate  waitset_state; ///< Waitset per-channel state
    struct udn_in_chan_state  in_chan;       ///< Incoming UDN channel state to poll
};

errval_t udn_endpoint_init(struct udn_endpoint *ep, int num_msg);

void udn_endpoint_destroy(struct udn_endpoint *ep);

errval_t udn_endpoint_register(struct udn_endpoint *ep, struct waitset *ws, struct event_closure closure);

errval_t udn_endpoint_deregister(struct udn_endpoint *ep);

void udn_endpoint_migrate(struct udn_endpoint *ep, struct waitset *ws);

/**
 * \brief Returns true if there is a message pending on the given UDN endpoint
 */
static inline bool udn_endpoint_can_recv(struct udn_endpoint *ep)
{
	udn_chan_lock();
	struct udn_message *msg = NULL;
    msg = udn_chan_impl_poll(&ep->in_chan);
    udn_chan_unlock();
    return msg != NULL;
}

/**
 * \brief Retrieve a message from the given UDN endpoint, if possible
 *
 * Non-blocking, may fail if there are no messages available.
 *
 * \param ep UDN endpoint
 * \param retmsg Storage for incoming message
 */
static inline errval_t udn_endpoint_recv(struct udn_endpoint *ep, struct udn_message **msg)
{
    *msg = udn_impl_recv(&ep->in_chan);

    if(*msg != NULL) {
        return SYS_ERR_OK;
    } else {
        return LIB_ERR_NO_UDN_MSG;
    }
}

#endif /* LIBBARRELFISH_UDN_ENDPOINT_H_ */
