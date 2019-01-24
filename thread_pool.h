#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <stddef.h>
#include <pthread.h>

struct	s_work_queue {
	void				*data;
	void				*(*task)(void*);
	struct s_work_queue	*next;
};
typedef struct s_work_queue	t_work_queue;

struct	s_thread_pool {
	int				thread_count;
	int				working_count;
	pthread_t		*thread_array;
	t_work_queue	*work_queue;
	pthread_mutex_t	queue_mutex;
	pthread_cond_t	work_available;
	pthread_mutex_t	working_count_mutex;
	pthread_cond_t	work_done;
};
typedef struct s_thread_pool	t_thread_pool;

t_thread_pool	*create_thread_pool(int thread_count);
void			thread_pool_add_work(t_thread_pool *pool, void *data, size_t data_size, void *(*task)(void *));
void			thread_pool_wait(t_thread_pool* thread_pool);

#endif