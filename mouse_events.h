/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_events.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdarchiv <tdarchiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/23 16:55:05 by tdarchiv          #+#    #+#             */
/*   Updated: 2019/02/23 16:55:05 by tdarchiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_MOUSE_EVENTS_H
# define FRACTOL_MOUSE_EVENTS_H

int	mouse_move(int x, int y, void *param);
int	mouse_down(int button, int x, int y, void *param);
int	mouse_up(int button, int x, int y, void *param);

#endif
