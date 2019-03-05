/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memzero.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdarchiv <tdarchiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/09 18:41:13 by tdarchiv          #+#    #+#             */
/*   Updated: 2018/10/09 18:41:13 by tdarchiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stddef.h>

static void	ft_memzero_byte(void *src, size_t count)
{
	while (--count)
		((char*)src)[count] = 0;
}

void		ft_memzero(void *src, register size_t elem_count, size_t elem_size)
{
	register int *ptr;

	if (elem_size != 4)
		return (ft_memzero_byte(src, elem_count * elem_size));
	ptr = src;
	while (--elem_count)
		*ptr++ = 0;
}
