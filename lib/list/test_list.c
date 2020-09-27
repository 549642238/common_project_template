#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "list.h"

#define ELE_NUM 20

int control_msg_level = 0;

struct element {
	int data;
	struct list_head list;
};

int test_list_1(void)
{
	int i;
	LIST_HEAD(elements);
	struct element eles[ELE_NUM], *pos, *tmp_e;

	if (!list_empty(&elements)) {
		WARN("Init list must be empty");
		return -1;
	}

	for (i = 0; i < ELE_NUM; ++i) {
		eles[i].data = i;
		list_add_tail(&eles[i].list, &elements);
	}

	list_for_each_entry_safe_reverse(pos, tmp_e, &elements, list) {
		if (pos->data % 2)
			list_del_init(&pos->list);
	}

	MSG(0, "All elements from head to tail in list:");
	list_for_each_entry(pos, &elements, list) {
		MSG_NOHEAD(0, " %d", pos->data);
	}
	MSG_NOHEAD(0, "\n");

	return 0;
}

int test_list_2(void)
{
	int i, err = 0;
	struct element *pos, *tmp_e;
	struct list_head *elements, *tmp;

	elements = malloc(sizeof(*elements));
	if (!elements) {
		WARN("Cannot allocate memory for list");
		return -1;
	}

	INIT_LIST_HEAD(elements);

	for (i = 0; i < ELE_NUM; ++i) {
		struct element *e = malloc(sizeof(*e));
		if (!e) {
			WARN("Cannot allocate memory for element");
			err = -1;
			goto free_all;
		}

		e->data = i;
		if (i % 2)
			list_add(&e->list, elements);
		else
			list_add_tail(&e->list, elements);
	}

	MSG(0, "All elements from head to tail in list:");
	list_for_each(tmp, elements) {
		pos = list_entry(tmp, struct element, list);
		MSG_NOHEAD(0, " %d", pos->data);
	}
	MSG_NOHEAD(0, "\n");

free_all:
	list_for_each_entry_safe(pos, tmp_e, elements, list) {
		list_del_init(&pos->list);
		free(pos);
	}
	free(elements);
	return err;
}

int main(void)
{
	WARN_ON(test_list_1() == -1, "Test 1 fail");
	WARN_ON(test_list_2() == -1, "Test 2 fail");
	MSG(1, "list test success\n");
	return 0;
}
