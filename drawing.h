#ifndef DRAWING_H
# define DRAWING_H

#include <stdint.h>
#include "t_float2.h"
#include "config.h"

#define MAX_THREAD 16

typedef struct thread_config {
	t_config	config;
	t_float2	win_size;
	int			first_line;
	int			last_line;
	uint32_t	*pixels;
	char		thread_id;
} thread_config;

struct	s_surface
{
	uint32_t	*pixels;
	t_float2	size;
};
typedef struct s_surface	t_surface;

void	app_draw_parallel(t_config config, t_surface pixels, int thread_count);
void	app_partial_draw(t_config config, t_rect skip_rect, t_surface surface);

#endif
