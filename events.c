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
	app->need_full_redraw = (keycode == KEY_MORE || keycode == KEY_LESS
							 || keycode == KEY_ZOOM || keycode == KEY_DEZOOM
							 || keycode == KEY_R);
	return 0;
}

int	mouse_down(int button, int x, int y, void *param)
{
	t_app *app;

	app = param;
	if (y < 0)
		return (0);
	if (button == 1)
		app->is_dragging = true;
	if (button == 4)
		config_zoom_to(&app->config, x, y, app->surface.size);
	if (button == 5)
		config_dezoom_from(&app->config, x, y, app->surface.size);
	app->need_full_redraw = (button == 4 || button == 5);

	return 0;
}

int	mouse_up(int button, int x, int y, void *param)
{
	t_app *app;

	x++;
	y++;
	app = param;
	if (button == 1)
		app->is_dragging = false;
	return 0;
}


int	quit_event()
{
	printf("Bye");
	exit(0);
	return 0;
}
