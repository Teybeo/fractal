/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdarchiv <tdarchiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/11 15:08:17 by tdarchiv          #+#    #+#             */
/*   Updated: 2018/12/17 17:35:43 by tdarchiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef APP_H
# define APP_H

# include "t_float2.h"

# include <stdint.h>
# include <stdbool.h>

enum	e_key
{
	KEY_ESCAPE = 53,
	KEY_UP = 126,
	KEY_DOWN = 125,
	KEY_RIGHT = 124,
	KEY_LEFT = 123,
	KEY_R = 15,
	KEY_PLUS = 69,
	KEY_MINUS = 78,
	KEY_ZOOM = 116,
	KEY_DEZOOM = 121,
	KEY_LAST = 512
};

struct	s_config
{
	int			depth_max;
	t_float2	z_min;
	t_float2	z_max;
	t_float2	z_size;
};
typedef struct s_config	t_config;

struct	s_app
{
	void		*mlx_context;
	void		*mlx_texture;
	void		*mlx_window;
	t_float2	win_size;
	uint32_t	*pixels;
	t_config	config;
	bool		keystate[KEY_LAST];
	bool		need_redraw;
};
typedef struct s_app	t_app;

void	app_init(t_app *app);
void	app_draw(t_app app);
t_app	app_run();
int		app_callback(void *param);

#endif
