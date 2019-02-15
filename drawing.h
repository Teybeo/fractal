#ifndef FRACTOL_REAL_DRAWING_H
#define FRACTOL_REAL_DRAWING_H

#include "t_float2.h"
#include "config.h"
#include <stdint.h>

struct	s_surface16
{
	uint16_t	*iter;
	t_float2	size;
};
typedef struct s_surface16	t_surface16;

void	draw_iter_region(t_config config, t_rect rect, t_surface16 iter_frame, bool chunk_mask);
int		get_mandelbrot_value(t_float2 c, int depth_max);
int		get_burningship_value(t_float2 c, int depth_max);
int		get_julia_value(t_float2 c, int depth_max, t_float2 z_in);

#endif
