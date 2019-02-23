/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rgb.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdarchiv <tdarchiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/23 16:29:51 by tdarchiv          #+#    #+#             */
/*   Updated: 2019/02/23 21:01:39 by tdarchiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_RGB_H
# define FRACTOL_RGB_H

# include <stdint.h>

# define R_SHIFT (16u)
# define G_SHIFT (8u)
# define B_SHIFT (0u)

struct		s_rgb
{
	float r;
	float g;
	float b;
};
typedef struct s_rgb	t_rgb;

t_rgb		color_mix(float factor, t_rgb a, t_rgb b);
uint32_t	rgb_pack(t_rgb rgb);

#endif
