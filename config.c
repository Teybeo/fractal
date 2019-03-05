/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdarchiv <tdarchiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/27 15:20:30 by tdarchiv          #+#    #+#             */
/*   Updated: 2019/03/04 18:03:31 by tdarchiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.h"

#include "coloring.h"
#include "drawing.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

t_config	config_init(t_double2 win_size, int fractal_index)
{
	static uint32_t	palette[PALETTE_COLOR_COUNT];
	double			aspect_ratio;
	t_config		config;

	memset(&config, 0, sizeof(t_config));
	aspect_ratio = win_size.x / win_size.y;
	config.depth_max = DEPTH_MAX;
	config.z_min = (t_double2){-2 * aspect_ratio, -2};
	config.z_max = (t_double2){2 * aspect_ratio, 2};
	config.z_size = double2_sub(config.z_max, config.z_min);
	config.z_mouse = (t_double2){0, 0};
	config.fractal_fn = get_fractal_by_index(fractal_index);
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

void		config_zoom_factor(t_config *cfg, double factor)
{
	t_double2	new_half_size;
	t_double2	z_center;

	new_half_size = double2_sub(cfg->z_max, cfg->z_min);
	double2_mul_this(&new_half_size, 0.5f);
	double2_mul_this(&new_half_size, factor);
	z_center = get_center(cfg->z_min, cfg->z_max);
	cfg->z_min = double2_sub(z_center, new_half_size);
	cfg->z_max = double2_add(z_center, new_half_size);
	cfg->z_size = double2_sub(cfg->z_max, cfg->z_min);
}

/*
** Compute new_half_size as in config_zoom_factor()
** After that, offset the center by half of the center_to_mouse vector
** Add the new_half_size to the new center
*/

void		config_zoom_to(t_config *cfg, int x, int y, t_double2 win_size)
{
	t_double2 new_half_size;
	t_double2 center_to_mouse;
	t_double2 z_mouse;
	t_double2 z_center;

	new_half_size = double2_sub(cfg->z_max, cfg->z_min);
	double2_mul_this(&new_half_size, 0.5f);
	double2_mul_this(&new_half_size, ZOOM);
	z_mouse = (t_double2){x, y};
	double2_remap(&z_mouse, win_size, cfg->z_size, cfg->z_min);
	z_center = get_center(cfg->z_min, cfg->z_max);
	center_to_mouse = double2_sub(z_mouse, z_center);
	double2_mul_this(&center_to_mouse, 0.5f);
	double2_add_this(&z_center, center_to_mouse);
	cfg->z_min = double2_sub(z_center, new_half_size);
	cfg->z_max = double2_add(z_center, new_half_size);
	cfg->z_size = double2_sub(cfg->z_max, cfg->z_min);
}

void		config_dezoom_from(t_config *cfg, int x, int y, t_double2 win_size)
{
	t_double2 new_half_size;
	t_double2 center_to_mouse;
	t_double2 z_mouse;
	t_double2 z_center;

	new_half_size = double2_sub(cfg->z_max, cfg->z_min);
	double2_mul_this(&new_half_size, 0.5f);
	double2_mul_this(&new_half_size, DEZOOM);
	z_mouse = (t_double2){x, y};
	double2_remap(&z_mouse, win_size, cfg->z_size, cfg->z_min);
	z_center = get_center(cfg->z_min, cfg->z_max);
	center_to_mouse = double2_sub(z_mouse, z_center);
	double2_mul_this(&center_to_mouse, -1);
	double2_add_this(&z_center, center_to_mouse);
	cfg->z_min = double2_sub(z_center, new_half_size);
	cfg->z_max = double2_add(z_center, new_half_size);
	cfg->z_size = double2_sub(cfg->z_max, cfg->z_min);
}

void		config_move_by(t_config *cfg, t_double2 delta, t_double2 win_size)
{
	t_double2	z_delta;

	z_delta.x = (delta.x / win_size.x) * cfg->z_size.x;
	z_delta.y = (delta.y / win_size.y) * cfg->z_size.y;
	double2_sub_this(&cfg->z_max, z_delta);
	double2_sub_this(&cfg->z_min, z_delta);
}
