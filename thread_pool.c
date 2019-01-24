#include "thread_pool.h"

#include <stdlib.h>
#include <memory.h>

t_work_queue	*pop_work(t_work_queue **queue);
void			push_work(t_work_queue **queue, t_work_queue *new_work);
void			*run_task(void *arg);

#include <stdio.h>
t_thread_pool	*create_thread_pool(int thread_count)
{
	int				i;
	t_thread_pool	*pool;

	pool = malloc(sizeof(t_thread_pool));
	pool->thread_count = thread_count;
	pool->working_count = 0;
	pool->work_queue = NULL;
	pool->thread_array = malloc(sizeof(pthread_t) * thread_count);
	pool->working_count_mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	pool->queue_mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	pool->work_available = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
	pool->work_done = (pthread_cond_t)PTHREAD_COND_INITIALIZER;

	struct hax
	{
		t_thread_pool	*pool;
		int				thread_id;
	};
	static struct hax hax_array[128];

	i = 0;
	while (i < thread_count)
	{
		hax_array[i].pool = pool;
		hax_array[i].thread_id = i;
		pthread_create(pool->thread_array + i, NULL, run_task, hax_array + i);
//		pthread_create(pool->thread_array + i, NULL, run_task, pool);
		i++;
	}
	return (pool);
}

/*
** Lock queue mutex
** If queue still has work, wait for signal (mutex is unlocked while waiting)
** Unlock queue mutex
**/

void thread_pool_wait(t_thread_pool* pool)
{
	puts("Acquiring queue mutex...");
//	pthread_mutex_lock(&pool->queue_mutex);
	pthread_mutex_lock(&pool->working_count_mutex);
	//TODO: How do we wait on 2 values that are behind 2 mutexes if cond_wait only release 1 ?
	while (pool->work_queue != NULL || pool->working_count != 0)
	{
		puts("Sleeping waiting for all work done!");
		pthread_cond_wait(&pool->work_done, &pool->working_count_mutex);
	}
	pthread_mutex_unlock(&pool->working_count_mutex);
//	pthread_mutex_unlock(&pool->queue_mutex);
	puts("WAIT DONE");
}

/*
** Lock queue mutex
** push_work()
** If queue was empty, send signal
** Unlock queue mutex
**/

void	thread_pool_add_work(t_thread_pool *pool, void *data, size_t data_size, void *(*task)(void *))
{
	t_work_queue *new;

	new = malloc(sizeof(t_work_queue));
	new->data = malloc(data_size);
	memcpy(new->data, data, data_size);
	new->task = task;
	new->next = NULL;
	pthread_mutex_lock(&pool->queue_mutex);
	push_work(&pool->work_queue, new);
	puts("New work pushed");
//	if (pool->work_queue == new)
	pthread_cond_signal(&pool->work_available);
//		pthread_cond_broadcast(&pool->work_available);
	pthread_mutex_unlock(&pool->queue_mutex);
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

/*
** Lock queue mutex
** If no work, wait for a signal (mutex is unlocked while waiting)
** Pop work from queue
** Unlock queue mutex
*/
#include <stdio.h>
void	*run_task(void *arg)
{
	t_thread_pool	*pool;
	t_work_queue	*work;
	int				id;

	pool = *(t_thread_pool**)arg;
	id = *(int*)(arg + 8);
	char tmp[256];
	sprintf(tmp, "Thread  %d ", id);
	pthread_setname_np(tmp);

//	pool = arg;
	while (42)
	{
		printf(" %d  Acquiring queue mutex...\n", id);
		pthread_mutex_lock(&pool->queue_mutex);
		while (pool->work_queue == NULL)
		{
			printf(" %d  Sleeping waiting for work !\n", id);
//			fflush(stdout);
			pthread_cond_wait(&pool->work_available, &pool->queue_mutex);
		}
		printf(" %d  We got work !\n", id);
		work = pop_work(&pool->work_queue);
		pthread_mutex_unlock(&pool->queue_mutex);

		pthread_mutex_lock(&pool->working_count_mutex);
		pool->working_count++;
		pthread_mutex_unlock(&pool->working_count_mutex);

		printf(" %d  Task running...\n", id);
		work->task(work->data);
		printf(" %d  Task end !\n", id);

		pthread_mutex_lock(&pool->working_count_mutex);
		pool->working_count--;
//		pthread_mutex_lock(&pool->queue_mutex);
		if (pool->working_count == 0)
		{
			printf(" %d  All threads idle !\n", id);
			pthread_cond_signal(&pool->work_done);
		}
		pthread_mutex_unlock(&pool->working_count_mutex);
//		pthread_mutex_unlock(&pool->queue_mutex);

		free(work->data);
		free(work);
	}
	return NULL;
}
#pragma clang diagnostic pop

void	push_work(t_work_queue **queue, t_work_queue *new_work)
{
	t_work_queue *work_queue;
	t_work_queue *ptr;

	work_queue = *queue;
	if (work_queue == NULL)
	{
		*queue = new_work;
		return;
	}
	ptr = work_queue;
	while (ptr->next != NULL)
		ptr = ptr->next;
	ptr->next = new_work;
}

t_work_queue	*pop_work(t_work_queue **queue)
{
	t_work_queue *work_queue;
	t_work_queue *work_to_pop;

	work_queue = *queue;
	if (work_queue == NULL)
		return NULL;
	work_to_pop = work_queue;
	work_queue = work_queue->next;
	*queue = work_queue;
	return (work_to_pop);
}