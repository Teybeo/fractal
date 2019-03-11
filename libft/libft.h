/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdarchiv <tdarchiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/05 19:50:39 by tdarchiv          #+#    #+#             */
/*   Updated: 2019/03/11 13:41:59 by tdarchiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stddef.h>

void	ft_memzero(void *src, register size_t elem_count, size_t elem_size);
void	*ft_memcpy(void *dst, const void *src, size_t count);
void	*ft_memmove(void *z_dst, const void *z_src, size_t count);

size_t	ft_strlen(const char *s);
void	ft_puts(char const *s);
void	ft_putstr(char const *s);
int		ft_strequ(char const *a, char const *b);

#endif
