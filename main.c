/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdarchiv <tdarchiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/23 18:11:09 by tdarchiv          #+#    #+#             */
/*   Updated: 2019/02/23 18:11:14 by tdarchiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "app.h"
#include <stdio.h>

int	main(void)
{
	t_app	app;

	setbuf(stdout, NULL);
	app_init(&app);
	app_run(&app);
	return (0);
}
