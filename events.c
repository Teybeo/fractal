/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdarchiv <tdarchiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/23 19:01:22 by tdarchiv          #+#    #+#             */
/*   Updated: 2019/02/23 20:14:06 by tdarchiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "events.h"

#include <stdio.h>
#include <stdlib.h>
#include <mlx.h>

#include "app.h"

static const t_fractl_fn g_fractal_map[3] =
{
	&get_mandelbrot_value,
	&get_julia_value,
	&get_burningship_value,
};

bool	is_full_redraw_key_event(int key)
{
	return (key == KEY_MORE || key == KEY_LESS
			|| key == KEY_ZOOM || key == KEY_DEZOOM
			|| key == KEY_R
			|| key == KEY_1 || key == KEY_2 || key == KEY_3);
}

int		keyup_event(int key, void *param)
{
	t_app		*app;
	t_config	*cfg;

	app = param;
	cfg = &app->config;
	app->keystate[key] = false;
	if (key == KEY_R)
		*cfg = config_init(app->surface.size);
	(key == KEY_ESCAPE) && quit_event();
	(key == KEY_G) && (cfg->show_chunks ^= 1);
	(key == KEY_P) && (cfg->show_palette ^= 1);
	(key == KEY_MORE) && (cfg->lines_per_chunk++);
	(key == KEY_LESS) && (cfg->lines_per_chunk -= (cfg->lines_per_chunk != 1));
	if (key == KEY_ZOOM || key == KEY_DEZOOM)
		config_zoom_factor(cfg, (key == KEY_ZOOM) ? ZOOM : DEZOOM);
	if (key == KEY_1 || key == KEY_2 || key == KEY_3)
		cfg->fractal_fn = g_fractal_map[key];
	if (key == KEY_KP_1 || key == KEY_KP_2)
		set_palette(cfg->palette, key - KEY_KP_1);
	app->need_full_redraw = is_full_redraw_key_event(key);
	if (key == KEY_G || key == KEY_P || key == KEY_KP_1 || key == KEY_KP_2)
		draw_color(app->surface, app->iter_buffer, app->config);
	if (app->need_full_redraw)
		app_callback(param);
	return (0);
}

int		keydown_event(int keycode, void *param)
{
	t_app *app;

	app = param;
	app->keystate[keycode] = true;
	return (0);
}

int		expose_callback(void *param)
{
	puts("EXPOSE CALLBACK");
	(void)param;
	return (0);
}

int		quit_event(void)
{
	printf("Bye");
	exit(0);
	return (0);
}
