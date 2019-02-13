#include "events.h"

#include <stdio.h>
#include <stdlib.h>
#include "app.h"

int	 keydown_event(int keycode, void *param)
{
	t_app *app;
	app = param;
	app->keystate[keycode] = true;
	return 0;
}

int	keyup_event(int keycode, void *param)
{
	t_app *app;

	app = param;
	app->keystate[keycode] = false;
	if (keycode == KEY_ESCAPE)
		exit(0);
	if (keycode == KEY_R)
		app->config = config_init(app->surface.size);
	if (keycode == KEY_MORE)
		app->thread_count++;
	if (keycode == KEY_LESS)
		app->thread_count -= (app->thread_count != 1);
	if (keycode == KEY_ZOOM)
		config_zoom_factor(&app->config, ZOOM);
	if (keycode == KEY_DEZOOM)
		config_zoom_factor(&app->config, DEZOOM);
	if (keycode == KEY_1)
		app->config.fractal_type = MANDELBROT;
	if (keycode == KEY_2)
		app->config.fractal_type = BURNING_SHIP;
	if (keycode == KEY_3)
		app->config.fractal_type = JULIA;
	app->need_full_redraw = (keycode == KEY_MORE || keycode == KEY_LESS
							 || keycode == KEY_ZOOM || keycode == KEY_DEZOOM
							 || keycode == KEY_R
							 || keycode == KEY_1 || keycode == KEY_2 || keycode == KEY_3);
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
