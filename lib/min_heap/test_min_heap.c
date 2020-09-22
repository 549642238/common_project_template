#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "common.h"
#include "min_heap.h"

int control_msg_level = 0;

static int less_than(const void *lhs, const void *rhs)
{
	return *(int *)lhs < *(int *)rhs;
}

static int greater_than(const void *lhs, const void *rhs)
{
	return *(int *)lhs > *(int *)rhs;
}

static void swap_ints(void *lhs, void *rhs)
{
	int temp = *(int *)lhs;

	*(int *)lhs = *(int *)rhs;
	*(int *)rhs = temp;
}

static int pop_verify_heap(int min_heap,
				struct min_heap *heap,
				const struct min_heap_callbacks *funcs)
{
	int *values = heap->data;
	int err = 0;
	int last;

	last = values[0];
	min_heap_pop(heap, funcs);
	while (heap->nr > 0) {
		if (min_heap) {
			if (last > values[0]) {
				MSG(2, "error: expected %d <= %d\n", last,
					values[0]);
				err++;
			}
		} else {
			if (last < values[0]) {
				MSG(2, "error: expected %d >= %d\n", last,
					values[0]);
				err++;
			}
		}
		last = values[0];
		min_heap_pop(heap, funcs);
	}
	return err;
}

static int test_heapify_all(int min_heap)
{
	int values[] = { 3, 1, 2, 4, 0x8000000, 0x7FFFFFF, 0,
			 -3, -1, -2, -4, 0x8000000, 0x7FFFFFF };
	struct min_heap heap = {
		.data = values,
		.nr = ARRAY_SIZE(values),
		.size =  ARRAY_SIZE(values),
	};
	struct min_heap_callbacks funcs = {
		.elem_size = sizeof(int),
		.less = min_heap ? less_than : greater_than,
		.swp = swap_ints,
	};
	int i, err;

	/* Test with known set of values. */
	min_heapify_all(&heap, &funcs);
	err = pop_verify_heap(min_heap, &heap, &funcs);


	/* Test with randomly generated values. */
	heap.nr = ARRAY_SIZE(values);
	srand((unsigned int)time(NULL));
	for (i = 0; i < heap.nr; i++)
		values[i] = rand();

	min_heapify_all(&heap, &funcs);
	err += pop_verify_heap(min_heap, &heap, &funcs);

	return err;
}

static int test_heap_push(int min_heap)
{
	const int data[] = { 3, 1, 2, 4, 0x80000000, 0x7FFFFFFF, 0,
			     -3, -1, -2, -4, 0x80000000, 0x7FFFFFFF };
	int values[ARRAY_SIZE(data)];
	struct min_heap heap = {
		.data = values,
		.nr = 0,
		.size =  ARRAY_SIZE(values),
	};
	struct min_heap_callbacks funcs = {
		.elem_size = sizeof(int),
		.less = min_heap ? less_than : greater_than,
		.swp = swap_ints,
	};
	int i, temp, err;

	/* Test with known set of values copied from data. */
	for (i = 0; i < ARRAY_SIZE(data); i++)
		min_heap_push(&heap, &data[i], &funcs);

	err = pop_verify_heap(min_heap, &heap, &funcs);

	/* Test with randomly generated values. */
	srand((unsigned int)time(NULL));
	while (heap.nr < heap.size) {
		temp = rand();
		min_heap_push(&heap, &temp, &funcs);
	}
	err += pop_verify_heap(min_heap, &heap, &funcs);

	return err;
}

static int test_heap_pop_push(int min_heap)
{
	const int data[] = { 3, 1, 2, 4, 0x80000000, 0x7FFFFFFF, 0,
			     -3, -1, -2, -4, 0x80000000, 0x7FFFFFFF };
	int values[ARRAY_SIZE(data)];
	struct min_heap heap = {
		.data = values,
		.nr = 0,
		.size =  ARRAY_SIZE(values),
	};
	struct min_heap_callbacks funcs = {
		.elem_size = sizeof(int),
		.less = min_heap ? less_than : greater_than,
		.swp = swap_ints,
	};
	int i, temp, err;

	/* Fill values with data to pop and replace. */
	temp = min_heap ? 0x80000000 : 0x7FFFFFFF;
	for (i = 0; i < ARRAY_SIZE(data); i++)
		min_heap_push(&heap, &temp, &funcs);

	/* Test with known set of values copied from data. */
	for (i = 0; i < ARRAY_SIZE(data); i++)
		min_heap_pop_push(&heap, &data[i], &funcs);

	err = pop_verify_heap(min_heap, &heap, &funcs);

	heap.nr = 0;
	for (i = 0; i < ARRAY_SIZE(data); i++)
		min_heap_push(&heap, &temp, &funcs);

	/* Test with randomly generated values. */
	srand((unsigned int)time(NULL));
	for (i = 0; i < ARRAY_SIZE(data); i++) {
		temp = rand();
		min_heap_pop_push(&heap, &temp, &funcs);
	}
	err += pop_verify_heap(min_heap, &heap, &funcs);

	return err;
}

int main(void)
{
	int err = 0;

	err += test_heapify_all(1);
	err += test_heapify_all(0);
	err += test_heap_push(1);
	err += test_heap_push(0);
	err += test_heap_pop_push(1);
	err += test_heap_pop_push(0);
	if (err) {
		MSG(2, "test failed with %d errors\n", err);
		return -1;
	}
	MSG(1, "min_heap test success\n");

	return 0;
}
