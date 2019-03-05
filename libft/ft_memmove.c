/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdarchiv <tdarchiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/06 15:30:06 by tdarchiv          #+#    #+#             */
/*   Updated: 2018/04/06 16:41:19 by tdarchiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *z_dst, const void *z_src, size_t count)
{
	const char	*src;
	char		*dst;

	src = z_src;
	dst = z_dst;
	if (src < dst)
	{
		while (count-- > 0)
			dst[count] = src[count];
	}
	else
	{
		while (count-- > 0)
			*dst++ = *src++;
	}
	return (z_dst);
}
