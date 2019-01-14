#ifndef FRACTOL_CONFIG_H
#define FRACTOL_CONFIG_H

#include "t_float2.h"

struct	s_config
{
	int			depth_max;
	t_float2	z_min;
	t_float2	z_max;
	t_float2	z_size;
};
typedef struct s_config	t_config;

typedef struct s_rect {
	t_float2 origin;
	t_float2 size;
} t_rect;

t_config	config_init();
void config_zoom_to(t_config *config, int x, int y, t_float2 win_size);
void config_dezoom_from(t_config *config, int x, int y, t_float2 win_size);

#endif
