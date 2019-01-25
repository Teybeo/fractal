#ifndef DRAWING_H
# define DRAWING_H

#include <stdint.h>
#include "t_float2.h"
#include "config.h"
#include "thread_pool.h"

#define MAX_THREAD 256

typedef struct thread_config {
	t_config	config;
	t_float2	win_size;
	uint16_t	*pixels;
	t_rect		rect;
	char		thread_id;
} thread_config;

struct	s_surface16
{
	uint16_t	*iter;
	t_float2	size;
};
typedef struct s_surface16	t_surface16;

void	draw_iter_region(t_config config, t_rect rect, t_surface16 iter_frame);
void	draw_iter_region_parallel(t_config config, t_surface16 iter_frame, int thread_count, t_rect rect);
void	draw_iter_region_parallel_pool(t_config config, t_thread_pool *pool, t_surface16 iter_frame, t_rect rect);

#endif
