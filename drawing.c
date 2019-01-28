#include "drawing.h"
#include "app.h"

#include <pthread.h>
#include <stdbool.h>

bool	is_inside_rect(t_rect rect, int x, int y)
{
	return ((x > rect.origin.x) && (x < (rect.origin.x + rect.size.x)) && (y > rect.origin.y) && (y < rect.origin.y + rect.size.y));
}

#if 1
int	get_mandelbrot_value(t_float2 c, int depth_max)
{
	t_float2	z;
//	t_float2	z_old;
	int			depth;

	depth = 0;
	z = c;
//	z_old = z;
//	while (((z.x < 2 && z.x > -2) && (z.y < 2 && z.y > -2)) && (depth < depth_max))
	while (((z.x * z.x) + (z.y * z.y) < 4) && (depth < depth_max))
	{
		float z_y_temp = 2 * z.x * z.y + c.y;
//		z.y = 2 * z.x * z.y + c.y;
//		z.x = z_squared.x - z_squared.y + c.x;
		z.x = (z.x * z.x) - (z.y * z.y) + c.x;
		z.y = z_y_temp;
		depth++;
//		if (z.x == z_old.x && z.y == z_old.y)
//		{
//			depth = depth_max;
//			break;
//		}
//		z_old = z;
	}
	if (depth >= depth_max)
		return (0);
	return (depth);
}
#else
int	get_mandelbrot_value(t_float2 c, int depth_max)
{
	t_float2	z;
	t_float2	z_squared;
	int			depth;

	depth = 0;
	z = (t_float2){};
	z_squared = (t_float2){z.x * z.x, z.y * z.y};
	while (((z_squared.x + z_squared.y) < 4) && (depth < depth_max))
	{
		z.y = 2 * z.x * z.y + c.y;
		z.x = z_squared.x - z_squared.y + c.x;
		depth++;
		z_squared = (t_float2){z.x * z.x, z.y * z.y};
	}
	if (depth == depth_max)
		return (0);
	return (depth - 1);
}
#endif

#include <stdio.h>
#include <assert.h>
#include <math.h>

void	prepare_threads_chunks(thread_config *thread_list, int chunk_count, int chunk_height, t_rect rect);
void	draw_iter_region_debug(t_config config, t_rect rect, t_surface16 iter_frame, int thread_id);

void	draw_iter_region(t_config config, t_rect rect, t_surface16 iter_frame)
{
	draw_iter_region_debug(config, rect, iter_frame, -1);
}

void	draw_iter_region_debug(t_config config, t_rect rect, t_surface16 iter_frame, int thread_id)
{
	int			x;
	int			y;
	t_float2	c;
	int			depth;

//	printf("draw_iter_region\n\0origin: %4g %4g,  size: %4g %4g\n", rect.origin.x, rect.origin.y, rect.size.x, rect.size.y);
	y = (int)rect.origin.y;
	while (y < (rect.origin.y + rect.size.y))
	{
		x = (int)rect.origin.x;
		c.y = (y / iter_frame.size.y) * (config.z_size.y) + (config.z_min.y);
		while (x < (rect.origin.x + rect.size.x))
		{
			c.x = (x / iter_frame.size.x) * (config.z_size.x) + (config.z_min.x);
//			c.x = (x / surface.size.x) * (config.z_max.x - config.z_min.x) + (config.z_min.x);
			depth = get_mandelbrot_value(c, config.depth_max);
			iter_frame.iter[(int)(y * iter_frame.size.x + x)] = depth;
			if (thread_id >= 0)
				iter_frame.iter[(int)(y * iter_frame.size.x + x)] |= (thread_id * 2) << 8;
			x++;
		}
		y++;
	}
}

void	*draw_task(void *param)
{
	thread_config	conf;

	conf = *(thread_config*)param;
//	draw_iter_region(conf.config, conf.rect, (t_surface16) {conf.pixels, conf.win_size});
	draw_iter_region_debug(conf.config, conf.rect, (t_surface16) {conf.pixels, conf.win_size}, conf.thread_id + 1);
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
		i++;
	}
}

#define X_IS_MAJOR_AXIS 1
#define Y_IS_MAJOR_AXIS 0

void prepare_threads_chunks(thread_config *thread_list, int chunk_count, int chunk_height, t_rect rect)
{
	int		i;
	bool	major_axis;
	t_rect	chunk;

	major_axis = (rect.size.x > rect.size.y);
	chunk.size.x = rect.size.x;
	if (chunk_height == 0)
		chunk_height = (int) (rect.size.y / chunk_count);
	printf("prepare_threads_chunks %d\norigin: %4g %4g,  size: %4g %4g\n", chunk_count, rect.origin.x, rect.origin.y, rect.size.x, rect.size.y);

	i = 0;
	while (i < chunk_count)
	{
		chunk.origin.x = rect.origin.x;
		chunk.origin.y = rect.origin.y + (i * chunk_height);

		chunk.size.y = chunk_height;
		int overshoot = (chunk.origin.y + chunk_height) - (rect.origin.y + rect.size.y);
		printf("overshoot %d\n", overshoot);
		if (overshoot > 0)
			chunk.size.y -= overshoot;

		printf("Thread: %d   ", i);
		printf("origin: %4g %4g,  size: %4g %4g\n", chunk.origin.x, chunk.origin.y, chunk.size.x, chunk.size.y);

		if (major_axis == X_IS_MAJOR_AXIS)
		{
		}
		thread_list[i].rect = chunk;
		i++;
	}
}

void	draw_iter_region_parallel(t_config config, t_surface16 iter_frame, int thread_count, t_rect rect)
{
	if (rect.size.x == 0 || rect.size.y == 0)
		return;

	printf("draw_iter_region_parallel\norigin: %4g %4g,  size: %4g %4g\n", rect.origin.x, rect.origin.y, rect.size.x, rect.size.y);

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

	printf("draw_iter_region_parallel_pool\norigin: %4g %4g,  size: %4g %4g\n", rect.origin.x, rect.origin.y, rect.size.x, rect.size.y);

	int				i;
	thread_config	thread_config_list[MAX_THREAD] = {};
	int chunk_count = get_adjusted_chunk_count(rect, CHUNK_HEIGHT);
	assert(chunk_count < MAX_THREAD);

	prepare_threads(config, iter_frame, thread_config_list, chunk_count);
	prepare_threads_chunks(thread_config_list, chunk_count, CHUNK_HEIGHT, rect);
	i = 0;
	while (i < chunk_count)
	{
		thread_pool_add_work(pool, thread_config_list + i, sizeof(thread_config), draw_task);
		i++;
	}

	thread_pool_wait(pool);
}

int	get_adjusted_chunk_count(t_rect rect, int chunk_height)
{
	int		major_axis;
	float	chunk_width;
	float	temp_chunk_count;
	int		chunk_count;

	major_axis = (rect.size.x > rect.size.y);
	if (1 || major_axis == X_IS_MAJOR_AXIS)
	{
		if (rect.size.y < chunk_height)
			return (int)(rect.size.y);
		chunk_width = rect.size.x;
		temp_chunk_count = (rect.size.y / chunk_height);
		printf("rect height: %d, target chunk height: %d -> %d \\ %d = %f\n", (int) rect.size.y, chunk_height,
			   (int) rect.size.y, chunk_height, temp_chunk_count);
		chunk_count = (int)(temp_chunk_count);
		if (chunk_count * chunk_height < rect.size.y)
			chunk_count++;
		printf("Final chunk count: %d\n", chunk_count);
	}
	return chunk_count;
}