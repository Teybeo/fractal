/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   work_queue.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdarchiv <tdarchiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 16:35:09 by tdarchiv          #+#    #+#             */
/*   Updated: 2019/02/28 16:35:14 by tdarchiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "work_queue.h"
#include <stddef.h>

void			push_work(t_work_queue **queue, t_work_queue *new_work)
{
	t_work_queue *work_queue;
	t_work_queue *ptr;

	work_queue = *queue;
	if (work_queue == NULL)
	{
		*queue = new_work;
		return ;
	}
	ptr = work_queue;
	while (ptr->next != NULL)
		ptr = ptr->next;
	ptr->next = new_work;
}

t_work_queue	*pop_work(t_work_queue **queue)
{
	t_work_queue *work_queue;
	t_work_queue *work_to_pop;

	work_queue = *queue;
	if (work_queue == NULL)
		return (NULL);
	work_to_pop = work_queue;
	work_queue = work_queue->next;
	*queue = work_queue;
	return (work_to_pop);
}
