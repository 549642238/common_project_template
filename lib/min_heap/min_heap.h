#ifndef __MIN_HEAP_H__
#define __MIN_HEAP_H__

/**
 * struct min_heap - Data structure to hold a min-heap.
 * @data: Start of array holding the heap elements.
 * @nr: Number of elements currently in the heap.
 * @size: Maximum number of elements that can be held in current storage.
 */
struct min_heap {
	void *data;
	int nr;
	int size;
};

/**
 * struct min_heap_callbacks - Data/functions to customise the min_heap.
 * @elem_size: The nr of each element in bytes.
 * @less: Partial order function for this heap.
 * @swp: Swap elements function.
 */
struct min_heap_callbacks {
	int elem_size;
	int (*less)(const void *lhs, const void *rhs);
	void (*swp)(void *lhs, void *rhs);
};


void min_heapify_all(struct min_heap *heap,
		const struct min_heap_callbacks *func);
void min_heap_pop(struct min_heap *heap,
		const struct min_heap_callbacks *func);
void min_heap_pop_push(struct min_heap *heap,
		const void *element,
		const struct min_heap_callbacks *func);
void min_heap_push(struct min_heap *heap, const void *element,
		const struct min_heap_callbacks *func);

#endif
