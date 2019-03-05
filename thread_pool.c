/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_pool.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdarchiv <tdarchiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 16:11:53 by tdarchiv          #+#    #+#             */
/*   Updated: 2019/03/05 19:40:56 by tdarchiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "thread_pool.h"
#include "libft/libft.h"

#include <stdlib.h>
#include <stdbool.h>

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
	i = 0;
	while (i < thread_count)
	{
		pthread_create(pool->thread_array + i, NULL, run_task, pool);
		pthread_detach(pool->thread_array[i]);
		i++;
	}
	return (pool);
}

/*
** Lock queue mutex
** 		If there is unfinished_work,
** 			Wait for work_done signal
** Unlock queue mutex
*/

void			thread_pool_wait(t_thread_pool *pool)
{
	pthread_mutex_lock(&pool->queue_mutex);
	while (pool->unfinished_work)
	{
		pthread_cond_wait(&pool->work_done, &pool->queue_mutex);
	}
	pthread_mutex_unlock(&pool->queue_mutex);
}

/*
** Lock queue mutex
** 		Push work on queue
** 		Increment unfinished_work
** 		Send work_available signal
** Unlock queue mutex
*/

void			thread_pool_add_work(t_thread_pool *pool, void *data,
										size_t data_size, void *(*task)(void *))
{
	t_work_queue *new;

	new = malloc(sizeof(t_work_queue));
	new->data = malloc(data_size);
	ft_memcpy(new->data, data, data_size);
	new->task = task;
	new->next = NULL;
	pthread_mutex_lock(&pool->queue_mutex);
	push_work(&pool->work_queue, new);
	pool->unfinished_work++;
	pthread_cond_signal(&pool->work_available);
	pthread_mutex_unlock(&pool->queue_mutex);
}

/*
** Lock queue mutex
** 		If did work (in previous iteration)
** 			Decrement unfinished_work
**		If no unfinished_work
** 			Send work_done signal
** 		If no work
** 			Wait for a work_available signal
** 		Pop work from queue
** Unlock queue mutex
** Do work
*/

void			*run_task(void *arg)
{
	t_thread_pool	*pool;
	t_work_queue	*work;
	bool			did_work;

	did_work = false;
	pool = arg;
	while (42)
	{
		pthread_mutex_lock(&pool->queue_mutex);
		if (did_work)
			pool->unfinished_work--;
		if (pool->unfinished_work == 0)
			pthread_cond_signal(&pool->work_done);
		while (pool->work_queue == NULL)
			pthread_cond_wait(&pool->work_available, &pool->queue_mutex);
		work = pop_work(&pool->work_queue);
		pthread_mutex_unlock(&pool->queue_mutex);
		work->task(work->data);
		did_work = true;
		free(work->data);
		free(work);
	}
	return (NULL);
}
