#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>

#include "common.h"
#include "bitmap.h"

#define ELE_NUM 100

int control_msg_level = 0;

int main(void)
{
	int set_6 = 0, clear_12 = 0;
	unsigned int pos;
	unsigned long *bitmap = bitmap_alloc(ELE_NUM);
	if (!bitmap) {
	      WARN("%s", strerror(errno));
	      return -1;
	}
	MSG(1, "Construct bitmap success[%d]\n", ELE_NUM);

	MSG(1, "Set bit (mod 2 = 0)\n");
	for (pos = 0; pos < ELE_NUM; pos += 2) {
		set_bit(bitmap, pos);
	}

	MSG(1, "Clear bit (mod 3 = 0)\n");
	for (pos = 0; pos < ELE_NUM; pos += 3) {
		clear_bit(bitmap, pos);
	}

	MSG(1, "Test and set bit (mod 6 = 0)\n");
	for (pos = 0; pos < ELE_NUM; pos += 6) {
		if (!test_and_set_bit(bitmap, pos)) {
			set_6++;
		}
	}

	MSG(1, "Test and clear bit (mod 12 = 0)\n");
	for (pos = 0; pos < ELE_NUM; pos += 12) {
		if (test_and_clear_bit(bitmap, pos)) {
			clear_12++;
		}
	}

	MSG(0, "Bitmap: ");
	for (pos = 0; pos < ELE_NUM; ++pos) {
		MSG_NOHEAD(0, "%d", test_bit(bitmap, pos) ? 1: 0);
	}
	MSG_NOHEAD(0, "\n");
	MSG(1, "set_6 %d, clear_12 %d\n", set_6, clear_12);

	if ((pos = find_next_bit(bitmap, ELE_NUM, 0)) < ELE_NUM)
		MSG(1, "Next set bit %u\n", pos);
	if ((pos = find_next_zero_bit(bitmap, ELE_NUM, 0)) < ELE_NUM)
		MSG(1, "Next zero bit %u\n", pos);

	bitmap_free(bitmap);

	return 0;
}
