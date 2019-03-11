/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdarchiv <tdarchiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/24 14:38:58 by tdarchiv          #+#    #+#             */
/*   Updated: 2019/03/11 13:42:02 by tdarchiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app.h"

#include "multithreading.h"
#include "events.h"
#include "delta_draw.h"
#include "mouse_events.h"
#include "libft/libft.h"
#include "key_event.h"

#include <stdlib.h>
#include <mlx.h>

void	app_init(t_app *app, int fractal_index)
{
	int			osef;
	t_double2	win_size;

	win_size = (t_double2){WIDTH, HEIGHT};
	ft_memzero(app, 1, sizeof(t_app));
	app->mlx_context = mlx_init();
	app->config = config_init(win_size, fractal_index);
	app->thread_count = THREAD_COUNT;
	app->thread_pool = create_thread_pool(app->thread_count);
	app->need_full_redraw = true;
	ft_memzero(app->keystate, 1, sizeof(app->keystate));
	app->mlx_window = mlx_new_window(app->mlx_context, win_size.x, win_size.y,
			"Fractol");
	app->mlx_texture = mlx_new_image(app->mlx_context, win_size.x, win_size.y);
	app->frame.size = win_size;
	app->frame.iter = malloc(sizeof(uint16_t) * win_size.x * win_size.y);
	app->frame.pixels = (uint32_t*)mlx_get_data_addr(app->mlx_texture,
														&osef, &osef, &osef);
	ft_memzero(app->frame.iter, 1, sizeof(uint16_t) * win_size.x * win_size.y);
	set_hooks(app);
}

void	set_hooks(t_app *app)
{
	int			osef;

	osef = 0xDEADBEEF;
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
	t_double2	delta;

	delta.x = app->keystate[KEY_RIGHT] - app->keystate[KEY_LEFT];
	delta.y = app->keystate[KEY_DOWN] - app->keystate[KEY_UP];
	delta.x *= 4;
	delta.y *= 4;
	if (delta.x != 0 || delta.y != 0)
		delta_draw(delta, &app->config, app->frame, app->thread_pool);
	if (app->keystate[KEY_PLUS])
		app->config.depth_max += 50;
	if (app->keystate[KEY_MINUS])
		app->config.depth_max -= 50 * (app->config.depth_max >= 50);
	if (app->keystate[KEY_PLUS] || app->keystate[KEY_MINUS])
		app->need_full_redraw = true;
}

int		app_callback(void *param)
{
	t_app	*app;
	t_rect	rect;

	app = param;
	app_update(app);
	if (app->need_full_redraw)
	{
		rect = (t_rect){{0, 0}, app->frame.size};
		compute_region_parallel(app->config, app->thread_pool, app->frame,
																		rect);
		draw_color(app->config, app->frame);
	}
	draw_screen(app);
	app->need_full_redraw = false;
	return (0);
}
