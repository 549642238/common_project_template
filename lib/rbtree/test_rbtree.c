#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "common.h"
#include "rbtree.h"

int control_msg_level = 0;

struct element {
	int key;
	struct rb_node node;
};

static void insert(struct element *e, struct rb_root *root)
{
	struct element *tmp;
	struct rb_node **new = &(root->rb_node), *parent = NULL;
	int key = e->key;

	while (*new) {
		parent = *new;
		tmp = rb_entry(parent, struct element, node);

		if (key < tmp->key)
			new = &parent->rb_left;
		else
			new = &parent->rb_right;
	}

	rb_link_node(&e->node, parent, new);
	rb_insert_color(&e->node, root);
}

static void erase(struct element *e, struct rb_root *root)
{
	rb_erase(&e->node, root);
}

static struct element *find(int key, struct rb_root *root)
{
	struct element *tmp;
	struct rb_node *n = root->rb_node;

	while (n) {
		tmp = rb_entry(n, struct element, node);

		if (key == tmp->key)
			return tmp;

		if (key < tmp->key)
			n = n->rb_left;
		else
			n = n->rb_right;
	}

	return NULL;
}

static int test_rbtree_1(void)
{
#if defined(ELE_NUM)
	#undef ELE_NUM
#endif
	#define ELE_NUM	3
	int i;
	struct rb_root elements = RB_ROOT;
	struct element eles[ELE_NUM];
	struct element *cur, *n;

	for (i = 0; i < ELE_NUM; ++i) {
		eles[i].key = i - 1;
		insert(&eles[i], &elements);
	}

	MSG(0, "Post-order traversal in rbtree:");
	rbtree_postorder_for_each_entry_safe(cur, n, &elements, node) {
		MSG_NOHEAD(0, " %d", cur->key);
		erase(cur, &elements);
	}
	MSG_NOHEAD(0, "\n");

	if (!RB_EMPTY_ROOT(&elements)) {
		WARN("The rbtree should be empty");
		return -1;
	}

	return 0;
}

static int test_rbtree_2(void)
{
#if defined(ELE_NUM)
	#undef ELE_NUM
#endif
	#define ELE_NUM 10
	int i;
	struct rb_root elements = RB_ROOT;
	struct element eles[ELE_NUM], *e;
	struct rb_node *rn;

	for (i = 0; i < ELE_NUM; ++i) {
		eles[i].key = pow(-1, i + 1) * i;
		insert(&eles[i], &elements);
	}

	if (find(0, &elements) == NULL) {
		WARN("key 0 should be exist in rbtree");
		return -1;
	}

	MSG(0, "In-order traversal in rbtree:");
	rn = rb_first(&elements);
	while (rn) {
		e = rb_entry(rn, struct element, node);
		MSG_NOHEAD(0, " %d", e->key);
		rn = rb_next(rn);
	}
	MSG_NOHEAD(0, "\n");

	if (RB_EMPTY_ROOT(&elements)) {
		WARN("The rbtree should not be empty");
		return -1;
	}

	return 0;
}

int main(void)
{
	WARN_ON(test_rbtree_1() == -1, "Test 1 fail");
	WARN_ON(test_rbtree_2() == -1, "Test 2 fail");
	MSG(1, "rbtree test success\n");
	return 0;
}
