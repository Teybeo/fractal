/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdarchiv <tdarchiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/23 16:59:00 by tdarchiv          #+#    #+#             */
/*   Updated: 2019/02/23 16:59:00 by tdarchiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENTS_H
# define EVENTS_H

enum	e_key
{
	KEY_ESCAPE = 53,
	KEY_UP = 13,
	KEY_DOWN = 1,
	KEY_RIGHT = 2,
	KEY_LEFT = 0,
	KEY_R = 15,
	KEY_G = 5,
	KEY_MINUS = 7,
	KEY_PLUS = 8,
	KEY_ZOOM = 12,
	KEY_DEZOOM = 6,
	KEY_P = 35,
	KEY_LESS = 78,
	KEY_MORE = 69,
	KEY_1 = 18,
	KEY_2 = 19,
	KEY_3 = 20,
	KEY_KP_1 = 83,
	KEY_KP_2 = 84,
	KEY_LAST = 512
};

int	keydown_event(int keycode, void *param);
int	keyup_event(int key, void *param);
int	expose_callback(void *param);
int	quit_event(void);

#endif
