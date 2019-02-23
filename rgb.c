/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rgb.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdarchiv <tdarchiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/23 17:41:03 by tdarchiv          #+#    #+#             */
/*   Updated: 2019/02/23 17:41:03 by tdarchiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rgb.h"

t_rgb		color_mix(float factor, t_rgb a, t_rgb b)
{
	t_rgb output;

	output.r = ((1 - factor) * a.r) + (factor * b.r);
	output.g = ((1 - factor) * a.g) + (factor * b.g);
	output.b = ((1 - factor) * a.b) + (factor * b.b);
	return (output);
}

uint32_t	rgb_pack(t_rgb rgb)
{
	uint32_t res;

	res = 0;
	res += (uint8_t)(rgb.r * 255) << R_SHIFT;
	res += (uint8_t)(rgb.g * 255) << G_SHIFT;
	res += (uint8_t)(rgb.b * 255) << B_SHIFT;
	return (res);
}
