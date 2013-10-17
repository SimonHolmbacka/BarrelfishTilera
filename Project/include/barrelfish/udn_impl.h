/*
 * udn_impl.h
 *
 *  Created on: Feb 1, 2013
 *      Author: robert
 */

#ifndef UDN_IMPL_H_
#define UDN_IMPL_H_

#define TAKE_MSG_FROM_BUFFER(x) ((x)->avail_pos = ((x)->avail_pos + 1) % (x)->bufmsgs)
#include <arch/udn.h>
#include <barrelfish_kpi/spinlocks_arch.h>

#include <assert.h>

#define UDN_PAYLOAD_WORDS 64

#define UDN_RECV_HEADER_BYTES (3 * sizeof(udn_msg_word_t)) // header + tag + channel
#define UDN_RECV_LENGTH         (UDN_MSG_BYTES + UDN_RECV_HEADER_BYTES)

typedef uint32_t udn_msg_word_t;
typedef uint16_t udn_index_t;

struct udn_message {
	uint32_t type;  // message type defined in backend
    uint32_t data_len;  // the actual data length
    udn_msg_word_t data[UDN_PAYLOAD_WORDS];
};

/**
 * \brief State of a (one-way) UDN channel
 */
struct udn_in_chan_state {
    uint32_t           	chan_id;        ///< the channel id, unique per core
    struct udn_message	*buf;           ///< Ring buffer
    udn_index_t        	pos;            ///< Current position
    udn_index_t        	avail_pos;      ///< position of the last available message
    udn_index_t        	bufmsgs;        ///< Buffer size in messages
};

struct udn_out_chan_state {
    uint32_t           chan_id;        ///< the channel id, unique per core, for demultiplexing the message
    uint32_t           int_id;         ///< internal id - used to create header, contains destination coordinates
    uint8_t            dest_asid;      ///< identify which dispatcher can receive the message
};

/**
 * \brief Initizalize the UDN system
 */
void udn_impl_init(void);

/*
 * add an incoming chan to the backend
 */
void udn_backend_add_incoming_chan(struct udn_in_chan_state *c);

/**
 * \brief Initialize UDN channel state
 *
 * The channel-state structure and buffer must already be allocated.
 *
 * \param       c       Pointer to channel-state structure to initialize.
 * \param       num_msg Number of messages, which should be able to store at the same time
 */
errval_t udn_in_chan_state_init(struct udn_in_chan_state *c, int num_msg);

/*
 * \brief Initialize UDN channel state
 */
void udn_out_chan_state_init(struct udn_out_chan_state *c, coreid_t dest_core_id, uint32_t out_chan_id, uint8_t dest_asid);

void udn_in_chan_state_destroy(struct udn_in_chan_state *c);

/**
 * \brief Return if a message is outstanding on 'c'.
 *
 * \param c     Pointer to UDN channel-state structure.
 *
 * \return      Pointer to UDN message, otherwise NULL.
 */
struct udn_message* udn_chan_impl_poll(struct udn_in_chan_state *c);

/**
 * \brief Return pointer to a message if outstanding on 'c' and
 * advance pointer.
 *
 * \param c     Pointer to UDN channel-state structure.
 *
 * \return Pointer to message if outstanding, or NULL.
 */
struct udn_message* udn_impl_recv(struct udn_in_chan_state *c);

/**
 * \brief send a message
 *
 * \param c     Pointer to UDN channel-state structure.
 * \param msg   The message
 *
 * \return Pointer to message if outstanding, or NULL.
 */
void udn_impl_send_msg(struct udn_out_chan_state *c, struct udn_message *msg);

void udn_chan_lock(void);

void udn_chan_unlock(void);

#endif /* UDN_IMPL_H_ */
