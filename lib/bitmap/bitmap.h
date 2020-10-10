#ifndef __BITMAP_H__
#define __BITMAP_H__

#define BIT_MASK(nr)		(1UL << ((nr) % BITS_PER_LONG))
#define BIT_WORD(nr)		((nr) / BITS_PER_LONG)
#define BITMAP_FIRST_WORD_MASK(start) (~0UL << ((start) & (BITS_PER_LONG - 1)))
#define BITMAP_LAST_WORD_MASK(nbits) (~0UL >> (-(nbits) & (BITS_PER_LONG - 1)))
#define BITS_TO_LONGS(nr)	DIV_ROUND_UP(nr, BITS_PER_LONG)

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

#endif /* __BITMAP_H__ */
