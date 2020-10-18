#ifndef __WORKQUEUE_H__
#define __WORKQUEUE_H__

#include "list.h"

#define WORKQUEUE_NAME_LEN	40	/* Max workqueue name length */
#define WORKER_NAME_LEN		60	/* Max worker thread name length */
#define MULTIPLE_CPUS		3	/* = nr_max_worker / nr_cpu */

struct work_struct;

typedef void (*work_func_t)(struct work_struct *work);

struct work_struct {
	// Will be assigned to wurkqueue after work being inserted into wq
	struct workqueue *wq;
	// An entry in workqueue work list
	struct list_head entry;
	// Specified function in work
	work_func_t func;
};

struct workqueue {
	int alive;				// Whether workqueue is working
	int nr_worker;				// # of workers
	// # of workers plus # of workers to be created
	int ref_count;
	int nr_max_worker;			// Maximum # of workers
	int nr_idle_worker;			// # of idle workers
	// Maximum # of idle workers. Workers out of range will be released
	int nr_max_idle_worker;
	char name[WORKQUEUE_NAME_LEN];		// Workqueue name

	// All works on work_list should be processed by workers
	struct list_head work_list;

	pthread_mutex_t lock;			// Protect above data
	pthread_cond_t cond;			// Used to sync workqueue ops
};

extern int queue_work(struct workqueue* wq, struct work_struct *work);
extern void flush_work(struct work_struct *work);
extern int init_workqueue(struct workqueue* wq, const char *name);
extern void destroy_workqueue(struct workqueue* wq);

#define INIT_WORK(_work, _func)						\
	do {								\
		INIT_LIST_HEAD(&(_work)->entry);			\
		(_work)->func = (_func);				\
		(_work)->wq = NULL;					\
	} while (0)

#endif /* __WORKQUEUE_H__ */
