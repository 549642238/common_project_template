#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "bitmap.h"

/**
 * __ffs - find first bit in word.
 * @word: The word to search
 *
 * Undefined if no bit exists, so code should check against 0 first.
 */
static inline unsigned long __ffs(unsigned long word)
{
	int num = 0;

	if (BITS_PER_LONG == 64) {
		if ((word & 0xffffffff) == 0) {
			num += 32;
			word >>= 32;
		}
	}
	if ((word & 0xffff) == 0) {
		num += 16;
		word >>= 16;
	}
	if ((word & 0xff) == 0) {
		num += 8;
		word >>= 8;
	}
	if ((word & 0xf) == 0) {
		num += 4;
		word >>= 4;
	}
	if ((word & 0x3) == 0) {
		num += 2;
		word >>= 2;
	}
	if ((word & 0x1) == 0)
		num += 1;
	return num;
}

/*
 * This is a common helper function for find_next_bit, find_next_zero_bit, and
 * find_next_and_bit. The differences are:
 *  - The "invert" argument, which is XORed with each fetched word before
 *    searching it for one bits.
 *  - The optional "addr2", which is anded with "addr1" if present.
 */
static unsigned long _find_next_bit(const unsigned long *addr1,
				const unsigned long *addr2, unsigned long nbits,
				unsigned long start, unsigned long invert)
{
	unsigned long tmp;

	if (start >= nbits)
		return nbits;

	tmp = addr1[BIT_WORD(start)];
	if (addr2)
		tmp &= addr2[BIT_WORD(start)];
	tmp ^= invert;

	/* Handle 1st word. */
	tmp &= BITMAP_FIRST_WORD_MASK(start);
	start = round_down(start, BITS_PER_LONG);

	while (!tmp) {
		start += BITS_PER_LONG;
		if (start >= nbits)
			return nbits;

		tmp = addr1[BIT_WORD(start)];
		if (addr2)
			tmp &= addr2[BIT_WORD(start)];
		tmp ^= invert;
	}

	return min(start + __ffs(tmp), nbits);
}

/*
 * find_next_bit - Find the next set bit in a memory region.
 * @addr: the bitmap in memory
 * @size: the length of bitmap
 * @offset: start position to find
 */
unsigned long find_next_bit(const unsigned long *addr, unsigned long size,
				unsigned long offset)
{
	return _find_next_bit(addr, NULL, size, offset, 0UL);
}

/*
 * find_next_bit - Find the next zero bit in a memory region.
 * @addr: the bitmap in memory
 * @size: the length of bitmap
 * @offset: start position to find
 */
unsigned long find_next_zero_bit(const unsigned long *addr, unsigned long size,
				unsigned long offset)
{
	return _find_next_bit(addr, NULL, size, offset, ~0UL);
}

/**
 * set_bit - Set a bit in memory
 * @addr: the bitmap in memory
 * @nr: the bit to set
 */
void set_bit(unsigned long *addr, unsigned long nr)
{
	addr[nr / BITS_PER_LONG] |= BIT_MASK(nr);
}

/**
 * set_bit - Set a bit in memory
 * @addr: the bitmap in memory
 * @nr: the bit to set
 */
void clear_bit(unsigned long *addr, unsigned long nr)
{
	addr[nr / BITS_PER_LONG] &= ~(BIT_MASK(nr));
}

/**
 * test_and_set_bit - Set a bit and return its old value
 * @addr: the bitmap in memory
 * @nr: the bit to set
 */
int test_and_set_bit(unsigned long *addr, unsigned long nr)
{
	unsigned long mask = BIT_MASK(nr);
	unsigned long *p = ((unsigned long *)addr) + BIT_WORD(nr);
	unsigned long old;

	old = *p;
	*p = old | mask;

	return (old & mask) != 0;
}

/**
 * test_and_clear_bit - Clear a bit and return its old value
 * @addr: the bitmap in memory
 * @nr: the bit to clear
 */
int test_and_clear_bit(unsigned long *addr, unsigned long nr)
{
	unsigned long mask = BIT_MASK(nr);
	unsigned long *p = ((unsigned long *)addr) + BIT_WORD(nr);
	unsigned long old;

	old = *p;
	*p = old & ~mask;

	return (old & mask) != 0;
}

/**
 * test_bit - Determine whether a bit is set
 * @addr: the bitmap in memory
 * @nr: bit number to test
 */
int test_bit(const unsigned long *addr, unsigned long nr)
{
	return 1UL & (addr[BIT_WORD(nr)] >> (nr & (BITS_PER_LONG-1)));
}

/**
 * bitmap_alloc - Allocate bitmap
 * @nbits: Number of bits
 */
unsigned long *bitmap_alloc(unsigned long nbits)
{
	return calloc(1, BITS_TO_LONGS(nbits) * sizeof(unsigned long));
}

/**
 * bitmap_free - Free bitmap
 * @addr: Bitmap to be freed
 */
void bitmap_free(unsigned long *addr)
{
	free(addr);
}
