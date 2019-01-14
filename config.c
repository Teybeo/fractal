#include "config.h"

t_config	config_init()
{
	t_config config;
	config.depth_max = 100;
//	config.z_min = (t_float2){-2, -2};
//	config.z_max = (t_float2){2, 2};
	config.z_min = (t_float2){-0.6, -0.};
	config.z_max = (t_float2){-0.8, -0.2};
	config.z_size = float2_sub(config.z_max, config.z_min);
	return (config);
}

t_rect	config_fast_move(t_config *config, t_float2 delta)
{
	t_rect	skip_rect;

	return (skip_rect);
}

void config_zoom_factor(t_config *config, float factor)
{
	t_float2	size;

	size = float2_sub(config->z_max, config->z_min);
	float2_mul_this(&size, factor);
	float2_add_this(&config->z_min, size);
	float2_sub_this(&config->z_max, size);
	config->z_size = float2_sub(config->z_max, config->z_min);
}

void config_zoom_to(t_config *config, int x, int y, t_float2 win_size)
{
	t_float2 size;
	t_float2 center_to_mouse;
	t_float2 z_mouse;
	t_float2 z_center;

	z_mouse = (t_float2){x, y};
	float2_remap(&z_mouse, win_size, config->z_size, config->z_min);
	z_center = get_center(config->z_min, config->z_max);
	size = float2_sub(config->z_max, config->z_min);
	center_to_mouse = float2_sub(z_mouse, z_center);
	center_to_mouse.x /= 2;
	center_to_mouse.y /= 2;
	size.x *= 0.25f;
	size.y *= 0.25f;
	float2_add_this(&config->z_min, size);
	float2_sub_this(&config->z_max, size);
	float2_add_this(&config->z_min, center_to_mouse);
	float2_add_this(&config->z_max, center_to_mouse);
	config->z_size = float2_sub(config->z_max, config->z_min);
}

void config_dezoom_from(t_config *config, int x, int y, t_float2 win_size)
{
	t_float2 size;
	t_float2 center_to_mouse;
	t_float2 z_mouse;
	t_float2 z_center;

	z_mouse = (t_float2){x, y};
	float2_remap(&z_mouse, win_size, config->z_size, config->z_min);
	z_center = get_center(config->z_min, config->z_max);
	size = float2_sub(config->z_max, config->z_min);
	center_to_mouse = float2_sub(z_mouse, z_center);
	center_to_mouse.x /= -1;
	center_to_mouse.y /= -1;
	size.x *= -0.5f;
	size.y *= -0.5f;
	float2_add_this(&config->z_min, size);
	float2_sub_this(&config->z_max, size);
	float2_add_this(&config->z_min, center_to_mouse);
	float2_add_this(&config->z_max, center_to_mouse);
	config->z_size = float2_sub(config->z_max, config->z_min);
}