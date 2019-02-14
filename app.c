#include "app.h"

#include "multithreading.h"
#include "events.h"
#include "delta_draw.h"
#include "mouse_events.h"

#include <mlx.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <assert.h>
#include <stdarg.h>

void	app_update(t_app *app);
float	get_frametime();
void	draw_string(t_app app, int x, int y, const char* fmt, ...);

int frame_counter = 0;

void	app_init(t_app *app)
{
	int			osef;
	t_float2	win_size;

	win_size = (t_float2){2560, 1440};
	win_size = (t_float2){1000, 1000};
	app->mlx_context = mlx_init();
	app->config = config_init(win_size);
	app->thread_count = THREAD_COUNT;
	app->thread_pool = create_thread_pool(app->thread_count);
	app->hold_left_click = false;
	app->need_full_redraw = true;
	memset(app->keystate, 0, sizeof(app->keystate));
	app->mlx_window = mlx_new_window(app->mlx_context, win_size.x, win_size.y, "Fractol");
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
	mlx_expose_hook(app->mlx_window, expose_callback, app);
	mlx_loop_hook(app->mlx_context, app_callback, app);
}

void	app_run(t_app *app)
{
	app_callback(app);
	mlx_loop(app->mlx_context);
}

void	app_update(t_app *app)
{
	t_float2	delta;

	delta.x = app->keystate[KEY_RIGHT] - app->keystate[KEY_LEFT];
	delta.y = app->keystate[KEY_DOWN] - app->keystate[KEY_UP];
	delta.x *= 4;
	delta.y *= 4;
	if (delta.x != 0 || delta.y != 0)
	{
		delta_draw(delta, &app->config, app->iter_buffer, app->surface, app->thread_pool);
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
	printf("[%d] callback\n", frame_counter);
	t_app	*app;
	app = param;
	app_update(app);
	mlx_clear_window(app->mlx_context, app->mlx_window);
	if (app->need_full_redraw)
	{
		t_rect rect = {{}, app->iter_buffer.size};
#if USE_THREAD_POOL
		draw_iter_region_parallel_pool(app->config, app->thread_pool, app->iter_buffer, rect);
#else
		draw_iter_region_parallel(app->config, app->iter_buffer, app->thread_count, rect);
#endif
		draw_color(app->surface, app->iter_buffer, app->config);
		frame_counter++;
	}
	mlx_put_image_to_window(app->mlx_context, app->mlx_window, app->mlx_texture, 0, 0);
	app_draw_ui(*app);
//	app->need_full_redraw = false;
	return (0);
}

void	app_draw_ui(t_app app)
{
	float	time;

	time = get_frametime();
	draw_string(app, 10, 10, "Frametime: %-7.4g ms (%-3.3g fps)", time, 1000 / time);
	draw_string(app, 10, 30, "Depth max: %-5d", app.config.depth_max);
	draw_string(app, 10, 50, "Size: %g, %g", app.config.z_size.x, app.config.z_size.y);
	draw_string(app, 10, 70, "Center: %g, %g", app.config.z_min.x + app.config.z_size.x / 2, app.config.z_min.y + app.config.z_size.y / 2);
	draw_string(app, 10, 90, "Threads: %d", app.thread_count);
	draw_string(app, 10, 110, "Lines per chunk: %d", app.config.lines_per_chunk);
	if (app.need_full_redraw)
		draw_string(app, 10, 130, "FULL REDRAW");
}

void	draw_string(t_app app, int x, int y, const char* fmt, ...)
{
	char	string[512];
	va_list args;

	va_start(args, fmt);
	memset(string, 0, sizeof(string));
	vsprintf(string, fmt, args);
	mlx_string_put(app.mlx_context, app.mlx_window, x, y, 0x00FFFFFF, string);
	va_end(args);
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

