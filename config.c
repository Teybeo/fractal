#include <stdio.h>
#include "config.h"

extern int frame_counter;

t_config config_init(t_float2 win_size)
{
	float		aspect_ratio;
	t_config	config;

	aspect_ratio = win_size.x / win_size.y;
	config.depth_max = 1000;
	config.z_min = (t_float2){-2 * aspect_ratio, -2};
	config.z_max = (t_float2){2 * aspect_ratio, 2};
	config.z_size = float2_sub(config.z_max, config.z_min);
	return (config);
}

/*
** Recompute z_min and z_max as center +/- new_half_size
** new_half_size = (old_size / 2) * factor
** new_size = (new_half_size * 2)
** new_size = ((old_size / 2) * factor) * 2
** new_size = old_size * factor
*/
void config_zoom_factor(t_config *config, float factor)
{
	t_float2	new_half_size;
	t_float2	z_center;
	new_half_size = float2_sub(config->z_max, config->z_min);
	float2_mul_this(&new_half_size, 0.5f);
	float2_mul_this(&new_half_size, factor);
	z_center = get_center(config->z_min, config->z_max);
	config->z_min = float2_sub(z_center, new_half_size);
	config->z_max = float2_add(z_center, new_half_size);
	config->z_size = float2_sub(config->z_max, config->z_min);
}

/*
** Compute new_half_size as in config_zoom_factor()
** After that, offset the center by half of the center_to_mouse vector
** Add the new_half_size to the new center
**/

void config_zoom_to(t_config *config, int x, int y, t_float2 win_size)
{
	t_float2 new_half_size;
	t_float2 center_to_mouse;
	t_float2 z_mouse;
	t_float2 z_center;
	printf("[%d] ZOOM\n", frame_counter);

	new_half_size = float2_sub(config->z_max, config->z_min);
	float2_mul_this(&new_half_size, 0.5f);
	float2_mul_this(&new_half_size, ZOOM);

	z_mouse = (t_float2){x, y};
	float2_remap(&z_mouse, win_size, config->z_size, config->z_min);
	z_center = get_center(config->z_min, config->z_max);
	center_to_mouse = float2_sub(z_mouse, z_center);
	float2_mul_this(&center_to_mouse, 0.5f);
	float2_add_this(&z_center, center_to_mouse);
	config->z_min = float2_sub(z_center, new_half_size);
	config->z_max = float2_add(z_center, new_half_size);
	config->z_size = float2_sub(config->z_max, config->z_min);
}

void config_dezoom_from(t_config *config, int x, int y, t_float2 win_size)
{
	t_float2 new_half_size;
	t_float2 center_to_mouse;
	t_float2 z_mouse;
	t_float2 z_center;
	printf("[%d] DEZOOM\n", frame_counter);

	new_half_size = float2_sub(config->z_max, config->z_min);
	float2_mul_this(&new_half_size, 0.5f);
	float2_mul_this(&new_half_size, DEZOOM);

	z_mouse = (t_float2){x, y};
	float2_remap(&z_mouse, win_size, config->z_size, config->z_min);
	z_center = get_center(config->z_min, config->z_max);
	center_to_mouse = float2_sub(z_mouse, z_center);
	float2_mul_this(&center_to_mouse, -1);
	float2_add_this(&z_center, center_to_mouse);
	config->z_min = float2_sub(z_center, new_half_size);
	config->z_max = float2_add(z_center, new_half_size);
	config->z_size = float2_sub(config->z_max, config->z_min);
}

void	config_move_by_delta(t_config *config, t_float2 delta, t_float2 win_size)
{
	t_float2	z_delta;

	z_delta.x = (delta.x / win_size.x) * config->z_size.x;
	z_delta.y = (delta.y / win_size.y) * config->z_size.y;
	float2_sub_this(&config->z_max, z_delta);
	float2_sub_this(&config->z_min, z_delta);
}