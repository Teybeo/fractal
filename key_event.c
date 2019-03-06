/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_event.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdarchiv <tdarchiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/06 20:04:14 by tdarchiv          #+#    #+#             */
/*   Updated: 2019/03/06 20:04:23 by tdarchiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "key_event.h"

#include "app.h"
#include "fractal_list.h"

#include <stdbool.h>
#include <stdio.h>

bool	is_full_redraw_key_event(int key)
{
	return (key == KEY_MORE || key == KEY_LESS
			|| key == KEY_ZOOM || key == KEY_DEZOOM
			|| key == KEY_R
			|| (key >= KEY_1 && key <= KEY_8));
}

int		keyup_event(int key, void *param)
{
	t_app		*app;
	t_config	*cf;

	app = param;
	cf = &app->config;
	app->keystate[key] = false;
	if (key == KEY_R)
		*cf = config_init(app->frame.size, 0);
	(key == KEY_ESCAPE) && quit_event();
	(key == KEY_G) && (cf->show_chunks ^= 1);
	(key == KEY_P) && (cf->show_palette ^= 1);
	(key == KEY_MORE) && (cf->lines_per_chunk++);
	(key == KEY_LESS) && (cf->lines_per_chunk -= (cf->lines_per_chunk != 1));
	if (key == KEY_ZOOM || key == KEY_DEZOOM)
		config_zoom_factor(cf, (key == KEY_ZOOM) ? ZOOM : DEZOOM);
	if (key >= KEY_1 && key <= KEY_8)
		cf->fractal_index = remap_key_number(key);
	cf->fractal_fn = get_fractal_fn(cf->fractal_index);
	if (key == KEY_KP_1 || key == KEY_KP_2)
		set_palette(cf->palette, key - KEY_KP_1);
	app->need_full_redraw = is_full_redraw_key_event(key);
	if (key == KEY_G || key == KEY_P || key == KEY_KP_1 || key == KEY_KP_2)
		draw_color(app->config, app->frame);
	(app->need_full_redraw) && app_callback(param);
	return (0);
}

char	remap_key_number(int keycode)
{
	if (keycode >= KEY_1 && keycode <= KEY_4)
		return (char)(keycode - KEY_1);
	if (keycode == KEY_5)
		return (4);
	if (keycode == KEY_6)
		return (5);
	if (keycode == KEY_7)
		return (6);
	if (keycode == KEY_8)
		return (7);
	return (0);
}

int		keydown_event(int keycode, void *param)
{
	t_app *app;

	app = param;
	app->keystate[keycode] = true;
	printf("%d keycode\n", keycode);
	return (0);
}
