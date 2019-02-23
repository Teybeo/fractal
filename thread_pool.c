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
	pool->unfinished_work = 0;
	pool->work_queue = NULL;
	pool->thread_array = malloc(sizeof(pthread_t) * thread_count);
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
//		pthread_create(pool->thread_array + i, NULL, run_task, pool);
		pthread_create(pool->thread_array + i, NULL, run_task, hax_array + i);
		pthread_detach(pool->thread_array[i]);
		i++;
	}
	return (pool);
}

/*
** Lock queue mutex
** 		If there is unfinished_work,
 * 			Wait for work_done signal
** Unlock queue mutex
**/

void thread_pool_wait(t_thread_pool* pool)
{
	DEBUG_PRINT("%s\n", "Acquiring queue mutex...");
	pthread_mutex_lock(&pool->queue_mutex);
	while (pool->unfinished_work)
	{
		DEBUG_PRINT("%s\n", "Sleeping waiting for all work done!");
		pthread_cond_wait(&pool->work_done, &pool->queue_mutex);
	}
	pthread_mutex_unlock(&pool->queue_mutex);
	DEBUG_PRINT("%s\n", "WAIT DONE");
}

/*
** Lock queue mutex
** 		Push work on queue
** 		Increment unfinished_work
** 		Send work_available signal
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
		pool->unfinished_work++;
		DEBUG_PRINT("%s\n", "New work pushed");
		pthread_cond_signal(&pool->work_available);
	pthread_mutex_unlock(&pool->queue_mutex);
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

/*
** Lock queue mutex
** 		If no work
 * 			Wait for a work_available signal
** 		Pop work from queue
** Unlock queue mutex
 *
 * Do work
 *
 * Lock queue mutex
 * 		Decrement unfinished_work
 *		If no unfinished_work,
 * 			Send work_done signal
 * Unlock queue mutex
*/

#include <stdio.h>
#include <stdbool.h>

void	*run_task(void *arg)
{
	t_thread_pool	*pool;
	t_work_queue	*work;
	int				id;
	bool			did_work;

	pool = *(t_thread_pool**)arg;
	id = *(int*)(arg + 8);
	char tmp[256];
	sprintf(tmp, "Thread  %d ", id);
	pthread_setname_np(tmp);

	did_work = false;
//	pool = arg;
	while (42)
	{
		DEBUG_PRINT(" %d  Acquiring queue mutex...\n", id);
		pthread_mutex_lock(&pool->queue_mutex);
			if (did_work)
				pool->unfinished_work --;
			if (pool->unfinished_work == 0)
			{
				DEBUG_PRINT(" %d  No more work !\n", id);
				pthread_cond_signal(&pool->work_done);
			}
			while (pool->work_queue == NULL)
			{
				DEBUG_PRINT(" %d  Sleeping waiting for work !\n", id);
				pthread_cond_wait(&pool->work_available, &pool->queue_mutex);
			}
			DEBUG_PRINT(" %d  We got work !\n", id);
			work = pop_work(&pool->work_queue);
		pthread_mutex_unlock(&pool->queue_mutex);

		DEBUG_PRINT(" %d  Task running...\n", id);
		work->task(work->data);
		DEBUG_PRINT(" %d  Task end !\n", id);
		did_work = true;
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
