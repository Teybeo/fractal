#include "events.h"

#include <stdio.h>
#include <stdlib.h>

#include "app.h"

int	 keydown_event(int keycode, void *param)
{
	t_app *app;
	app = param;
	app->keystate[keycode] = true;
//	printf("%d\n", keycode);
	return 0;
}

int	keyup_event(int key, void *param)
{
	t_app		*app;
	t_config	*config;

	app = param;
	config = &app->config;
	app->keystate[key] = false;
	if (key == KEY_ESCAPE)
		exit(0);
	if (key == KEY_R)
		*config = config_init(app->surface.size);
	if (key == KEY_G)
		config->show_chunks ^= 1;
	if (key == KEY_P)
		config->show_palette ^= 1;
	if (key == KEY_MORE)
		config->lines_per_chunk++;
	if (key == KEY_LESS)
		config->lines_per_chunk -= (config->lines_per_chunk != 1);
	if (key == KEY_ZOOM)
		config_zoom_factor(config, ZOOM);
	if (key == KEY_DEZOOM)
		config_zoom_factor(config, DEZOOM);
	if (key == KEY_1)
		config->fractal_fn = get_mandelbrot_value;
	if (key == KEY_2)
		config->fractal_fn = get_burningship_value;
	if (key == KEY_3)
		config->fractal_fn = get_julia_value;
	if (key == KEY_KP_1 || key == KEY_KP_2)
		set_palette(config->palette, key - KEY_KP_1);
	app->need_full_redraw = (key == KEY_MORE || key == KEY_LESS
							 || key == KEY_ZOOM || key == KEY_DEZOOM
							 || key == KEY_R
							 || key == KEY_1 || key == KEY_2 || key == KEY_3);
	if (key == KEY_G || key == KEY_P || key == KEY_KP_1 || key == KEY_KP_2)
		draw_color(app->surface, app->iter_buffer, app->config);
	if (app->need_full_redraw)
		app_callback(param);
	return 0;
}

int	expose_callback(void *param)
{
	puts("EXPOSE CALLBACK");
	(void)param;
	return 0;
}

int	quit_event()
{
	printf("Bye");
	exit(0);
	return 0;
}
