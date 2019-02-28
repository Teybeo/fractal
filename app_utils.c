/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdarchiv <tdarchiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 18:09:58 by tdarchiv          #+#    #+#             */
/*   Updated: 2019/02/28 18:25:14 by tdarchiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app.h"

#include <mlx.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

void	draw_screen(t_app *app)
{
	mlx_clear_window(app->mlx_context, app->mlx_window);
	mlx_put_image_to_window(
			app->mlx_context, app->mlx_window, app->mlx_texture, 0, 0);
	app_draw_ui(*app);
}

void	app_draw_ui(t_app app)
{
	float	time;

	time = get_frametime();
	draw_string(app, 10, 10, "Frametime: %7.2f ms (%7.2f fps)",
			time, 1000 / time);
	draw_string(app, 10, 30, "Depth max: %-5d", app.config.depth_max);
	draw_string(app, 10, 50, "Size: %g, %g",
			app.config.z_size.x, app.config.z_size.y);
	draw_string(app, 10, 70, "Center: %g, %g",
			app.config.z_min.x + app.config.z_size.x / 2,
			app.config.z_min.y + app.config.z_size.y / 2);
	draw_string(app, 10, 90, "Threads: %d", app.thread_count);
	draw_string(app, 10, 110, "Lines per chunk: %d",
			app.config.lines_per_chunk);
	draw_string(app, 10, 130, "Chunk count %d",
			get_chunk_count(app.frame.size.y, app.config.lines_per_chunk));
	if (app.need_full_redraw)
		draw_string(app, 10, 150, "FULL REDRAW");
}

void	draw_string(t_app app, int x, int y, const char *fmt, ...)
{
	char	string[512];
	va_list	args;

	va_start(args, fmt);
	memset(string, 0, sizeof(string));
	vsprintf(string, fmt, args);
	mlx_string_put(app.mlx_context, app.mlx_window, x, y, 0x00FFFFFF, string);
	va_end(args);
}

float	get_frametime(void)
{
	static struct timespec	start = {0, 0};
	struct timespec			now;
	float					duration;

	clock_gettime(CLOCK_REALTIME, &now);
	duration = (now.tv_sec - start.tv_sec) * 1000
			+ (now.tv_nsec - start.tv_nsec) / 1000000.f;
	start = now;
	return (duration);
}
