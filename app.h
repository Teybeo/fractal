/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdarchiv <tdarchiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/11 15:08:17 by tdarchiv          #+#    #+#             */
/*   Updated: 2019/02/18 15:08:20 by tdarchiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef APP_H
# define APP_H

# include "t_double2.h"
# include "config.h"
# include "multithreading.h"
# include "coloring.h"
# include "thread_pool.h"
# include "events.h"

# include <stdint.h>
# include <stdbool.h>

#define USE_THREAD_POOL 1
#define THREAD_COUNT 8

struct	s_app
{
	void			*mlx_context;
	void			*mlx_texture;
	void			*mlx_window;
	t_surface		surface;
	t_surface16		iter_buffer;
	t_config		config;
	t_thread_pool	*thread_pool;
	int				thread_count;
	bool			keystate[KEY_LAST];
	bool			hold_left_click;
	bool			hold_right_click;
	bool			need_full_redraw;
};
typedef struct s_app	t_app;

void	app_init(t_app *app);
void	app_run(t_app *app);
int		app_callback(void *param);
void	app_draw_ui(t_app app);

#endif
