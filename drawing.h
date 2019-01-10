#ifndef DRAWING_H
# define DRAWING_H

#include <stdint.h>
#include "t_float2.h"
#include "config.h"

#define MAX_THREAD 16

typedef struct s_rect {
	t_float2 origin;
	t_float2 size;
} t_rect;

typedef struct thread_config {
	t_config	config;
	t_float2	win_size;
	int			first_line;
	int			last_line;
	uint32_t	*pixels;
	char		thread_id;
} thread_config;

void	app_draw_parallel(t_config config, uint32_t *pixels, int thread_count);
void	app_partial_draw(t_config config, t_rect skip_rect, uint32_t  *pixels);

#endif
