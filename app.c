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
void	app_delta_draw(t_float2 delta, t_config *config, t_surface16 iter_frame);

int	 keydown_event(int keycode, void *param)
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

void	copy_region(t_float2 src, t_float2 dst, t_float2 region_size, t_surface16 surface)
{
	if ((dst.y > src.y) || ((dst.y == src.y) && (dst.x > src.y)))
	{
		for (int y = region_size.y - 1; y >= 0; --y)
		{
			for (int x = region_size.x - 1; x >= 0; --x)
			{
				int src_index = (int) ((src.y + y) * surface.size.x + src.x + x);
				int dst_index = (int) ((dst.y + y) * surface.size.x + dst.x + x);
				surface.iter[dst_index] = surface.iter[src_index];
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
				surface.iter[dst_index] = surface.iter[src_index];
			}
		}
	}
}


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
		app_delta_draw(delta, &app->config, app->iter_buffer);
		draw_color(app->surface, app->iter_buffer, app->config);
		mlx_put_image_to_window(app->mlx_context, app->mlx_window, app->mlx_texture, 0, 0);
		app_draw_ui(*app);
	}
	return (0);
}

t_rect	get_wide_dirty_rect(t_float2 frame_size, t_rect skip_rect, t_float2 delta)
{
	t_rect rect;
	rect.size.x = frame_size.x;
	rect.size.y = fabsf(delta.y);
	rect.origin.x = 0;
	rect.origin.y = (delta.y > 0) ? skip_rect.size.y : 0;
	return rect;
}

t_rect	get_tall_dirty_rect(t_float2 frame_size, t_rect skip_rect, t_float2 delta)
{
	t_rect rect;
	rect.size.x = fabsf(delta.x);
	rect.size.y = skip_rect.size.y;
	rect.origin.x = (delta.x > 0) ? skip_rect.size.x : 0;
	rect.origin.y = (delta.y > 0) ? 0 : skip_rect.origin.y;
	(void)frame_size;
	return rect;
}

void app_delta_draw(t_float2 delta, t_config *config, t_surface16 iter_frame)
{
	t_float2	src = {};
	t_float2	dst = {};
	t_rect		skip_rect;
	t_float2	region_size;
	t_float2	z_delta;

	region_size = iter_frame.size;
	src.x = (delta.x >= 0) ? delta.x : 0;
	src.y = (delta.y >= 0) ? delta.y : 0;
	dst.x = (delta.x >= 0) ? 0 : -delta.x;
	dst.y = (delta.y >= 0) ? 0 : -delta.y;
	region_size.x -= fabsf(delta.x);
	region_size.y -= fabsf(delta.y);
	z_delta.x = (delta.x / iter_frame.size.x) * config->z_size.x;
	z_delta.y = (delta.y / iter_frame.size.y) * config->z_size.y;
	float2_add_this(&config->z_max, z_delta);
	float2_add_this(&config->z_min, z_delta);
	// TODO: apply partial draw logic to color rendering
	// TODO: what about multi-threading the partial draw ?
	copy_region(src, dst, region_size, iter_frame);
	skip_rect.origin = dst;
	skip_rect.size = region_size;
	t_rect tall_dirty_rect = get_tall_dirty_rect(iter_frame.size, skip_rect, delta);
	t_rect wide_dirty_rect = get_wide_dirty_rect(iter_frame.size, skip_rect, delta);
//	draw_iter_region(*config, skip_rect, iter_frame);
//	puts("Tall dirty rect");
	draw_iter_region(*config, tall_dirty_rect, iter_frame);
//	puts("Wide dirty rect");
	draw_iter_region(*config, wide_dirty_rect, iter_frame);
}


void	app_init(t_app *app)
{
	int			osef;
	t_float2	win_size;

//	win_size = (t_float2){1000, 1000};
	win_size = (t_float2){2560, 1440};
	app->mlx_context = mlx_init();
	app->config = config_init(win_size);
	app->thread_count = 8;
	app->is_dragging = false;
	app->need_full_redraw = true;
	memset(app->keystate, 0, sizeof(app->keystate));
	app->mlx_window = mlx_new_window(app->mlx_context, win_size.x, win_size.y, "Wireframe");
	app->mlx_texture = mlx_new_image(app->mlx_context, win_size.x, win_size.y);
	app->surface.pixels = (uint32_t*)mlx_get_data_addr(app->mlx_texture, &osef, &osef, &osef);
	app->surface.size = win_size;
	app->iter_buffer.iter = malloc(sizeof(uint16_t) * win_size.x * win_size.y);
	app->iter_buffer.size = win_size;
	memset(app->iter_buffer.iter, 0, sizeof(uint16_t) * win_size.x * win_size.y);
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

	delta.x = app->keystate[KEY_RIGHT] - app->keystate[KEY_LEFT];
	delta.y = app->keystate[KEY_DOWN] - app->keystate[KEY_UP];
	delta.x *= 4;
	delta.y *= 4;
	if (delta.x != 0 || delta.y != 0)
	{
		app_delta_draw(delta, &app->config, app->iter_buffer);
		draw_color(app->surface, app->iter_buffer, app->config);
	}
	if (app->keystate[KEY_PLUS])
		app->config.depth_max += 10;
	if (app->keystate[KEY_MINUS])
		app->config.depth_max -= 10 * ((app->config.depth_max - 10) >= 0);
	if (app->keystate[KEY_PLUS] || app->keystate[KEY_MINUS])
		app->need_full_redraw = true;
}

int		app_callback(void *param)
{
	t_app	*app;
	app = param;
	app_update(app);
	mlx_clear_window(app->mlx_context, app->mlx_window);
//	draw_iter_region(app->config, (t_rect){}, app->pixels);
//	draw_iter_parallel(app->config, app->iter_buffer, app->thread_count);
	if (app->need_full_redraw)
	{
		draw_iter_parallel(app->config, app->iter_buffer, app->thread_count);
		draw_color(app->surface, app->iter_buffer, app->config);
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

