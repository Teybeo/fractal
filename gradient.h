/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gradient.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdarchiv <tdarchiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/23 16:57:31 by tdarchiv          #+#    #+#             */
/*   Updated: 2019/02/23 21:01:39 by tdarchiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_GRADIENT_H
# define FRACTOL_GRADIENT_H

# include "rgb.h"

typedef struct	s_grad_step
{
	t_rgb		color;
	float		treshold;
}				t_grad_step;

typedef struct	s_gradient
{
	t_grad_step	*step;
	int			step_count;
}				t_gradient;

t_gradient		create_gradient(int gradient_type);
void			destroy_gradient(t_gradient gradient);
uint32_t		get_color_from_gradient(t_gradient gradient, float percent);
void			get_color_interval(t_gradient gradient, float percent,
									t_grad_step *a, t_grad_step *b);

#endif
