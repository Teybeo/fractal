/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multithreading.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdarchiv <tdarchiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/27 18:50:56 by tdarchiv          #+#    #+#             */
/*   Updated: 2019/02/27 20:25:19 by tdarchiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "multithreading.h"
#include "drawing.h"

#include <pthread.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>

void	*draw_task(void *param)
{
	t_task_data	task_data;

	task_data = *(t_task_data*)param;
	compute_region(task_data.config, task_data.rect, task_data.frame);
	return (NULL);
}

void	prepare_tasks(t_config config, t_frame frame,
						t_task_data *task_data, int thread_count)
{
	short i;

	i = 0;
	while (i < thread_count)
	{
		task_data[i].config = config;
		task_data[i].frame = frame;
		task_data[i].thread_id = i;
		i++;
	}
}

void	prepare_task_chunks(t_task_data *task_data, int chunk_count,
							int chunk_height, t_rect rect)
{
	int		i;
	t_rect	chunk;
	double	overshoot;

	chunk.size.x = rect.size.x;
	i = 0;
	while (i < chunk_count)
	{
		chunk.origin.x = rect.origin.x;
		chunk.origin.y = rect.origin.y + (i * chunk_height);
		chunk.size.y = chunk_height;
		overshoot = (chunk.origin.y + chunk_height)
					- (rect.origin.y + rect.size.y);
		if (overshoot > 0)
			chunk.size.y -= overshoot;
		task_data[i].rect = chunk;
		i++;
	}
}

void	compute_region_parallel(t_config cfg, t_thread_pool *pool,
		t_frame frame, t_rect rect)
{
	int				i;
	t_task_data		task_data[MAX_TASK_COUNT];
	int				chunk_count;

	if (rect.size.x == 0 || rect.size.y == 0)
		return ;
	chunk_count = get_chunk_count(rect.size.y, cfg.lines_per_chunk);
	assert(chunk_count < MAX_TASK_COUNT);
	prepare_tasks(cfg, frame, task_data, chunk_count);
	prepare_task_chunks(task_data, chunk_count, cfg.lines_per_chunk, rect);
	i = 0;
	while (i < chunk_count)
	{
		thread_pool_add_work(pool, task_data + i, sizeof(t_task_data),
								draw_task);
		i++;
	}
	thread_pool_wait(pool);
}

/*
** Divide the region in even chunks
** If the region is not evenly divisible, add a chunk to cover the full region
** prepare_task_chunks() will make sure that the last chunk will not go oob
** If we dont even have enough lines for a full chunk, report 1 line per chunk
*/

int		get_chunk_count(int line_count, int lines_per_chunk)
{
	int	chunk_count;

	if (line_count < lines_per_chunk)
		return (line_count);
	chunk_count = line_count / lines_per_chunk;
	if ((chunk_count * lines_per_chunk) < line_count)
		chunk_count++;
	return (chunk_count);
}
