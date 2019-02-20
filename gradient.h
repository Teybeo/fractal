#ifndef FRACTOL_GRADIENT_H
#define FRACTOL_GRADIENT_H

#include "rgb.h"

struct	s_grad_step
{
	t_rgb		color;
	float	treshold;
};
typedef struct s_grad_step	t_grad_step;

struct	s_gradient
{
	t_grad_step	*step;
	int			step_count;
};
typedef struct s_gradient	t_gradient;

t_gradient	create_gradient(int gradient_type);
void		destroy_gradient(t_gradient gradient);
uint32_t	get_color_from_gradient(t_gradient gradient, float percent);

#endif
