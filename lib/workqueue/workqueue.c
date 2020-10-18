/*
 * This is a kernel-simplified implementation for workqueu.
 * Users may use it like a threads pool to process works asynchronously.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <sys/prctl.h>
#include <sys/sysinfo.h>

#include "common.h"
#include "workqueue.h"

static void *worker_thread(void *wq);

/**
 * Handle two thing:
 * 1. Process one work which is waiting in workqueue, then delete the finished
 *    work from workqueue.
 * 2. Create more workers when there are too many workers, the additional
 *    creating workers will be released in their threads.
 */
static inline void process_one_work(struct work_struct *work,
							struct workqueue *wq)
{
	list_del_init(&work->entry);
	pthread_mutex_unlock(&wq->lock);
	work->func(work);
	work->wq = NULL;
	pthread_mutex_lock(&wq->lock);

	/**
	 * Dynamicly create worker when there is no idle workers (which
	 * means work_cnt exceeds nr_worker).
	 */
	if (wq->alive && !list_empty(&wq->work_list) && !wq->nr_idle_worker &&
					wq->nr_worker < wq->nr_max_worker) {
		pthread_t tid;

		wq->ref_count++;
		if (pthread_create(&tid, NULL, worker_thread, wq)) {
			wq->ref_count--;
			WARN("Cannot create additional worker for workqueue");
		}
	}
}

static void *worker_thread(void *__wq)
{
	char name[WORKER_NAME_LEN];
	struct workqueue *wq = __wq;

	pthread_mutex_lock(&wq->lock);
	wq->nr_idle_worker++;
	snprintf(name, WORKER_NAME_LEN, "%s/t-%d", wq->name, wq->nr_worker++);
	prctl(PR_SET_NAME, name);

wake_up:
	wq->nr_idle_worker--;
	while (!list_empty(&wq->work_list)) {
		struct work_struct *work =
			list_first_entry(&wq->work_list,
						struct work_struct, entry);
		process_one_work(work, wq);
	}

	/**
	 * Alive check must be after work_list empty check, in case of leaving
	 * un-processed work in workqueue.
	 */
	if (!wq->alive) {
		wq->nr_worker--;
		wq->ref_count--;
		pthread_mutex_unlock(&wq->lock);
		return NULL;
	}

	wq->nr_idle_worker++;
	if (wq->nr_idle_worker > wq->nr_max_idle_worker) {
		wq->nr_idle_worker--;
		wq->nr_worker--;
		wq->ref_count--;
		pthread_mutex_unlock(&wq->lock);
		return NULL;
	}
	pthread_cond_wait(&wq->cond, &wq->lock);
	goto wake_up;
}

/**
 * Insert a work into workqueue.
 * Note: never insert one work twice into workqueue.
 * Return: 0 for success, others for error.
 */
int queue_work(struct workqueue* wq, struct work_struct *work)
{
	pthread_mutex_lock(&wq->lock);
	if (!wq->alive) {
		pthread_mutex_unlock(&wq->lock);
		WARN("Dead workqueue, cannot insert new work");
		return -1;
	}

	list_add_tail(&work->entry, &wq->work_list);
	work->wq = wq;

	if (wq->nr_idle_worker)
		pthread_cond_signal(&wq->cond);
	pthread_mutex_unlock(&wq->lock);

	return 0;
}

/**
 * Wait until the given work is finished.
 */
void flush_work(struct work_struct *work)
{
	while (work->wq);
}

static inline void wait_worker_exit(struct workqueue* wq)
{
	// Make sure there is not worker thread alive
	while (wq->ref_count)
		pthread_cond_signal(&wq->cond);

	// Make sure the last worker thread exited
	pthread_mutex_lock(&wq->lock);
	pthread_mutex_unlock(&wq->lock);
}

/**
 * Init a workqueue.
 * @wq: the given workqueue to be initialized
 * @name: workqueue name
 */
int init_workqueue(struct workqueue* wq, const char *name)
{
	pthread_t tid;
	int nr_worker, i;

	if (strlen(name) > WORKQUEUE_NAME_LEN) {
		WARN("Workqueue name is too long (max %d)", WORKQUEUE_NAME_LEN);
		return -1;
	}
	strncpy(wq->name, name, WORKQUEUE_NAME_LEN);

	wq->alive = 1;
	wq->nr_worker = 0;
	wq->ref_count = 0;
	nr_worker = get_nprocs();
	wq->nr_max_worker = nr_worker * MULTIPLE_CPUS;
	wq->nr_idle_worker = 0;
	wq->nr_max_idle_worker = nr_worker;

	if (pthread_mutex_init(&wq->lock, NULL)) {
		WARN("Cannot init mutex for workqueue");
		return -1;
	}
	if (pthread_cond_init(&wq->cond, NULL)) {
		WARN("Cannot init cond for workqueue");
		goto out_destroy_lock;
	}
	INIT_LIST_HEAD(&wq->work_list);

	for (i = 0; i < nr_worker; ++i) {
		wq->ref_count++;
		if (pthread_create(&tid, NULL, worker_thread, wq)) {
			wq->ref_count--;
			WARN("Cannot create worker for workqueue");
			goto out_destroy_cond;
		}
	}

	return 0;

out_destroy_cond:
	while (wq->nr_worker)
		pthread_cond_signal(&wq->cond);
	pthread_cond_destroy(&wq->cond);
	wait_worker_exit(wq);
out_destroy_lock:
	pthread_mutex_destroy(&wq->lock);

	return -1;
}

/**
 * Destroy the workqueue after all works done and wokers exited.
 */
void destroy_workqueue(struct workqueue* wq)
{
	pthread_mutex_lock(&wq->lock);
	wq->alive = 0;
	pthread_mutex_unlock(&wq->lock);

	while (!list_empty(&wq->work_list) || wq->nr_worker) {
		pthread_cond_signal(&wq->cond);
	}
	wait_worker_exit(wq);
	WARN_ON(wq->nr_idle_worker, "There are %d idle workers in workqueue",
							wq->nr_idle_worker);

	pthread_cond_destroy(&wq->cond);
	pthread_mutex_destroy(&wq->lock);
}
