/*
 * flounder_support_udn.h
 *
 *  Created on: Feb 2, 2013
 *      Author: robert
 */

#ifndef FLOUNDER_SUPPORT_UDN_H_
#define FLOUNDER_SUPPORT_UDN_H_

#include <barrelfish/udn_chan.h>
#include <flounder/flounder_support_caps.h>
#include <trace/trace.h>

#include <assert.h>

/// Special message types
enum flounder_udn_msgtype {
	FL_UDN_ACK = 0,
    FL_UDN_CAP_ACK = 0x7FFFFFFF,  // make it the largest positive number in 32-bit number
};

struct flounder_udn_state {
    struct udn_chan chan;

    udn_index_t next_id;   ///< Sequence number of next message to be sent
    udn_index_t seq_id;    ///< Last sequence number received from remote
    udn_index_t ack_id;    ///< Last sequence number acknowledged by remote
    udn_index_t last_ack;  ///< Last acknowledgement we sent to remote

    struct flounder_cap_state capst; ///< State for indirect cap tx/rx machinery
};

void flounder_stub_udn_state_init(struct flounder_udn_state *s, void *binding);

errval_t flounder_stub_udn_send_string(struct flounder_udn_state *s, int msgnum, const char *str, size_t *pos, size_t *len);

errval_t flounder_stub_udn_recv_string(struct udn_message *msg, char **str, size_t *pos, size_t *len);

errval_t flounder_stub_udn_send_buf(struct flounder_udn_state *s, int msgnum, const void *buf, size_t len, size_t *pos);

errval_t flounder_stub_udn_recv_buf(struct udn_message *msg, void **buf, size_t *len, size_t *pos);

/// Computes (from seq/ack numbers) whether we can currently send on the channel
static inline bool flounder_stub_udn_can_send(struct flounder_udn_state *s)
{
	return (udn_index_t)(s->next_id - s->ack_id) <= s->chan.max_send_msgs;
}

/// Prepare a "control" word (header for each UDN message fragment)
//static inline void flounder_stub_udn_control_fill(struct flounder_udn_state *s, struct udn_control *ctrl, int msgtype)
//{
//	assert(!"implement me!");
//}

/// Process a "control" word
//static inline int flounder_stub_udn_control_process(struct flounder_udn_state *s, struct udn_control ctrl)
//{
//	assert(!"implement me!");
//}

/// Emit memory barrier needed between writing UDN payload and header
//static inline void flounder_stub_udn_barrier(void)
//{
//	assert(!"implement me!");
//}

/// Should we send an ACK?
static inline bool flounder_stub_udn_needs_ack(struct flounder_udn_state *s)
{
    // send a forced ACK if the channel is full
    // FIXME: should probably send it only when "nearly" full
    return ((udn_index_t)(s->seq_id - s->last_ack) >= (udn_index_t)(s->chan.max_recv_msgs - 1));
}

/// Send an explicit ACK
static inline void flounder_stub_udn_send_ack(struct flounder_udn_state *s)
{
//    assert(flounder_stub_udn_can_send(s));
//    struct udn_control ctrl;
//    volatile struct udn_message *msg = udn_chan_get_next(&s->chan, &ctrl);
//    flounder_stub_udn_control_fill(s, &ctrl, FL_UDN_ACK);
//    msg->header.control = ctrl;
	assert(!"implement me!");
}

/// Send a cap ACK (message that we are ready to receive caps)
static inline void flounder_stub_udn_send_cap_ack(struct flounder_udn_state *s)
{
	assert(flounder_stub_udn_can_send(s));

	// in this case we only send "FL_UDN_CAP_ACK"
	//debug_printf("sending cap ack message to core(%lu,%lu)", (s->chan.send_chan.int_id >> 18) & 0b11111111111, (s->chan.send_chan.int_id >> 7) & 0b11111111111);
	udn_send(s->chan.send_chan.int_id | 3);
	udn_send((s->chan.send_chan.dest_asid << 24) | s->chan.send_chan.chan_id);
	udn_send(FL_UDN_CAP_ACK);
	udn_send(0);
}


#endif /* FLOUNDER_SUPPORT_UDN_H_ */
