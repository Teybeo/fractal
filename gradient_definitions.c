#include "gradient_definitions.h"

#include <stdlib.h>

t_gradient	create_blue_yellow()
{
	t_gradient gradient;

	gradient.step_count = 6;
	gradient.step = malloc(sizeof(t_grad_step) * gradient.step_count);
	gradient.step[0].treshold = 0;
	gradient.step[1].treshold = 0.2;
	gradient.step[2].treshold = 0.4;
	gradient.step[3].treshold = 0.6;
	gradient.step[4].treshold = 0.95;
	gradient.step[5].treshold = 1;
	gradient.step[0].color = (t_rgb){0, 0, 0.5};
	gradient.step[1].color = (t_rgb){0.12, 0.4, 0.78};
	gradient.step[2].color = (t_rgb){1, 1, 1};
	gradient.step[3].color = (t_rgb){1, 0.66, 0};
	gradient.step[4].color = (t_rgb){0, 0, 0};
	gradient.step[5].color = (t_rgb){0, 0, 0.5};
	return (gradient);
}

t_gradient	create_red_brown()
{
	t_gradient	gradient;

	gradient.step_count = 6;
	gradient.step = malloc(sizeof(t_grad_step) * gradient.step_count);
	gradient.step[0].treshold = 0;
	gradient.step[1].treshold = 0.2;
	gradient.step[2].treshold = 0.4;
	gradient.step[3].treshold = 0.6;
	gradient.step[4].treshold = 0.95;
	gradient.step[5].treshold = 1;
	gradient.step[0].color = (t_rgb){0.5, 0, 0};
	gradient.step[1].color = (t_rgb){0.78, 0.4, 0.12};
	gradient.step[2].color = (t_rgb){1, 1, 1};
	gradient.step[3].color = (t_rgb){0.66, 1, 0};
	gradient.step[4].color = (t_rgb){0, 0, 0};
	gradient.step[5].color = (t_rgb){0.5, 0, 0};
	return (gradient);
}