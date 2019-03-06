/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdarchiv <tdarchiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/23 19:01:22 by tdarchiv          #+#    #+#             */
/*   Updated: 2019/03/06 20:02:49 by tdarchiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "events.h"

#include "libft/libft.h"

#include <stdlib.h>

int		expose_callback(void *param)
{
	ft_puts("EXPOSE CALLBACK");
	(void)param;
	return (0);
}

int		quit_event(void)
{
	ft_puts("Bye");
	exit(0);
	return (0);
}
