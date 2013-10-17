/*
 * udn_impl.c
 *
 *  Created on: Feb 3, 2013
 *      Author: robert
 */

#include <barrelfish/barrelfish.h>
#include <barrelfish/udn_impl.h>
#include <string.h>

#define MAX_MSG_NUM 100

#define PUT_MSG_INTO_BUFFER(x) ((x)->pos = ((x)->pos + 1) % (x)->bufmsgs)
#define TAKE_MSG_FROM_BUFFER(x) ((x)->avail_pos = ((x)->avail_pos + 1) % (x)->bufmsgs)

struct udn_impl_message {
	uint32_t chan_id;
	struct udn_message msg;
};

struct udn_backend_chan_info {
	bool available;
	struct udn_in_chan_state *state;
};

struct udn_backend_state {
	spinlock_t                    lock;
	uint32_t                      bufmsgs;
	struct udn_impl_message      *msg_buf;
	udn_index_t                   pos;        ///< Current position
	udn_index_t                   avail_pos;  ///< position of the last available message
	uint32_t                      chan_bytes;
	struct udn_backend_chan_info *chan_info;
};

struct udn_backend_state be_state = {
		.lock = 0,
		.bufmsgs = 0,
		.msg_buf = NULL,
		.pos = 0,
		.avail_pos = 0,
		.chan_bytes = 0,
		.chan_info = NULL
};

void udn_impl_init(void)
{
	be_state.bufmsgs = MAX_MSG_NUM;
	be_state.msg_buf = malloc(be_state.bufmsgs * sizeof(struct udn_impl_message));
	assert(be_state.msg_buf);
}

void udn_backend_add_incoming_chan(struct udn_in_chan_state *c)
{
	acquire_spinlock(&be_state.lock);
	be_state.chan_bytes += sizeof(struct udn_backend_chan_info);
	be_state.chan_info = realloc(be_state.chan_info, be_state.chan_bytes);
	c->chan_id = get_new_in_chan_id();
	be_state.chan_info[c->chan_id].state = c;
	be_state.chan_info[c->chan_id].available = 1;
	release_spinlock(&be_state.lock);
	debug_printf("create new incoming channel id : %lu", c->chan_id);
}

static inline void udn_backend_remove_chan(struct udn_in_chan_state *c) {
	acquire_spinlock(&be_state.lock);
	// Memory reclaiming is left as a task for the reader.
	struct udn_backend_chan_info chan_info = be_state.chan_info[c->chan_id];
	assert(chan_info.available);
	be_state.chan_info[c->chan_id].available = 0;
	be_state.chan_info[c->chan_id].state = NULL;
	release_spinlock(&be_state.lock);
}

static inline bool is_backend_buf_full(struct udn_backend_state *state) {
	return ((state->pos + 1) % state->bufmsgs) == state->avail_pos;
}

static inline bool is_buf_full(struct udn_in_chan_state *chan_state) {
	if (((chan_state->pos + 1) % chan_state->bufmsgs) == chan_state->avail_pos) {
		return 1;
	}
	// XXX Leave one message free, because the last delivered message maybe still in use?
	// Maybe not true, due to all the internals of the message passing, but for the moment
	// this is save and in the worst case waste of one message size per channel end.
	return(((chan_state->pos + 2) % chan_state->bufmsgs) == chan_state->avail_pos);
}

static inline void deliver_from_backend_to_channel(void) {
	assert(be_state.lock);
	while (be_state.pos != be_state.avail_pos) {
		uint32_t chan_id = be_state.msg_buf[be_state.avail_pos].chan_id;
		struct udn_backend_chan_info *info = &be_state.chan_info[chan_id];
		assert(info->available);
		struct udn_in_chan_state *c = info->state;
		if (is_buf_full(c)) {
			// No magic here, if the first part of the ring buffer can't be delivered,
			// we just block.
			break;
		}
		struct udn_message *to = &c->buf[c->pos];
		struct udn_message *from = &be_state.msg_buf[be_state.avail_pos].msg;
		// the actual size of message
		size_t msg_prolog = 2 * sizeof(uint32_t);
		assert(msg_prolog == offsetof(struct udn_message, data));
		size_t bytes = msg_prolog + from->data_len * sizeof(udn_msg_word_t);
		memcpy(to, from, bytes);
		//c->pos = (c->pos + 1) % c->bufmsgs;
		PUT_MSG_INTO_BUFFER(c);
		//be_state.avail_pos = (be_state.avail_pos + 1) % be_state.bufmsgs;
		TAKE_MSG_FROM_BUFFER(&be_state);
//		printf("Put one message of length %" PRIu32 " in buffer of channel %" PRIu32 ".\n", from->data_len, chan_id);
//		printf("c->pos: %d, c->avail_pos: %d\n", c->pos, c->avail_pos);
	}
}

static inline void udn_backend_impl_poll(void)
{
	assert(be_state.lock);
	deliver_from_backend_to_channel();
	while(!is_backend_buf_full(&be_state) && __insn_mfspr(SPR_UDN_CA_REM)) {
		uint32_t chan_id = __insn_mfspr(SPR_UDN_CA_TAG) & 0x00FFFFFF;
		uint8_t asid = (__insn_mfspr(SPR_UDN_CA_TAG) >> 24) & 0xFF;
		if(asid != get_asid()) {  // compare if we can receive this message
			return;
		}
		printf("udn backend received message chan_id : %lu\n", chan_id);
		uint32_t type = __insn_mfspr(SPR_UDN_CA_DATA);
//		printf("udn backend received message type : %lu\n", type);
		uint32_t data_len = __insn_mfspr(SPR_UDN_CA_DATA);
		assert(__insn_mfspr(SPR_UDN_CA_REM) >= data_len);
//		printf("udn backend received message length : %lu\n", data_len);
		struct udn_impl_message *recv = &be_state.msg_buf[be_state.pos];
		recv->chan_id = chan_id;
		recv->msg.type = type;
		recv->msg.data_len = data_len;
		for(int i = 0; i < data_len; i++) {
			recv->msg.data[i] = __insn_mfspr(SPR_UDN_CA_DATA);
			//printf("udn backend received message data %d : %lu\n", i, recv->msg.data[i]);
		}
		//printf("Put one message of length %" PRIu32 " for channel %" PRIu32 " in buffer of backend.\n", recv->msg.data_len, chan_id);

		//be_state.pos = (be_state.pos + 1) % be_state.bufmsgs;
		PUT_MSG_INTO_BUFFER(&be_state);
	}
	// Maybe something new available due to udn_read - we want it immediately.
	deliver_from_backend_to_channel();
}

errval_t udn_in_chan_state_init(struct udn_in_chan_state *c, int num_msg)
{
    void *buf = malloc(num_msg * sizeof(struct udn_message));
    if(buf == NULL) {
    	return  LIB_ERR_MALLOC_FAIL;
    }
    c->buf = buf;
    c->pos = 0;
    c->avail_pos = 0;
    c->bufmsgs = num_msg;
	udn_backend_add_incoming_chan(c);
    return SYS_ERR_OK;
}

void udn_out_chan_state_init(struct udn_out_chan_state *c, coreid_t dest_core_id, uint32_t out_chan_id, uint8_t dest_asid)
{
	c->chan_id = out_chan_id;
	c->dest_asid = dest_asid;

	debug_printf("udn_impl.c out_chan_id: %d\n",(int)out_chan_id);

	int tile_width = 0;
	tile_width = get_tile_width();
	assert(tile_width > 0);
	assert(tile_width <= 8);

	// get the destination coordinate
	int x = dest_core_id % tile_width;
	int y = dest_core_id / tile_width;

    // first 7 bits are the length, which is filled per message
    // next 11 bits are y and next 11 bits are x.
    // final_route & final_direction are ignored
    int max_cpu_axis = (1 << 7) - 1;
    assert(y <= max_cpu_axis);
    assert(x <= max_cpu_axis);
    c->int_id = (y << 7) | (x << 18);
}

void udn_in_chan_state_destroy(struct udn_in_chan_state *c)
{
	assert(c->buf != NULL);
	udn_backend_remove_chan(c);
	free(c->buf);
	c->buf = NULL;
}

struct udn_message* udn_chan_impl_poll(struct udn_in_chan_state *c)
{
	// Must be locked
	assert(be_state.lock);

    if(c->pos != c->avail_pos) {
    	return &c->buf[c->avail_pos];
    } else {
    	udn_backend_impl_poll();
    	if(c->pos != c->avail_pos) {
			return &c->buf[c->avail_pos];
    	}
    	return NULL;
    }
}


struct udn_message* udn_impl_recv(struct udn_in_chan_state *c)
{
    acquire_spinlock(&be_state.lock);

    coreid_t core_id = 255;
    core_id = disp_get_core_id();
    assert(core_id < 64);
    printf("\ncore (%u) receiving udn message from chan %" PRIu32 "... \n", core_id, c->chan_id);

    struct udn_message *msg = udn_chan_impl_poll(c);
    if(msg != NULL) {
//		debug_printf("received message...");
//		debug_printf("received message type : %lu", msg->type);
//		debug_printf("received message length : %lu", msg->data_len);
//		for(int i = 0; i < msg->data_len; i++) {
//			debug_printf("received message data[%d] : %lu", i, msg->data[i]);
//		}
    	//c->avail_pos = (c->avail_pos + 1) % c->bufmsgs;
    	TAKE_MSG_FROM_BUFFER(c);
//    	printf("core %u in the channel %lu after receiving message: c->pos: %d, c->avail_pos: %d\n", core_id, c->chan_id, c->pos, c->avail_pos);
    }
    release_spinlock(&be_state.lock);
    return msg;
}

void udn_impl_send_msg(struct udn_out_chan_state *c, struct udn_message *msg)
{
    // udn_header itself is not considered in the length
    // so the UDN length is tag (chan_id) + length + type
	debug_printf("sending udn message to core(%lu,%lu)", (c->int_id >> 18) & 0b11111111111, (c->int_id >> 7) & 0b11111111111);
    udn_send(c->int_id | (msg->data_len + 3));
    //debug_printf("sent message header");
    udn_send((c->dest_asid << 24) | c->chan_id);
    //debug_printf("sent message tag (chan_id) : %" PRIu32 " ", c->chan_id);
    udn_send(msg->type);
    //debug_printf("sent message type : %lu", msg->type);
    udn_send(msg->data_len);
    //debug_printf("sent message length : %lu", msg->data_len);
    for(int i = 0; i < msg->data_len; i++) {
    	udn_send(msg->data[i]);
    	//debug_printf("sent message data[%d] : %lu", i, msg->data[i]);
    }
}

void udn_chan_lock(void)
{
	acquire_spinlock(&be_state.lock);
}

void udn_chan_unlock(void)
{
	release_spinlock(&be_state.lock);
}
