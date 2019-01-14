#include "app.h"

#include "drawing.h"

#include <mlx.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <assert.h>


int	quit_event()
{
	printf("Bye");
	exit(0);
	return 0;
}
static unsigned long frame_counter;

void	app_update(t_app *app);
void	app_draw_ui(t_app app);

int	keydown_event(int keycode, void *param)
{
	t_app *app;

	app = param;
	printf("Frame: %lu, %d KEYDOWN\n", frame_counter, keycode);
	app->keystate[keycode] = true;
	return 0;
}

int	keyup_event(int keycode, void *param)
{
	t_app *app;

	app = param;
	printf("Frame: %lu, %d KEYUP\n", frame_counter, keycode);
	app->keystate[keycode] = false;
	if (keycode == KEY_ESCAPE)
		exit(0);
	if (keycode == KEY_R)
		app->config = config_init();
	if (keycode == KEY_MORE)
		app->thread_count++;
	if (keycode == KEY_LESS)
		app->thread_count -= (app->thread_count != 1);
	app->need_full_redraw = (keycode == KEY_MORE || keycode == KEY_LESS || keycode == KEY_R);
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

void	copy_region(t_float2 src, t_float2 dst, t_float2 region_size, t_surface surface)
{
	if ((dst.y > src.y) || ((dst.y == src.y) && (dst.x > src.y)))
	{
		for (int y = region_size.y - 1; y >= 0; --y)
		{
			for (int x = region_size.x - 1; x >= 0; --x)
			{
				int src_index = (int) ((src.y + y) * surface.size.x + src.x + x);
				int dst_index = (int) ((dst.y + y) * surface.size.x + dst.x + x);
				surface.pixels[dst_index] = surface.pixels[src_index];
			}
		}
	}
	else
	{
		for (int y = 0; y < region_size.y; ++y)
		{
			for (int x = 0; x < region_size.x; ++x)
			{
				int src_index = (int) ((src.y + y) * surface.size.x + src.x + x);
				int dst_index = (int) ((dst.y + y) * surface.size.x + dst.x + x);
				surface.pixels[dst_index] = surface.pixels[src_index];
			}
		}
	}
}


void delta_draw(t_float2 delta, t_config *config, t_surface surface);

int mouse_move(int x, int y, void *param)
{
	static t_float2	old_pos;
	t_float2	new_pos;
	t_float2	delta;
	t_app		*app;

	app = param;
	new_pos = (t_float2){x, y};
	delta = float2_sub(old_pos, new_pos);
	old_pos = new_pos;
	if (app->is_dragging)
	{
		delta_draw(delta, &app->config, app->surface);
		mlx_put_image_to_window(app->mlx_context, app->mlx_window, app->mlx_texture, 0, 0);
		app_draw_ui(*app);
	}
	return (0);
}

void delta_draw(t_float2 delta, t_config *config, t_surface surface)
{
	t_float2	src = {};
	t_float2	dst = {};
	t_rect		skip_rect;
	t_float2	region_size;

	region_size = surface.size;
	src.x = (delta.x >= 0) ? delta.x : 0;
	src.y = (delta.y >= 0) ? delta.y : 0;
	dst.x = (delta.x >= 0) ? 0 : -delta.x;
	dst.y = (delta.y >= 0) ? 0 : -delta.y;
	region_size.x -= fabsf(delta.x);
	region_size.y -= fabsf(delta.y);
	delta.x = (delta.x / surface.size.x) * config->z_size.x;
	delta.y = (delta.y / surface.size.y) * config->z_size.y;
	float2_add_this(&config->z_max, delta);
	float2_add_this(&config->z_min, delta);
	copy_region(src, dst, region_size, surface);
	skip_rect.origin = dst;
	skip_rect.size = region_size;
	app_partial_draw(*config, skip_rect, surface);
}


void	app_init(t_app *app)
{
	int			osef;
	t_float2	win_size;

	win_size = (t_float2){1000, 1000};
	app->mlx_context = mlx_init();
	app->config = config_init();
	app->thread_count = 1;
	app->is_dragging = false;
	app->need_full_redraw = true;
	memset(app->keystate, 0, sizeof(app->keystate));
	app->mlx_window = mlx_new_window(app->mlx_context, win_size.x, win_size.y, "Wireframe");
	app->mlx_texture = mlx_new_image(app->mlx_context, win_size.x, win_size.y);
	app->surface.pixels = (uint32_t*)mlx_get_data_addr(app->mlx_texture, &osef, &osef, &osef);
	app->surface.size = win_size;
	mlx_do_key_autorepeatoff(app->mlx_context);
	mlx_hook(app->mlx_window, 2, osef, keydown_event, app);
	mlx_hook(app->mlx_window, 3, osef, keyup_event, app);
	mlx_hook(app->mlx_window, 4, osef, mouse_down, app);
	mlx_hook(app->mlx_window, 5, osef, mouse_up, app);
	mlx_hook(app->mlx_window, 6, osef, mouse_move, app);
	mlx_hook(app->mlx_window, 17, osef, quit_event, NULL);
//	mlx_expose_hook(app->mlx_window, expose_callback, app);
	mlx_loop_hook(app->mlx_context, app_callback, app);
	mlx_loop(app->mlx_context);
}

float get_frametime();

void	app_update(t_app *app)
{
	t_float2	delta;
	delta = (t_float2){};

	delta.y -= app->keystate[KEY_UP];
	delta.y += app->keystate[KEY_DOWN];
	delta.x += app->keystate[KEY_RIGHT];
	delta.x -= app->keystate[KEY_LEFT];
	delta.x *= 4;
	delta.y *= 4;
	delta_draw(delta, &app->config, app->surface);
	if (app->keystate[KEY_PLUS])
		app->config.depth_max += 10;
	if (app->keystate[KEY_MINUS])
		app->config.depth_max -= 10 * ((app->config.depth_max - 10) >= 0);
	if (app->keystate[KEY_ZOOM])
		config_zoom_factor(&app->config, 0.25f);
	if (app->keystate[KEY_DEZOOM])
		config_zoom_factor(&app->config, -0.5f);
	if (app->keystate[KEY_ZOOM] || app->keystate[KEY_DEZOOM]
	|| app->keystate[KEY_PLUS] || app->keystate[KEY_MINUS])
		app->need_full_redraw = true;
}

int		app_callback(void *param)
{
	t_app	*app;
	app = param;
	app_update(app);
	mlx_clear_window(app->mlx_context, app->mlx_window);
//	app_partial_draw(app->config, (t_rect){}, app->pixels);
	if (app->need_full_redraw)
	{
		app_draw_parallel(app->config, app->surface, app->thread_count);
		app->need_full_redraw = false;
	}
	mlx_put_image_to_window(app->mlx_context, app->mlx_window, app->mlx_texture, 0, 0);
	app_draw_ui(*app);
	frame_counter++;
	return (0);
}

void	app_draw_ui(t_app app)
{
	float	time;
	char	string[512];

	time = get_frametime();
	memset(string, 0, sizeof(string));
	sprintf(string, "Frametime: %-7.4g ms (%-3.3g fps)", time, 1000 / time);
	mlx_string_put(app.mlx_context, app.mlx_window, 10, 10, 0x00FFFFFF, string);
	memset(string, 0, sizeof(string));
	sprintf(string, "Threads: %d", app.thread_count);
	mlx_string_put(app.mlx_context, app.mlx_window, 10, 90, 0x00FFFFFF, string);
	memset(string, 0, sizeof(string));
	sprintf(string, "Depth max: %-5d", app.config.depth_max);
	mlx_string_put(app.mlx_context, app.mlx_window, 10, 30, 0x00FFFFFF, string);
	memset(string, 0, sizeof(string));
	sprintf(string, "Size: %g, %g", app.config.z_size.x, app.config.z_size.y);
	mlx_string_put(app.mlx_context, app.mlx_window, 10, 50, 0x00FFFFFF, string);
	memset(string, 0, sizeof(string));
	sprintf(string, "Center: %g, %g", app.config.z_min.x + app.config.z_size.x / 2, app.config.z_min.y + app.config.z_size.y / 2);
	mlx_string_put(app.mlx_context, app.mlx_window, 10, 70, 0x00FFFFFF, string);
}

float get_frametime()
{
	static struct timespec	start = {};
	struct timespec			now = {};
	float					duration;

	clock_gettime (CLOCK_REALTIME, &now);
	duration = (now.tv_sec - start.tv_sec) * 1000
			+ (now.tv_nsec - start.tv_nsec) / 1000000.f;
	start = now;
	return (duration);
}

