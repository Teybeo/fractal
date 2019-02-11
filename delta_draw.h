#ifndef FRACTOL_DELTA_DRAW_H
#define FRACTOL_DELTA_DRAW_H

#include "t_float2.h"
#include "drawing.h"
#include "coloring.h"

t_rect	get_wide_dirty_rect(t_float2 frame_size, t_float2 delta);
t_rect	get_tall_dirty_rect(t_float2 frame_size, t_float2 delta);
void	copy_region(t_float2 src, t_float2 dst, t_float2 region_size, t_surface16 surface, t_surface color_frame);

#endif
