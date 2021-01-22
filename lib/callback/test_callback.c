#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include "common.h"
#include "callback.h"

#define NODE_ID 1
int control_msg_level = 0;

static int test_callback(void)
{
	struct node node;

	node.id = NODE_ID;
	init_node_cbs();
	node_online(&node);
	node_offline(&node);
	node.id++;
	node_online(&node);
	node_offline(&node);

	return 0;
}

int main(void)
{
	WARN_ON(test_callback() == -1, "Test callback fail");
	MSG(1, "callback test success\n");
	return 0;
}
