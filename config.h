#ifndef FRACTOL_CONFIG_H
#define FRACTOL_CONFIG_H

#include "t_float2.h"

struct	s_config
{
	int			depth_max;
	t_float2	z_min;
	t_float2	z_max;
	t_float2	z_size;
	t_float2	win_size;

};
typedef struct s_config	t_config;

t_config	config_init();
void config_zoom_to(t_config *config, int x, int y);
void config_dezoom_from(t_config *config, int x, int y);

#endif
