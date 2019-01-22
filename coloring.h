#ifndef FRACTOL_COLORING_H
# define FRACTOL_COLORING_H

#include <stdint.h>
#include "t_float2.h"
#include "drawing.h"

struct	s_surface
{
	uint32_t	*pixels;
	t_float2	size;
};
typedef struct s_surface	t_surface;

void	draw_color(t_surface surface, t_surface16 iter_frame, t_config config);
void	draw_color_region(t_config config, t_rect rect, t_surface surface, t_surface16 iter_frame);


#endif
