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

int	keyup_event(int keycode, void *param)
{
	t_app *app;

	app = param;
	app->keystate[keycode] = false;
	if (keycode == KEY_ESCAPE)
		exit(0);
	if (keycode == KEY_R)
		app->config = config_init(app->surface.size);
	if (keycode == KEY_G)
		app->config.show_chunks ^= 1;
	if (keycode == KEY_P)
		app->config.show_palette ^= 1;
	if (keycode == KEY_MORE)
		app->config.lines_per_chunk++;
	if (keycode == KEY_LESS)
		app->config.lines_per_chunk -= (app->config.lines_per_chunk != 1);
	if (keycode == KEY_ZOOM)
		config_zoom_factor(&app->config, ZOOM);
	if (keycode == KEY_DEZOOM)
		config_zoom_factor(&app->config, DEZOOM);
	if (keycode == KEY_1)
		app->config.fractal_fn = get_mandelbrot_value;
	if (keycode == KEY_2)
		app->config.fractal_fn = get_burningship_value;
	if (keycode == KEY_3)
		app->config.fractal_fn = get_julia_value;
	if (keycode == KEY_KP_1)
		set_palette(app->config.palette, 1);
	if (keycode == KEY_KP_2)
		set_palette(app->config.palette, 2);
	app->need_full_redraw = (keycode == KEY_MORE || keycode == KEY_LESS
							 || keycode == KEY_ZOOM || keycode == KEY_DEZOOM
							 || keycode == KEY_R || keycode == KEY_G || keycode == KEY_P
							 || keycode == KEY_1 || keycode == KEY_2 || keycode == KEY_3
							 || keycode == KEY_KP_1 || keycode == KEY_KP_2);
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
