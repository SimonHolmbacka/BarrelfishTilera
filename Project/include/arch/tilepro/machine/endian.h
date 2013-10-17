/*
 * endian.h
 *
 *  Created on: Aug 22, 2012
 *      Author: robert
 */

#ifndef _MACHINE_ENDIAN_H_
#define _MACHINE_ENDIAN_H_

/*
 * Definitions for byte order, according to byte significance from low
 * address to high.
 */
#define	_LITTLE_ENDIAN	1234	/* LSB first: i386, vax */
#define	_BIG_ENDIAN	4321	/* MSB first: 68000, ibm, net */
#define	_PDP_ENDIAN	3412	/* LSB first in word, MSW first in long */

#define	_BYTE_ORDER	_LITTLE_ENDIAN

#if __BSD_VISIBLE
#define	LITTLE_ENDIAN	_LITTLE_ENDIAN
#define	BIG_ENDIAN	_BIG_ENDIAN
#define	PDP_ENDIAN	_PDP_ENDIAN
#define	BYTE_ORDER	_BYTE_ORDER
#endif

// Little endian, low word first
#define	_QUAD_HIGHWORD 1
#define	_QUAD_LOWWORD 0

#define __ntohl(x)        (__bswap32(x))
#define __ntohs(x)        (__bswap16(x))
#define __htonl(x)        (__bswap32(x))
#define __htons(x)        (__bswap16(x))

static __inline __uint64_t
__bswap64(__uint64_t src)
{
	__uint32_t dest_h;
	__uint32_t dest_l;

	__uint32_t src_h = src >> 32;
	__uint32_t src_l = src ; //& 0xffffffff;

	// Tilera byteswap primitive for lower and upper part.
	__asm __volatile("bytex %[dest_h], %[src_l]\n"
			"bytex %[dest_l], %[src_h]\n"
			: [dest_h] "=r" (dest_h), [dest_l] "=r" (dest_l)
			: [src_h] "r" (src_h), [src_l] "r" (src_l));

	return ((__uint64_t)dest_h << 32) | (__uint64_t)dest_l;
}

static __inline __uint32_t
__bswap32(__uint32_t src)
{
	__uint32_t dest;

	// Tilera byteswap primitive
	__asm __volatile("bytex %[dest], %[src]"
			: [dest] "=r" (dest)
			: [src] "r" (src));

	return dest;
}

static __inline __uint16_t
__bswap16(__uint16_t src) {
	__uint32_t dest;

	// Tilera byteswap primitive + shift right 16
	__asm __volatile("bytex %[dest], %[src]\n"
			"shri %[dest], %[dest], 16"
			: [dest] "=r" (dest)
			: [src] "r" (src));

	return (__uint16_t)dest;
}

#endif /* _MACHINE_ENDIAN_H_ */
