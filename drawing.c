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

void prepare_threads_continued(thread_config *thread_list, int thread_count, t_rect rect);

void	draw_iter_region(t_config config, t_rect rect, t_surface16 iter_frame)
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
			x++;
		}
		y++;
	}
}

void	*draw_task(void *param)
{
	thread_config	conf;

	t_rect rect;
	conf = *(thread_config*)param;
	rect = conf.rect;
	if (rect.size.x != -1)
		draw_iter_region(conf.config, rect, (t_surface16) {conf.pixels, conf.win_size});
	else
	{
		rect.origin.y = conf.first_line;
		rect.origin.x = 0;
		rect.size.y = conf.last_line - conf.first_line;
		rect.size.x = conf.win_size.x - 0;
		draw_iter_region(conf.config, rect, (t_surface16) {conf.pixels, conf.win_size});
	}
	return NULL;
}

void	prepare_threads(t_config config, t_surface16 iter_frame, thread_config *thread_list, int thread_count)
{
	int i;
	i = 0;
	while (i < thread_count)
	{
		thread_list[i].win_size = iter_frame.size;
		thread_list[i].first_line = i * (iter_frame.size.y / thread_count);
		thread_list[i].last_line = (i + 1) * (iter_frame.size.y / thread_count);
		thread_list[i].config = config;
		thread_list[i].pixels = iter_frame.iter;
		thread_list[i].thread_id = i;
		thread_list[i].rect.size.x = -1;
		i++;
	}
}

#define X_IS_MAJOR_AXIS 1
#define Y_IS_MAJOR_AXIS 0

void prepare_threads_continued(thread_config *thread_list, int thread_count, t_rect rect)
{
	int		i;
	bool	major_axis;
	int		rect_height;
	int		rect_width;

	printf("prepare_threads_continued\norigin: %4g %4g,  size: %4g %4g\n", rect.origin.x, rect.origin.y, rect.size.x, rect.size.y);
	major_axis = (rect.size.x > rect.size.y);
	rect_width = (int)rect.size.x;
	rect_height = (int)(rect.size.y / thread_count);
	i = 0;
	while (i < thread_count)
	{
		thread_list[i].rect.size.y = rect_height;
		thread_list[i].rect.size.x = rect_width;
		thread_list[i].rect.origin.x = rect.origin.x;
		thread_list[i].rect.origin.y = rect.origin.y + (i * rect_height);
		printf("Thread: %d   ", i);
		printf("origin: %4g %4g,  size: %4g %4g\n", thread_list[i].rect.origin.x, thread_list[i].rect.origin.y, thread_list[i].rect.size.x, thread_list[i].rect.size.y);

		if (major_axis == X_IS_MAJOR_AXIS)
		{
		}
		i++;
	}
}

void	draw_iter_parallel_region(t_config config, t_surface16 iter_frame, int thread_count, t_rect rect)
{
	int				i;
	pthread_t		thread_list[MAX_THREAD] = {};
	thread_config	thread_config_list[MAX_THREAD] = {};

	if (rect.size.y < thread_count)
		thread_count = (int) rect.size.y;
	if (rect.size.x == 0 || rect.size.y == 0)
		return;
	prepare_threads(config, iter_frame, thread_config_list, thread_count);
	prepare_threads_continued(thread_config_list, thread_count, rect);
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

void	draw_iter_parallel_pool(t_config config, t_thread_pool *pool, t_surface16 iter_frame)
{
	int				i;
	thread_config	thread_config_list[MAX_THREAD] = {};

	prepare_threads(config, iter_frame, thread_config_list, pool->thread_count);
	i = 0;
	while (i < pool->thread_count)
	{
		thread_pool_add_work(pool, thread_config_list + i, sizeof(thread_config), draw_task);
		i++;
	}

	thread_pool_wait(pool);
}

void	draw_iter_parallel(t_config config, t_surface16 iter_frame, int thread_count)
{
	int i;

	thread_config thread_config_list[MAX_THREAD] = {};
	pthread_t thread_list[MAX_THREAD] = {};
	prepare_threads(config, iter_frame, thread_config_list, thread_count);
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
