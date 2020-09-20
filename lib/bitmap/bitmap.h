#ifndef __HEAP_H__
#define __HEAP_H__

#define min(a, b) ((a) < (b) ? (a) : (b))

#define __round_mask(x, y) ((__typeof__(x))((y)-1))
#define round_down(x, y) ((x) & ~__round_mask(x, y))
#define DIV_ROUND_UP(n,d) (((n) + (d) - 1) / (d))

#define BITS_PER_BYTE		8
#define BITS_PER_LONG		(BITS_PER_BYTE * sizeof(long))
#define BIT_MASK(nr)		(1UL << ((nr) % BITS_PER_LONG))
#define BIT_WORD(nr)		((nr) / BITS_PER_LONG)
#define BITMAP_FIRST_WORD_MASK(start) (~0UL << ((start) & (BITS_PER_LONG - 1)))
#define BITMAP_LAST_WORD_MASK(nbits) (~0UL >> (-(nbits) & (BITS_PER_LONG - 1)))
#define BITS_TO_LONGS(nr)	DIV_ROUND_UP(nr, BITS_PER_LONG)

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

unsigned long find_next_bit(const unsigned long *addr, unsigned long size,
				unsigned long offset);
unsigned long find_next_zero_bit(const unsigned long *addr, unsigned long size,
				unsigned long offset);
void set_bit(unsigned long *addr, unsigned long nr);
void clear_bit(unsigned long *addr, unsigned long nr);
int test_and_set_bit(unsigned long *addr, unsigned long nr);
int test_and_clear_bit(unsigned long *addr, unsigned long nr);
int test_bit(const unsigned long *addr, unsigned long nr);
unsigned long *bitmap_alloc(unsigned long nbits);
void bitmap_free(unsigned long *addr);

#endif
