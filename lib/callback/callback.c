#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "callback.h"

static inline void node_online_print(struct node *node)
{
	MSG(0, "NODE %d ONLINE\n", node->id);
}

static inline void node_online_echo(struct node *node)
{
	MSG(0, "NODE %d ONLINE ECHO\n", node->id);
}

static inline void node_offline_print(struct node *node)
{
	MSG(0, "NODE %d OFFLINE\n", node->id);
}

static struct callback head_cb[] = {
	{
		.type = NODE_ONLINE,
		.func = node_online_print,
	},
	{
		.type = NODE_ONLINE,
		.func = node_online_echo,
	},
	{
		.type = NODE_OFFLINE,
		.func = node_offline_print,
	},
};

static struct list_head node_cbs[CALLBACK_TYPE_NR];

void init_node_cbs(void)
{
	int i;
	struct callback *cb;

	for (i = 0; i < ARRAY_SIZE(node_cbs); ++i)
		INIT_LIST_HEAD(&node_cbs[i]);

	for (i = 0; i < ARRAY_SIZE(head_cb); ++i) {
		cb = &head_cb[i];
		if (cb->type >= CALLBACK_TYPE_NR)
			continue;

		INIT_LIST_HEAD(&cb->list);
		list_add_tail(&cb->list, &node_cbs[cb->type]);
	}
}

static inline void call_func(struct node *node, enum callback_type type)
{
	struct callback *cb;

	list_for_each_entry(cb, &node_cbs[type], list)
		cb->func(node);
}

void node_online(struct node *node)
{
	call_func(node, NODE_ONLINE);
}

void node_offline(struct node *node)
{
	call_func(node, NODE_OFFLINE);
}
