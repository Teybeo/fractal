#include "app.h"

#include <mlx.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <pthread.h>

t_config	config_init()
{
	t_config config;
	config.depth_max = 100;
//	config.z_min = (t_float2){-2, -2};
//	config.z_max = (t_float2){2, 2};
	config.z_min = (t_float2){-0.6, -0.};
	config.z_max = (t_float2){-0.8, -0.2};
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
		app->thread_count -= (app->thread_count != 0);
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
		config_zoom_to(app, x, y);
	if (button == 5)
		config_dezoom_from(app, x, y);
	return 0;
}

int	mouse_up(int button, int x, int y, void *param)
{
	t_app *app;

	app = param;
	if (button == 1)
		app->is_dragging = false;
	return 0;
}

typedef struct s_rect {
	t_float2 origin;
	t_float2 size;
} t_rect;

void	copy_region(t_rect src, t_rect dst, t_float2 bounds, uint32_t *data)
{
	if ((dst.origin.y > src.origin.y) || ((dst.origin.y == src.origin.y) && (dst.origin.x > src.origin.y)))
	{
		for (int y = src.size.y - 1; y >= 0; --y)
		{
			for (int x = src.size.x - 1; x >= 0; --x)
			{
				int src_index = (int) ((src.origin.y + y) * bounds.x + src.origin.x + x);
				int dst_index = (int) ((dst.origin.y + y) * bounds.x + dst.origin.x + x);
				data[dst_index] = data[src_index];
			}
		}
	}
	else
	{
		for (int y = 0; y < src.size.y; ++y)
		{
			for (int x = 0; x < src.size.x; ++x)
			{
				int src_index = (int) ((src.origin.y + y) * bounds.x + src.origin.x + x);
				int dst_index = (int) ((dst.origin.y + y) * bounds.x + dst.origin.x + x);
				data[dst_index] = data[src_index];
			}
		}
	}
}

void	app_draw_parallel(t_app app)
;

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
		t_rect src = {};
		t_rect dst = {};
		src.size = app->win_size;
		if (delta.x < 0)
		{
			src.origin.x = 0;
			src.size.x = app->win_size.x + delta.x;
			dst.origin.x = -delta.x;
		}
		else if (delta.x > 0)
		{
			src.origin.x = delta.x;
			src.size.x = app->win_size.x - delta.x;
			dst.origin.x = 0;
		}
		if (delta.y < 0)
		{
			src.origin.y = 0;
			src.size.y = app->win_size.y + delta.y;
			dst.origin.y = -delta.y;
		}
		else if (delta.y > 0)
		{
			src.origin.y = delta.y;
			src.size.y = app->win_size.y - delta.y;
			dst.origin.y = 0;
		}
		delta.x = (delta.x / app->win_size.x) * app->config.z_size.x;
		delta.y = (delta.y / app->win_size.y) * app->config.z_size.y;
		float2_add_this(&app->config.z_max, delta);
		float2_add_this(&app->config.z_min, delta);
		copy_region(src, dst, app->win_size, app->pixels);
		mlx_put_image_to_window(app->mlx_context, app->mlx_window, app->mlx_texture, 0, 0);
	}
	return (0);
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
	app->thread_count = 1;
	app->is_dragging = false;
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
	mlx_hook(app->mlx_window, 5, osef, mouse_up, app);
	mlx_hook(app->mlx_window, 6, osef, mouse_move, app);
	mlx_hook(app->mlx_window, 17, osef, quit_event, NULL);
//	mlx_expose_hook(app->mlx_window, expose_callback, app);
	mlx_loop_hook(app->mlx_context, app_callback, app);
	mlx_loop(app->mlx_context);
}

float get_frametime();
void	app_draw_ui(t_app app);
void	app_draw_parallel(t_app app);

void	app_update(t_app *app)
{
	t_float2	delta;
	delta = (t_float2){};

	if (app->keystate[KEY_UP])
		delta.y -= 0.02f * app->config.z_size.y;
	if (app->keystate[KEY_DOWN])
		delta.y += 0.02f * app->config.z_size.y;
	if (app->keystate[KEY_RIGHT])
		delta.x += 0.02f * app->config.z_size.x;
	if (app->keystate[KEY_LEFT])
		delta.x -= 0.02f * app->config.z_size.x;

	float2_add_this(&app->config.z_min, delta);
	float2_add_this(&app->config.z_max, delta);

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
//	app_draw(*app);
	app_draw_parallel(*app);
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

#if 1
int	get_mandelbrot_value(t_float2 c, int depth_max)
{
	t_float2	z;
//	t_float2	z_old;
	int			depth;

	depth = 0;
	z = c;
//	z_old = z;
//	while (((z.x < 2 && z.x > -2) && (z.y < 2 && z.y > -2)) && (depth < depth_max))
	while (((z.x * z.x) + (z.y * z.y) < 4) && (depth < depth_max))
	{
		float z_y_temp = 2 * z.x * z.y + c.y;
//		z.y = 2 * z.x * z.y + c.y;
//		z.x = z_squared.x - z_squared.y + c.x;
		z.x = (z.x * z.x) - (z.y * z.y) + c.x;
		z.y = z_y_temp;
		depth++;
//		if (z.x == z_old.x && z.y == z_old.y)
//		{
//			depth = depth_max;
//			break;
//		}
//		z_old = z;
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

#define MAX_THREAD 16

typedef struct thread_config {
	t_config	config;
	t_float2	win_size;
	int			first_line;
	int			last_line;
	uint32_t	*pixels;
	char		thread_id;
} thread_config;

void	prepare_threads(t_app app, thread_config *thread_list)
{
	int i;
	i = 0;
	while (i < app.thread_count)
	{
		thread_list[i].win_size = app.win_size;
		thread_list[i].first_line = i * (app.win_size.y / app.thread_count);
		thread_list[i].last_line = (i + 1) * (app.win_size.y / app.thread_count);
		thread_list[i].config = app.config;
		thread_list[i].pixels = app.pixels;
		thread_list[i].thread_id = i;
		i++;
	}
}

void	*partial_draw(void *param)
{
	int				x;
	int				y;
	t_float2		c;
	int				depth;
	thread_config	conf;

	conf = *(thread_config*)param;
//	int pixel_id;
//	float inv_x = 1.f / conf.win_size.x;
//	float inv_y = 1 / conf.win_size.y;
	y = conf.first_line;
	while (y < conf.last_line)
	{
		x = 0;
		c.y = (y / conf.win_size.y) * (conf.config.z_size.y) + (conf.config.z_min.y);
//		c.y = (y * inv_y) * (conf.config.z_size.y) + (conf.config.z_min.y);
//		pixel_id = y * conf.win_size.x;
		while (x < conf.win_size.x)
		{
			c.x = (x / conf.win_size.x) * (conf.config.z_size.x) + (conf.config.z_min.x);
//			c.x = (x * inv_x) * (conf.config.z_size.x) + (conf.config.z_min.x);
//			c.x = (x / conf.win_size.x) * (conf.config.z_max.x - conf.config.z_min.x) + (conf.config.z_min.x);
			depth = get_mandelbrot_value(c, conf.config.depth_max);
			int channel = (255.f * (depth / (float)conf.config.depth_max));
			conf.pixels[(int)(y * conf.win_size.x + x)] = (0xFF & channel) << 8;
//			conf.pixels[(int)(y * conf.win_size.x + x)] |= (0xFF & conf.thread_id * 16) << 16;
			x++;
		}
		y++;
	}
	return NULL;
}


void	app_draw_parallel(t_app app)
{
	int i;

	thread_config thread_config_list[MAX_THREAD] = {};
//	thread_config *thread_config_list = malloc(sizeof(thread_config) * app.thread_count);
	pthread_t thread_list[MAX_THREAD] = {};
	prepare_threads(app, thread_config_list);
	i = 0;
	while (i < app.thread_count)
	{
		pthread_create(thread_list + i, NULL, partial_draw, thread_config_list + i);
		i++;
	}

	i = 0;
	while (i < app.thread_count)
	{
		pthread_join(thread_list[i], NULL);
		i++;
	}

//	free(thread_config_list);
}

