#include "mouse_events.h"

#include "t_double2.h"
#include "app.h"
#include "delta_draw.h"

#include <stdio.h>
#include <mlx.h>

extern int frame_counter;

int mouse_move(int x, int y, void *param)
{
	static t_float2 old_pos;
	t_float2 new_pos;
	t_float2 delta;
	t_app *app;

	app = param;
	new_pos = (t_float2) {x, y};
	delta = float2_sub(new_pos, old_pos);
	if (app->hold_left_click && (delta.x || delta.y)) {
		printf("[%d] Mouse move -> Delta draw  %.0f, %.0f\n", frame_counter, delta.x, delta.y);
		delta_draw(delta, &app->config, app->iter_buffer, app->surface, app->thread_pool);
		mlx_clear_window(app->mlx_context, app->mlx_window);
		mlx_put_image_to_window(app->mlx_context, app->mlx_window, app->mlx_texture, 0, 0);
		app_draw_ui(*app);
		frame_counter++;
	}
	if (app->hold_right_click && app->config.fractal_fn == get_julia_value)
	{
		app->config.z_mouse = new_pos;
		float2_remap(&app->config.z_mouse, app->iter_buffer.size, app->config.z_size, app->config.z_min);
		app->need_full_redraw = true;
	}
	old_pos = new_pos;
	return (0);
}

int	mouse_down(int button, int x, int y, void *param)
{
	t_app *app;

	app = param;
	if (y < 0)
		return (0);
	if (button == 1)
		app->hold_left_click = true;
	if (button == 2)
		app->hold_right_click = true;
	if (button == 4)
		config_zoom_to(&app->config, x, y, app->surface.size);
	if (button == 5)
		config_dezoom_from(&app->config, x, y, app->surface.size);
	app->need_full_redraw = (button == 4 || button == 5);
	if (app->need_full_redraw)
		app_callback(param);
	return 0;
}

int	mouse_up(int button, int x, int y, void *param)
{
	t_app *app;

	x++;
	y++;
	app = param;
	if (button == 1)
		app->hold_left_click = false;
	if (button == 2)
		app->hold_right_click = false;
	return 0;
}

