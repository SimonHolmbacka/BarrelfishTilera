/*
 * udn_endpoint.c
 *
 *  Created on: Feb 1, 2013
 *      Author: robert
 */

#include <barrelfish/barrelfish.h>
#include <barrelfish/udn_endpoint.h>
#include <barrelfish/udn_impl.h>
#include <barrelfish/waitset.h>
#include <barrelfish/waitset_chan.h>
#include "waitset_chan_priv.h"

#include <stdio.h>

/**
 * \brief Initialise a new UDN endpoint
 *
 * \param ep       Storage for endpoint state
 * \param num_msg  Number of messages, which should be able to store at the same time
 */
errval_t udn_endpoint_init(struct udn_endpoint *ep, int num_msg) {
	printf("aaa udn_endpoint_init \n");
    errval_t err = udn_in_chan_state_init(&ep->in_chan, num_msg);
    if (err_is_fail(err)) {
        return err;
    }

    waitset_chanstate_init(&ep->waitset_state, CHANTYPE_UDN_IN);
    return SYS_ERR_OK;
}

/**
 * \brief Destroy the local state associated with a given UDN endpoint
 */
void udn_endpoint_destroy(struct udn_endpoint *ep) {
	udn_in_chan_state_destroy(&ep->in_chan);
	waitset_chanstate_destroy(&ep->waitset_state);
}

/**
 * \brief Register an event handler to be notified when messages can be received
 *
 * In the future, call the closure on the given waitset when it is likely that
 * a message can be received on the endpoint. An endpoint may only be registered
 * with a single event handler on a single waitset at any one time.
 *
 * \param ep UDN endpoint
 * \param ws Waitset
 * \param closure Event handler
 */
errval_t udn_endpoint_register(struct udn_endpoint *ep, struct waitset *ws,
                                struct event_closure closure) {
    assert(ep != NULL);
    assert(ws != NULL);

    if (udn_endpoint_can_recv(ep)) { // trigger event immediately
        return waitset_chan_trigger_closure(ws, &ep->waitset_state, closure);
    } else {
        return waitset_chan_register_polled(ws, &ep->waitset_state, closure);
    }
}

/**
 * \brief Cancel an event registration made with udn_endpoint_register()
 *
 * \param ep UDN Endpoint
 */
errval_t udn_endpoint_deregister(struct udn_endpoint *ep) {
    assert(ep);
    return waitset_chan_deregister(&ep->waitset_state);
}

/**
 * \brief Migrate an event registration made with udn_endpoint_register() to a new waitset.
 *
 * \param ep UDN Endpoint
 * \param ws New waitset
 */
void udn_endpoint_migrate(struct udn_endpoint *ep, struct waitset *ws) {
    printf("udn_endpoint_migrate\n");
    waitset_chan_migrate(&ep->waitset_state, ws);
}

