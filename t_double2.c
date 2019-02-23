/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_double2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdarchiv <tdarchiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/23 17:33:09 by tdarchiv          #+#    #+#             */
/*   Updated: 2019/02/23 21:01:39 by tdarchiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "t_double2.h"

t_double2	double2_sub(t_double2 a, t_double2 b)
{
	return (t_double2){
			a.x - b.x,
			a.y - b.y};
}

t_double2	double2_add(t_double2 a, t_double2 b)
{
	return (t_double2){
			a.x + b.x,
			a.y + b.y};
}

void		double2_add_this(t_double2 *this, t_double2 b)
{
	this->x += b.x;
	this->y += b.y;
}

void		double2_sub_this(t_double2 *this, t_double2 b)
{
	this->x -= b.x;
	this->y -= b.y;
}

void		double2_mul_this(t_double2 *this, double x)
{
	this->x *= x;
	this->y *= x;
}
