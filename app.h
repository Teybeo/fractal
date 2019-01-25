/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdarchiv <tdarchiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/11 15:08:17 by tdarchiv          #+#    #+#             */
/*   Updated: 2019/01/25 15:33:37 by tdarchiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef APP_H
# define APP_H

# include "t_float2.h"
# include "config.h"
# include "drawing.h"
# include "coloring.h"
# include "thread_pool.h"

# include <stdint.h>
# include <stdbool.h>

#define USE_THREAD_POOL 1

enum	e_key
{
	KEY_ESCAPE = 53,
	KEY_UP = 13,
	KEY_DOWN = 1,
	KEY_RIGHT = 2,
	KEY_LEFT = 0,
	KEY_R = 15,
	KEY_MINUS = 7,
	KEY_PLUS = 8,
	KEY_ZOOM = 12,
	KEY_DEZOOM = 6,
	KEY_LESS = 78,
	KEY_MORE = 69,
	KEY_LAST = 512
};

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
	bool			is_dragging;
	bool			need_full_redraw;
};
typedef struct s_app	t_app;

void	app_init(t_app *app);
t_app	app_run();
int		app_callback(void *param);

#endif
