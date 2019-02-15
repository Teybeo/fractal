#ifndef FRACTOL_CONFIG_H
#define FRACTOL_CONFIG_H

#include <stdbool.h>
#include "t_float2.h"

#define DEPTH_MAX 1000
#define LINES_PER_CHUNK 5

typedef int	(*fractal_fn)();

struct	s_config
{
	int			depth_max;
	t_float2	z_min;
	t_float2	z_max;
	t_float2	z_size;
	t_float2	z_mouse;
	short		lines_per_chunk;
	fractal_fn	fractal_fn;
	bool		show_chunks;
};
typedef struct s_config	t_config;

typedef struct s_rect {
	t_float2 origin;
	t_float2 size;
} t_rect;

#define ZOOM (0.5)
#define DEZOOM (2)

t_config	config_init(t_float2 win_size);
void		config_zoom_to(t_config *config, int x, int y, t_float2 win_size);
void		config_dezoom_from(t_config *config, int x, int y, t_float2 win_size);
void		config_zoom_factor(t_config *config, float factor);
void		config_move_by_delta(t_config *config, t_float2 delta, t_float2 win_size);

#endif
