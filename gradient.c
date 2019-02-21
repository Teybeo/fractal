#include "gradient.h"

#include "gradient_definitions.h"

#include <stdlib.h>
#include <stdio.h>

void		get_color_interval(t_gradient gradient, float percent, t_grad_step *a, t_grad_step *b);

t_gradient	create_gradient(int gradient_type)
{
	if (gradient_type == 0)
		return (create_blue_yellow());
	else if (gradient_type == 1)
		return (create_random());
	else
		return (create_red_brown());
}

uint32_t	get_color_from_gradient(t_gradient gradient, float percent)
{
	t_grad_step	a;
	t_grad_step	b;
	float		mix_factor;
	t_rgb		color;

	get_color_interval(gradient, percent, &a, &b);
	mix_factor = (percent - a.treshold) / (b.treshold - a.treshold);
	color = color_mix(mix_factor, a.color, b.color);
	return rgb_pack(color);
}

void	get_color_interval(t_gradient gradient, float percent, t_grad_step *a, t_grad_step *b)
{
	t_grad_step	*step;
	int			i;

	step = gradient.step;
	i = 0;
	while (i < (gradient.step_count - 1))
	{
		if (percent >= step[i].treshold && percent < step[i + 1].treshold)
			break;
		i++;
	}
	*a = step[i];
	*b = step[i + 1];
}

void	destroy_gradient(t_gradient gradient)
{
	free(gradient.step);
}