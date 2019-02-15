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
	short i;

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

void	draw_iter_region_parallel(t_config config, t_surface16 iter_frame, t_rect rect)
{
	if (rect.size.x == 0 || rect.size.y == 0)
		return;

//	printf("draw_iter_region_parallel\n");
	int				i;
	pthread_t		thread_list[MAX_THREAD] = {};
	thread_config	thread_config_list[MAX_THREAD] = {};

	int chunk_count = get_chunk_count(rect.size.y, config.lines_per_chunk);
	prepare_threads(config, iter_frame, thread_config_list, chunk_count);
	prepare_threads_chunks(thread_config_list, chunk_count, config.lines_per_chunk, rect);
	i = 0;
	while (i < chunk_count)
	{
		pthread_create(thread_list + i, NULL, draw_task, thread_config_list + i);
		i++;
	}

	i = 0;
	while (i < chunk_count)
	{
		pthread_join(thread_list[i], NULL);
		i++;
	}
}


void	draw_iter_region_parallel_pool(t_config config, t_thread_pool *pool, t_surface16 iter_frame, t_rect rect)
{
	if (rect.size.x == 0 || rect.size.y == 0)
		return;

	printf("draw_iter_region_parallel_pool\n\0\norigin: %4g %4g,  size: %4g %4g\n", rect.origin.x, rect.origin.y, rect.size.x, rect.size.y);

	int				i;
	thread_config	thread_config_list[MAX_THREAD] = {};
	int chunk_count = get_chunk_count(rect.size.y, config.lines_per_chunk);
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

/*
** Divide the region in even chunks
** If the region is not evenly divisible, add a chunk to cover the full region
** prepare_threads_chunks() will make sure that the last chunk will not go oob
** If we dont even have enough lines for a full chunk, report 1 line per chunk
**/

int	get_chunk_count(int line_count, int lines_per_chunk)
{
	int	chunk_count;

	if (line_count < lines_per_chunk)
		return (line_count);
	chunk_count = line_count / lines_per_chunk;
	if ((chunk_count * lines_per_chunk) < line_count)
		chunk_count++;
	return chunk_count;
}