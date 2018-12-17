#include "app.h"

#include <mlx.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>

t_config	config_init()
{
	t_config config;
	config.depth_max = 10;
	config.z_min = (t_float2){-2, -2};
	config.z_max = (t_float2){2, 2};
	config.z_size = float2_sub(config.z_max, config.z_min);
	return (config);
}

int	quit_event()
{
	printf("Bye");
	exit(0);
	return 0;
}
static unsigned long frame_counter;

void	app_update(t_app *app);
void config_zoom_to(t_app *app, int x, int y);
void config_dezoom_from(t_app *app, int x, int y);

int	keydown_event(int keycode, void *param)
{
	t_app *app;

	app = param;
//	printf("Frame: %lu, %d KEYDOWN\n", frame_counter, keycode);
	app->keystate[keycode] = true;
	return 0;
}

int	keyup_event(int keycode, void *param)
{
	t_app *app;

	app = param;
//	printf("Frame: %lu, %d KEYUP\n", frame_counter, keycode);
	app->keystate[keycode] = false;
	if (keycode == KEY_ESCAPE)
		exit(0);
	if (keycode == KEY_R)
		app->config = config_init();
	return 0;
}

int	mouse_down(int button, int x, int y, void *param)
{
	t_app *app;

	app = param;
	if (button == 1)
		config_zoom_to(app, x, y);
	if (button == 2)
		config_dezoom_from(app, x, y);
	return 0;
}

void config_zoom_to(t_app *app, int x, int y)
{
	t_float2 size;
	t_float2 center_to_mouse;
	t_float2 z_mouse;
	t_float2 z_center;

	z_mouse = (t_float2){x, y};
	float2_remap(&z_mouse, app->win_size, app->config.z_size, app->config.z_min);
	z_center = get_center(app->config.z_min, app->config.z_max);
	size = float2_sub(app->config.z_max, app->config.z_min);
	center_to_mouse = float2_sub(z_mouse, z_center);
	center_to_mouse.x /= 2;
	center_to_mouse.y /= 2;
	size.x *= 0.25f;
	size.y *= 0.25f;
	float2_add_this(&app->config.z_min, size);
	float2_sub_this(&app->config.z_max, size);
	float2_add_this(&app->config.z_min, center_to_mouse);
	float2_add_this(&app->config.z_max, center_to_mouse);
	app->config.z_size = float2_sub(app->config.z_max, app->config.z_min);
}

void config_dezoom_from(t_app *app, int x, int y)
{
	t_float2 size;
	t_float2 center_to_mouse;
	t_float2 z_mouse;
	t_float2 z_center;

	z_mouse = (t_float2){x, y};
	float2_remap(&z_mouse, app->win_size, app->config.z_size, app->config.z_min);
	z_center = get_center(app->config.z_min, app->config.z_max);
	size = float2_sub(app->config.z_max, app->config.z_min);
	center_to_mouse = float2_sub(z_mouse, z_center);
	center_to_mouse.x /= -1;
	center_to_mouse.y /= -1;
	size.x *= -0.5f;
	size.y *= -0.5f;
	float2_add_this(&app->config.z_min, size);
	float2_sub_this(&app->config.z_max, size);
	float2_add_this(&app->config.z_min, center_to_mouse);
	float2_add_this(&app->config.z_max, center_to_mouse);
	app->config.z_size = float2_sub(app->config.z_max, app->config.z_min);
}

void	app_init(t_app *app)
{
	int osef;

	app->win_size = (t_float2){1000, 1000};
	memset(app->keystate, 0, sizeof(app->keystate));
	app->mlx_context = mlx_init();
	app->config = config_init();
	app->mlx_window = mlx_new_window(app->mlx_context, app->win_size.x, app->win_size.y, "Wireframe");
	app->mlx_texture = mlx_new_image(app->mlx_context, app->win_size.x, app->win_size.y);
	app->pixels = (uint32_t*)mlx_get_data_addr(app->mlx_texture, &osef, &osef, &osef);
	mlx_do_key_autorepeatoff(app->mlx_context);
	mlx_hook(app->mlx_window, 2, osef, keydown_event, app);
	mlx_hook(app->mlx_window, 3, osef, keyup_event, app);
	mlx_hook(app->mlx_window, 4, osef, mouse_down, app);
//	mlx_hook(app->mlx_window, 5, osef, mouse_up, app);
//	mlx_hook(app->mlx_window, 6, osef, mouse_move, app);
	mlx_hook(app->mlx_window, 17, osef, quit_event, NULL);
//	mlx_expose_hook(app->mlx_window, expose_callback, app);
	mlx_loop_hook(app->mlx_context, app_callback, app);
	mlx_loop(app->mlx_context);
}

float get_frametime();

void	app_draw_ui(t_app app);



void	app_update(t_app *app)
{
	if (app->keystate[KEY_UP])
	{
		app->config.z_min.y -= 0.02f * app->config.z_size.y;
		app->config.z_max.y -= 0.02f * app->config.z_size.y;
	}
	if (app->keystate[KEY_DOWN])
	{
		app->config.z_min.y += 0.02f * app->config.z_size.y;
		app->config.z_max.y += 0.02f * app->config.z_size.y;
	}
	if (app->keystate[KEY_RIGHT])
	{
		app->config.z_min.x += 0.02f * app->config.z_size.x;
		app->config.z_max.x += 0.02f * app->config.z_size.x;
	}
	if (app->keystate[KEY_LEFT])
	{
		app->config.z_min.x -= 0.02f * app->config.z_size.x;
		app->config.z_max.x -= 0.02f * app->config.z_size.x;
	}
	if (app->keystate[KEY_PLUS])
		app->config.depth_max += 10;
	if (app->keystate[KEY_MINUS])
		app->config.depth_max -= 10 * ((app->config.depth_max - 10) >= 0);
	if (app->keystate[KEY_ZOOM])
	{
		t_float2 size;
		size = float2_sub(app->config.z_max, app->config.z_min);
		size.x *= 0.25f;
		size.y *= 0.25f;
		float2_add_this(&app->config.z_min, size);
		float2_sub_this(&app->config.z_max, size);
		app->config.z_size = float2_sub(app->config.z_max, app->config.z_min);
	}
	if (app->keystate[KEY_DEZOOM])
	{
		t_float2 size;
		size = float2_sub(app->config.z_max, app->config.z_min);
		size.x *= -0.5f;
		size.y *= -0.5f;
		float2_add_this(&app->config.z_min, size);
		float2_sub_this(&app->config.z_max, size);
		app->config.z_size = float2_sub(app->config.z_max, app->config.z_min);
	}
}

int		app_callback(void *param)
{
	t_app	*app;
	app = param;
	app_update(app);
	mlx_clear_window(app->mlx_context, app->mlx_window);
	app_draw(*app);
	mlx_put_image_to_window(app->mlx_context, app->mlx_window, app->mlx_texture, 0, 0);
	app_draw_ui(*app);
	frame_counter++;
	app->need_redraw = false;
	return (0);
}

void	app_draw_ui(t_app app)
{
	float	time;
	char	*string;

	time = get_frametime();
	asprintf(&string, "Frametime: %-7.4g ms (%-3.3g fps)", time, 1 / time);
	mlx_string_put(app.mlx_context, app.mlx_window, 10, 10, 0x00FFFFFF, string);
	free(string);
	asprintf(&string, "Depth max: %-5d", app.config.depth_max);
	mlx_string_put(app.mlx_context, app.mlx_window, 10, 30, 0x00FFFFFF, string);
	free(string);
	asprintf(&string, "Size: %g, %g", app.config.z_size.x, app.config.z_size.y);
	mlx_string_put(app.mlx_context, app.mlx_window, 10, 50, 0x00FFFFFF, string);
	free(string);
	asprintf(&string, "Center: %g, %g", app.config.z_min.x + app.config.z_size.x / 2, app.config.z_min.y + app.config.z_size.y / 2);
	mlx_string_put(app.mlx_context, app.mlx_window, 10, 70, 0x00FFFFFF, string);
	free(string);
}

float get_frametime()
{
	float duration;
	static clock_t start = 0;

	duration = (clock() - start) / (float)CLOCKS_PER_SEC;
	start = clock();
	return duration;
}
#if 1
int	get_mandelbrot_value(t_float2 c, int depth_max)
{
	t_float2	z;
	t_float2	z_old;
	t_float2	z_squared;
	int			depth;

	depth = 0;
	z = c;
	z_squared = (t_float2){z.x * z.x, z.y * z.y};
	z_old = z;
	while (((z_squared.x + z_squared.y) < 4) && (depth < depth_max))
	{
		z.y = 2 * z.x * z.y + c.y;
		z.x = z_squared.x - z_squared.y + c.x;
		depth++;
		z_squared = (t_float2){z.x * z.x, z.y * z.y};
		if (z.x == z_old.x && z.y == z_old.y)
			depth = depth_max;
		z_old = z;
	}
	if (depth == depth_max)
		return (0);
	return (depth);
}
#else
int	get_mandelbrot_value(t_float2 c, int depth_max)
{
	t_float2	z;
	t_float2	z_squared;
	int			depth;

	depth = 0;
	z = (t_float2){};
	z_squared = (t_float2){z.x * z.x, z.y * z.y};
	while (((z_squared.x + z_squared.y) < 4) && (depth < depth_max))
	{
		z.y = 2 * z.x * z.y + c.y;
		z.x = z_squared.x - z_squared.y + c.x;
		depth++;
		z_squared = (t_float2){z.x * z.x, z.y * z.y};
	}
	if (depth == depth_max)
		return (0);
	return (depth - 1);
}
#endif
void	app_draw(t_app app)
{
	int			x;
	int			y;
	t_float2	c;
	int			depth;

	y = 0;
	while (y < app.win_size.y)
	{
		x = 0;
		c.y = (y / app.win_size.y) * (app.config.z_size.y) + (app.config.z_min.y);
		while (x < app.win_size.x)
		{
			c.x = (x / app.win_size.x) * (app.config.z_size.x) + (app.config.z_min.x);
//			c.x = (x / app.win_size.x) * (app.config.z_max.x - app.config.z_min.x) + (app.config.z_min.x);
			depth = get_mandelbrot_value(c, app.config.depth_max);
			int channel = (255.f * (depth / (float)app.config.depth_max));
			app.pixels[(int)(y * app.win_size.x + x)] = (0xFF & channel) << 8;
			x++;
		}
		y++;
	}
}
