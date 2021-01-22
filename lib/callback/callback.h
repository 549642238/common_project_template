#ifndef __CALLBACK_H__
#define __CALLBACK_H__

#include "list.h"

struct node {
	int id;
};

typedef void (*callback_func_t)(struct node *node);

enum callback_type {
	NODE_ONLINE = 0,
	NODE_OFFLINE = 1,
	CALLBACK_TYPE_NR,
};

struct callback {
	struct list_head list;
	unsigned int type;
	callback_func_t func;
};

void init_node_cbs(void);
void node_online(struct node *node);
void node_offline(struct node *node);

#endif /* __CALLBACK_H__ */
