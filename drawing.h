#ifndef FRACTOL_REAL_DRAWING_H
#define FRACTOL_REAL_DRAWING_H

#include "t_float2.h"

int	get_mandelbrot_value(t_float2 c, int depth_max);
int	get_burningship_value(t_float2 c, int depth_max);
int	get_julia_value(t_float2 c, t_float2 z_in, int depth_max);

#endif
