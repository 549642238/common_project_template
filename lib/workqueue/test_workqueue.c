#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include "common.h"
#include "workqueue.h"

#define WORK_QUEUE_NAME	"my_workqueue"
#define WORK_COUNT	200

int control_msg_level = 0;

struct element {
	int data;
	struct work_struct work;
};

static void work_func(struct work_struct *work)
{
	struct element *e = container_of(work, struct element, work);
	MSG(0, "Process work, get data %d\n", e->data);
	sleep(1);
}

static int test_workqueue(void)
{
	int i;
	struct workqueue wq;
	struct element eles[WORK_COUNT];

	if (init_workqueue(&wq, WORK_QUEUE_NAME)) {
		WARN("Init workqueue fail");
		return -1;
	}

	for (i = 0; i < WORK_COUNT; ++i) {
		eles[i].data = i;
		INIT_WORK(&eles[i].work, work_func);
		if (queue_work(&wq, &eles[i].work)) {
			WARN("Cannot insert work");
			return -1;
		}

		if (i % 50 == 0)
			flush_work(&eles[i].work);
	}

	sleep(3);
	destroy_workqueue(&wq);

	return 0;
}

static int test_workqueue_2(void)
{
	struct workqueue wq;
	struct element e;

	if (init_workqueue(&wq, WORK_QUEUE_NAME)) {
		WARN("Init workqueue fail");
		return -1;
	}

	e.data = 1000;
	INIT_WORK(&e.work, work_func);
	queue_work(&wq, &e.work);
	queue_work(&wq, &e.work);
	queue_work(&wq, &e.work);
	queue_work(&wq, &e.work);

	flush_work(&e.work);
	destroy_workqueue(&wq);

	return 0;
}

int main(void)
{
	WARN_ON(test_workqueue() == -1, "Test workqueue fail");
	WARN_ON(test_workqueue_2() == -1, "Test workqueue_2 fail");
	MSG(1, "workqueue test success\n");
	return 0;
}
