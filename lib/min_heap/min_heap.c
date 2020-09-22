#include <stdio.h>
#include <string.h>

#include "common.h"
#include "min_heap.h"

/* Sift the element at pos down the heap. */
static inline void min_heapify(struct min_heap *heap, int pos,
		const struct min_heap_callbacks *func)
{
	void *left, *right, *parent, *smallest;
	void *data = heap->data;

	for (;;) {
		if (pos * 2 + 1 >= heap->nr)
			break;

		left = data + ((pos * 2 + 1) * func->elem_size);
		parent = data + (pos * func->elem_size);
		smallest = parent;
		if (func->less(left, smallest))
			smallest = left;

		if (pos * 2 + 2 < heap->nr) {
			right = data + ((pos * 2 + 2) * func->elem_size);
			if (func->less(right, smallest))
				smallest = right;
		}
		if (smallest == parent)
			break;
		func->swp(smallest, parent);
		if (smallest == left)
			pos = (pos * 2) + 1;
		else
			pos = (pos * 2) + 2;
	}
}

/* Floyd's approach to heapification that is O(nr). */
void min_heapify_all(struct min_heap *heap,
		const struct min_heap_callbacks *func)
{
	int i;

	for (i = heap->nr / 2; i >= 0; i--)
		min_heapify(heap, i, func);
}

/* Remove minimum element from the heap, O(log2(nr)). */
void min_heap_pop(struct min_heap *heap,
		const struct min_heap_callbacks *func)
{
	void *data = heap->data;

	if (heap->nr <= 0) {
		WARN("Popping an empty heap");
		return;
	}

	/* Place last element at the root (position 0) and then sift down. */
	heap->nr--;
	memcpy(data, data + (heap->nr * func->elem_size), func->elem_size);
	min_heapify(heap, 0, func);
}

/*
 * Remove the minimum element and then push the given element. The
 * implementation performs 1 sift (O(log2(nr))) and is therefore more
 * efficient than a pop followed by a push that does 2.
 */
void min_heap_pop_push(struct min_heap *heap,
		const void *element,
		const struct min_heap_callbacks *func)
{
	memcpy(heap->data, element, func->elem_size);
	min_heapify(heap, 0, func);
}

/* Push an element on to the heap, O(log2(nr)). */
void min_heap_push(struct min_heap *heap, const void *element,
		const struct min_heap_callbacks *func)
{
	void *data = heap->data;
	void *child, *parent;
	int pos;

	if (heap->nr >= heap->size) {
		WARN("Pushing on a full heap");
		return;
	}

	/* Place at the end of data. */
	pos = heap->nr;
	memcpy(data + (pos * func->elem_size), element, func->elem_size);
	heap->nr++;

	/* Sift child at pos up. */
	for (; pos > 0; pos = (pos - 1) / 2) {
		child = data + (pos * func->elem_size);
		parent = data + ((pos - 1) / 2) * func->elem_size;
		if (func->less(parent, child))
			break;
		func->swp(parent, child);
	}
}
