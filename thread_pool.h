/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_pool.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdarchiv <tdarchiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/23 16:17:25 by tdarchiv          #+#    #+#             */
/*   Updated: 2019/02/28 16:36:31 by tdarchiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREAD_POOL_H
# define THREAD_POOL_H

# include <stddef.h>
# include <pthread.h>
# include "work_queue.h"

struct				s_thread_pool {
	int				thread_count;
	int				unfinished_work;
	pthread_t		*thread_array;
	t_work_queue	*work_queue;
	pthread_mutex_t	queue_mutex;
	pthread_cond_t	work_available;
	pthread_cond_t	work_done;
};
typedef struct s_thread_pool	t_thread_pool;

t_thread_pool		*create_thread_pool(int thread_count);
void				thread_pool_add_work(t_thread_pool *pool, void *data,
										size_t data_size, void *(*task)(void*));
void				thread_pool_wait(t_thread_pool *thread_pool);
void				*run_task(void *arg);

#endif
