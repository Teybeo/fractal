#include "multithreading.h"
#include "app.h"
#include "drawing.h"

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

void	prepare_threads_chunks(thread_config *thread_list, int chunk_count, int chunk_height, t_rect rect);

void	*draw_task(void *param)
{
	thread_config	cfg;
	t_surface16		iter_frame;

	cfg = *(thread_config*)param;
	iter_frame = (t_surface16) {cfg.pixels, cfg.win_size};
	draw_iter_region(cfg.config, cfg.rect, iter_frame, cfg.chunk_mask);
	return NULL;
}

void	prepare_threads(t_config config, t_surface16 iter_frame, thread_config *thread_list, int thread_count)
{
	int i;
	i = 0;
	while (i < thread_count)
	{
		thread_list[i].win_size = iter_frame.size;
		thread_list[i].config = config;
		thread_list[i].pixels = iter_frame.iter;
		thread_list[i].thread_id = i;
		thread_list[i].chunk_mask = config.show_chunks && (i % 2);
		i++;
	}
}

void prepare_threads_chunks(thread_config *thread_list, int chunk_count, int chunk_height, t_rect rect)
{
	int		i;
	t_rect	chunk;

	chunk.size.x = rect.size.x;
	if (chunk_height == 0)
		chunk_height = (int) (rect.size.y / chunk_count);
//	printf("prepare_threads_chunks %d\norigin: %4g %4g,  size: %4g %4g\n", chunk_count, rect.origin.x, rect.origin.y, rect.size.x, rect.size.y);

	i = 0;
	while (i < chunk_count)
	{
		chunk.origin.x = rect.origin.x;
		chunk.origin.y = rect.origin.y + (i * chunk_height);

		chunk.size.y = chunk_height;
		int overshoot = (chunk.origin.y + chunk_height) - (rect.origin.y + rect.size.y);
		if (overshoot > 0)
			chunk.size.y -= overshoot;

//		printf("Thread: %d   ", i);
//		printf("origin: %4g %4g,  size: %4g %4g\n", chunk.origin.x, chunk.origin.y, chunk.size.x, chunk.size.y);

		thread_list[i].rect = chunk;
		i++;
	}
}

void	draw_iter_region_parallel(t_config config, t_surface16 iter_frame, int thread_count, t_rect rect)
{
	if (rect.size.x == 0 || rect.size.y == 0)
		return;

//	printf("draw_iter_region_parallel\n");
	int				i;
	pthread_t		thread_list[MAX_THREAD] = {};
	thread_config	thread_config_list[MAX_THREAD] = {};

	if (rect.size.y < thread_count)
		thread_count = (int) rect.size.y;
	prepare_threads(config, iter_frame, thread_config_list, thread_count);
	prepare_threads_chunks(thread_config_list, thread_count, 0, rect);
	i = 0;
	while (i < thread_count)
	{
		pthread_create(thread_list + i, NULL, draw_task, thread_config_list + i);
		i++;
	}

	i = 0;
	while (i < thread_count)
	{
		pthread_join(thread_list[i], NULL);
		i++;
	}
}

int	get_adjusted_chunk_count(t_rect rect, int chunk_height);

void	draw_iter_region_parallel_pool(t_config config, t_thread_pool *pool, t_surface16 iter_frame, t_rect rect)
{
	if (rect.size.x == 0 || rect.size.y == 0)
		return;

	printf("draw_iter_region_parallel_pool\n\0\norigin: %4g %4g,  size: %4g %4g\n", rect.origin.x, rect.origin.y, rect.size.x, rect.size.y);

	int				i;
	thread_config	thread_config_list[MAX_THREAD] = {};
	int chunk_count = get_adjusted_chunk_count(rect, config.lines_per_chunk);
	assert(chunk_count < MAX_THREAD);

	printf("chunk_count: %d\n", chunk_count);

	prepare_threads(config, iter_frame, thread_config_list, chunk_count);
	prepare_threads_chunks(thread_config_list, chunk_count, config.lines_per_chunk, rect);
	i = 0;
	while (i < chunk_count)
	{
		thread_pool_add_work(pool, thread_config_list + i, sizeof(thread_config), draw_task);
		i++;
	}

	thread_pool_wait(pool);
	printf("draw_iter_region_parallel_pool DONE\n");
}

int	get_adjusted_chunk_count(t_rect rect, int chunk_height)
{
	float	temp_chunk_count;
	int		chunk_count;

	if (rect.size.y < chunk_height)
		return (int)(rect.size.y);
	temp_chunk_count = (rect.size.y / chunk_height);
	chunk_count = (int)(temp_chunk_count);
	if ((chunk_count * chunk_height) < rect.size.y)
		chunk_count++;
	return chunk_count;
}