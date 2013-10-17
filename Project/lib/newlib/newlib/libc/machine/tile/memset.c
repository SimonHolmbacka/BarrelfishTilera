/*
 * Copyright 2011 Tilera Corporation. All Rights Reserved.
 *
 *   Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 *    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *
 *    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
 *   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 *   STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. TILERA FURTHER DISCLAIMS ANY LIABILITY WHATSOEVER FOR INFRINGEMENT OF ANY INTELLECTUAL PROPERTY RIGHTS OF ANY THIRD PARTY.
 */

#include <arch/chip.h>






#include <string.h>
#include <stdint.h>


#undef memset

void *memset(void *s, int c, size_t n)
{
	uint32_t *out32;
	int n32;
	uint32_t v16, v32;
	uint8_t *out8 = s;
#if !CHIP_HAS_WH64()
	int ahead32;
#else
	int to_align32;
#endif

	/* Experimentation shows that a trivial tight loop is a win up until
	 * around a size of 20, where writing a word at a time starts to win.
	 */
#define BYTE_CUTOFF 20

#if BYTE_CUTOFF < 3
	/* This must be at least at least this big, or some code later
	 * on doesn't work.
	 */
#error "BYTE_CUTOFF is too small"
#endif

	if (n < BYTE_CUTOFF) {
		/* Strangely, this turns out to be the tightest way to
		 * write this loop.
		 */
		if (n != 0) {
			do {
				/* Strangely, combining these into one line
				 * performs worse.
				 */
				*out8 = c;
				out8++;
			} while (--n != 0);
		}

		return s;
	}

#if !CHIP_HAS_WH64()
	/* Use a spare issue slot to start prefetching the first cache
	 * line early. This instruction is free as the store can be buried
	 * in otherwise idle issue slots doing ALU ops.
	 */
	__insn_prefetch(out8);

	/* We prefetch the end so that a short memset that spans two cache
	 * lines gets some prefetching benefit. Again we believe this is free
	 * to issue.
	 */
	__insn_prefetch(&out8[n - 1]);
#endif /* !CHIP_HAS_WH64() */


	/* Align 'out8'. We know n >= 3 so this won't write past the end. */
	while (((uintptr_t) out8 & 3) != 0) {
		*out8++ = c;
		--n;
	}

	/* Align 'n'. */
	while (n & 3)
		out8[--n] = c;

	out32 = (uint32_t *) out8;
	n32 = n >> 2;

	/* Tile input byte out to 32 bits. */
	v16 = __insn_intlb(c, c);
	v32 = __insn_intlh(v16, v16);

	/* This must be at least 8 or the following loop doesn't work. */
#define CACHE_LINE_SIZE_IN_WORDS (CHIP_L2_LINE_SIZE() / 4)

#if !CHIP_HAS_WH64()

	ahead32 = CACHE_LINE_SIZE_IN_WORDS;

	/* We already prefetched the first and last cache lines, so
	 * we only need to do more prefetching if we are storing
	 * to more than two cache lines.
	 */
	if (n32 > CACHE_LINE_SIZE_IN_WORDS * 2) {
		int i;

		/* Prefetch the next several cache lines.
		 * This is the setup code for the software-pipelined
		 * loop below.
		 */
#define MAX_PREFETCH 5
		ahead32 = n32 & -CACHE_LINE_SIZE_IN_WORDS;
		if (ahead32 > MAX_PREFETCH * CACHE_LINE_SIZE_IN_WORDS)
			ahead32 = MAX_PREFETCH * CACHE_LINE_SIZE_IN_WORDS;

		for (i = CACHE_LINE_SIZE_IN_WORDS;
		     i < ahead32; i += CACHE_LINE_SIZE_IN_WORDS)
			__insn_prefetch(&out32[i]);
	}

	if (n32 > ahead32) {
		while (1) {
			int j;

			/* Prefetch by reading one word several cache lines
			 * ahead.  Since loads are non-blocking this will
			 * cause the full cache line to be read while we are
			 * finishing earlier cache lines.  Using a store
			 * here causes microarchitectural performance
			 * problems where a victimizing store miss goes to
			 * the head of the retry FIFO and locks the pipe for
			 * a few cycles.  So a few subsequent stores in this
			 * loop go into the retry FIFO, and then later
			 * stores see other stores to the same cache line
			 * are already in the retry FIFO and themselves go
			 * into the retry FIFO, filling it up and grinding
			 * to a halt waiting for the original miss to be
			 * satisfied.
			 */
			__insn_prefetch(&out32[ahead32]);

#if CACHE_LINE_SIZE_IN_WORDS % 4 != 0
#error "Unhandled CACHE_LINE_SIZE_IN_WORDS"
#endif

			n32 -= CACHE_LINE_SIZE_IN_WORDS;

			/* Save icache space by only partially unrolling
			 * this loop.
			 */
			for (j = CACHE_LINE_SIZE_IN_WORDS / 4; j > 0; j--) {
				*out32++ = v32;
				*out32++ = v32;
				*out32++ = v32;
				*out32++ = v32;
			}

			/* To save compiled code size, reuse this loop even
			 * when we run out of prefetching to do by dropping
			 * ahead32 down.
			 */
			if (n32 <= ahead32) {
				/* Not even a full cache line left,
				 * so stop now.
				 */
				if (n32 < CACHE_LINE_SIZE_IN_WORDS)
					break;

				/* Choose a small enough value that we don't
				 * prefetch past the end.  There's no sense
				 * in touching cache lines we don't have to.
				 */
				ahead32 = CACHE_LINE_SIZE_IN_WORDS - 1;
			}
		}
	}

#else /* CHIP_HAS_WH64() */

	/* Determine how many words we need to emit before the 'out32'
	 * pointer becomes aligned modulo the cache line size.
	 */
	to_align32 =
		(-((uintptr_t)out32 >> 2)) & (CACHE_LINE_SIZE_IN_WORDS - 1);

	/* Only bother aligning and using wh64 if there is at least
	 * one full cache line to process.  This check also prevents
	 * overrunning the end of the buffer with alignment words.
	 */
	if (to_align32 <= n32 - CACHE_LINE_SIZE_IN_WORDS) {
		int lines_left;

		/* Align out32 mod the cache line size so we can use wh64. */
		n32 -= to_align32;
		for (; to_align32 != 0; to_align32--) {
			*out32 = v32;
			out32++;
		}

		/* Use unsigned divide to turn this into a right shift. */
		lines_left = (unsigned)n32 / CACHE_LINE_SIZE_IN_WORDS;

		do {
			/* Only wh64 a few lines at a time, so we don't
			 * exceed the maximum number of victim lines.
			 */
			int x = ((lines_left < CHIP_MAX_OUTSTANDING_VICTIMS())
				  ? lines_left
				  : CHIP_MAX_OUTSTANDING_VICTIMS());
			uint32_t *wh = out32;
			int i = x;
			int j;

			lines_left -= x;

			do {
				__insn_wh64(wh);
				wh += CACHE_LINE_SIZE_IN_WORDS;
			} while (--i);

			for (j = x * (CACHE_LINE_SIZE_IN_WORDS / 4);
			     j != 0; j--) {
				*out32++ = v32;
				*out32++ = v32;
				*out32++ = v32;
				*out32++ = v32;
			}
		} while (lines_left != 0);

		/* We processed all full lines above, so only this many
		 * words remain to be processed.
		 */
		n32 &= CACHE_LINE_SIZE_IN_WORDS - 1;
	}

#endif /* CHIP_HAS_WH64() */

	/* Now handle any leftover values. */
	if (n32 != 0) {
		do {
			*out32 = v32;
			out32++;
		} while (--n32 != 0);
	}

	return s;
}






