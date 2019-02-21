#include "config.h"

#include "coloring.h"

#include <stdio.h>
#include <stdlib.h>

extern int frame_counter;

t_config config_init(t_double2 win_size)
{
	double		aspect_ratio;
	t_config	config;
//	static uint32_t	palette[PALETTE_COLOR_COUNT];
	static uint32_t *palette;
	if (palette == NULL)
		palette = malloc(sizeof(uint32_t) * PALETTE_COLOR_COUNT);

	aspect_ratio = win_size.x / win_size.y;
	config.depth_max = DEPTH_MAX;
	config.z_min = (t_double2){-2 * aspect_ratio, -2};
	config.z_max = (t_double2){2 * aspect_ratio, 2};
	config.z_size = double2_sub(config.z_max, config.z_min);
	config.z_mouse = (t_double2){};
	config.fractal_fn = get_mandelbrot_value;
	config.lines_per_chunk = LINES_PER_CHUNK;
	config.palette = palette;
	set_palette(palette, 0);
	return (config);
}

/*
** Recompute z_min and z_max as center +/- new_half_size
** new_half_size = (old_size / 2) * factor
** new_size = (new_half_size * 2)
** new_size = ((old_size / 2) * factor) * 2
** new_size = old_size * factor
*/
void config_zoom_factor(t_config *config, double factor)
{
	t_double2	new_half_size;
	t_double2	z_center;
	new_half_size = double2_sub(config->z_max, config->z_min);
	double2_mul_this(&new_half_size, 0.5f);
	double2_mul_this(&new_half_size, factor);
	z_center = get_center(config->z_min, config->z_max);
	config->z_min = double2_sub(z_center, new_half_size);
	config->z_max = double2_add(z_center, new_half_size);
	config->z_size = double2_sub(config->z_max, config->z_min);
}

/*
** Compute new_half_size as in config_zoom_factor()
** After that, offset the center by half of the center_to_mouse vector
** Add the new_half_size to the new center
**/

void config_zoom_to(t_config *config, int x, int y, t_double2 win_size)
{
	t_double2 new_half_size;
	t_double2 center_to_mouse;
	t_double2 z_mouse;
	t_double2 z_center;
	printf("[%d] ZOOM\n", frame_counter);

	new_half_size = double2_sub(config->z_max, config->z_min);
	double2_mul_this(&new_half_size, 0.5f);
	double2_mul_this(&new_half_size, ZOOM);

	z_mouse = (t_double2){x, y};
	double2_remap(&z_mouse, win_size, config->z_size, config->z_min);
	z_center = get_center(config->z_min, config->z_max);
	center_to_mouse = double2_sub(z_mouse, z_center);
	double2_mul_this(&center_to_mouse, 0.5f);
	double2_add_this(&z_center, center_to_mouse);
	config->z_min = double2_sub(z_center, new_half_size);
	config->z_max = double2_add(z_center, new_half_size);
	config->z_size = double2_sub(config->z_max, config->z_min);
}

void config_dezoom_from(t_config *config, int x, int y, t_double2 win_size)
{
	t_double2 new_half_size;
	t_double2 center_to_mouse;
	t_double2 z_mouse;
	t_double2 z_center;
	printf("[%d] DEZOOM\n", frame_counter);

	new_half_size = double2_sub(config->z_max, config->z_min);
	double2_mul_this(&new_half_size, 0.5f);
	double2_mul_this(&new_half_size, DEZOOM);

	z_mouse = (t_double2){x, y};
	double2_remap(&z_mouse, win_size, config->z_size, config->z_min);
	z_center = get_center(config->z_min, config->z_max);
	center_to_mouse = double2_sub(z_mouse, z_center);
	double2_mul_this(&center_to_mouse, -1);
	double2_add_this(&z_center, center_to_mouse);
	config->z_min = double2_sub(z_center, new_half_size);
	config->z_max = double2_add(z_center, new_half_size);
	config->z_size = double2_sub(config->z_max, config->z_min);
}

void	config_move_by_delta(t_config *config, t_double2 delta, t_double2 win_size)
{
	t_double2	z_delta;

	z_delta.x = (delta.x / win_size.x) * config->z_size.x;
	z_delta.y = (delta.y / win_size.y) * config->z_size.y;
	double2_sub_this(&config->z_max, z_delta);
	double2_sub_this(&config->z_min, z_delta);
}