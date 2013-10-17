/*
 * lmp_char_arch.h
 *
 *  Created on: Aug 22, 2012
 *      Author: robert
 */

#ifndef ARCH_TILEPRO_BARRELFISH_LMP_CHAN_H
#define ARCH_TILEPRO_BARRELFISH_LMP_CHAN_H

#include <barrelfish/syscall_arch.h>
#include <barrelfish/caddr.h>
#include <barrelfish_kpi/lmp.h>


/**
 * \brief Send a message on the given LMP endpoint, if possible
 *
 * Non-blocking, may fail if there is no space in the receiver's endpoint.
 *
 * \param ep Remote endpoint
 * \param flags LMP send flags
 * \param send_cap (Optional) capability to send with the message
 * \param length_words Length of the message in words; payload beyond this
 *                      size will not be delivered
 * \param arg1..N Message payload
 */
static inline errval_t lmp_ep_send(struct capref ep, lmp_send_flags_t flags,
                                   struct capref send_cap, uint8_t length_words,
                                   uintptr_t arg1, uintptr_t arg2, uintptr_t arg3,
                                   uintptr_t arg4, uintptr_t arg5, uintptr_t arg6,
                                   uintptr_t arg7)
{
    uint8_t send_bits = get_cap_valid_bits(send_cap);
    capaddr_t send_cptr = get_cap_addr(send_cap) >> (CPTR_BITS - send_bits);

    uint8_t invoke_bits = get_cap_valid_bits(ep);
    capaddr_t invoke_cptr = get_cap_addr(ep) >> (CPTR_BITS - invoke_bits);

//    printf("In lmp_ep_send():\n");
//    printf("invoke cap  : 0x%.8lX\n", invoke_cptr);
//    printf("invoke bits : %u\n", invoke_bits);
//    printf("send cap    : 0x%.8lX\n", send_cptr);
//    printf("send bits   : %u\n", send_bits);

	return syscall11(SYSCALL_INVOKE,
					 send_cptr,
					 (length_words << 20) | ((flags & 0xf) << 16) | (send_bits << 8) | invoke_bits,
					 invoke_cptr,
					 arg1, arg2, arg3, arg4, arg5, arg6, arg7).error;
}

#define lmp_ep_send7(ep, flags, send_cap, a, b, c, d, e, f, g) \
	lmp_ep_send((ep), (flags), (send_cap), 7, (a), (b), (c), (d), (e), (f), (g))
#define lmp_ep_send6(ep, flags, send_cap, a, b, c, d, e, f) \
	lmp_ep_send((ep), (flags), (send_cap), 6, (a), (b), (c), (d), (e), (f), 0)
#define lmp_ep_send5(ep, flags, send_cap, a, b, c, d, e) \
	lmp_ep_send((ep), (flags), (send_cap), 5, (a), (b), (c), (d), (e), 0, 0)
#define lmp_ep_send4(ep, flags, send_cap, a, b, c, d) \
	lmp_ep_send((ep), (flags), (send_cap), 4, (a), (b), (c), (d), 0, 0, 0)
#define lmp_ep_send3(ep, flags, send_cap, a, b, c) \
	lmp_ep_send((ep), (flags), (send_cap), 3, (a), (b), (c), 0, 0, 0, 0)
#define lmp_ep_send2(ep, flags, send_cap, a, b) \
	lmp_ep_send((ep), (flags), (send_cap), 2, (a), (b), 0, 0, 0, 0, 0)
#define lmp_ep_send1(ep, flags, send_cap, a) \
	lmp_ep_send((ep), (flags), (send_cap), 1, (a), 0, 0, 0, 0, 0, 0)
#define lmp_ep_send0(ep, flags, send_cap) \
	lmp_ep_send((ep), (flags), (send_cap), 0, 0, 0, 0, 0, 0, 0, 0)

#define lmp_chan_send(lc, flags, send_cap, len, a, b, c, d, e, f, g) \
	lmp_ep_send((lc)->remote_cap, (flags), (send_cap), (len), (a), (b), (c), (d), (e), (f), (g))

#define lmp_chan_send7(lc, flags, send_cap, a, b, c, d, e, f, g) \
	lmp_ep_send7((lc)->remote_cap, (flags), (send_cap), (a), (b), (c), (d), (e), (f), (g))
#define lmp_chan_send6(lc, flags, send_cap, a, b, c, d, e, f) \
	lmp_ep_send6((lc)->remote_cap, (flags), (send_cap), (a), (b), (c), (d), (e), (f))
#define lmp_chan_send5(lc, flags, send_cap, a, b, c, d, e) \
	lmp_ep_send5((lc)->remote_cap, (flags), (send_cap), (a), (b), (c), (d), (e))
#define lmp_chan_send4(lc, flags, send_cap, a, b, c, d) \
	lmp_ep_send4((lc)->remote_cap, (flags), (send_cap), (a), (b), (c), (d))
#define lmp_chan_send3(lc, flags, send_cap, a, b, c) \
	lmp_ep_send3((lc)->remote_cap, (flags), (send_cap), (a), (b), (c))
#define lmp_chan_send2(lc, flags, send_cap, a, b) \
	lmp_ep_send2((lc)->remote_cap, (flags), (send_cap), (a), (b))
#define lmp_chan_send1(lc, flags, send_cap, a) \
	lmp_ep_send1((lc)->remote_cap, (flags), (send_cap), (a))
#define lmp_chan_send0(lc, flags, send_cap) \
	lmp_ep_send0((lc)->remote_cap, (flags), (send_cap))

#endif /* ARCH_TILEPRO_BARRELFISH_LMP_CHAN_H */
