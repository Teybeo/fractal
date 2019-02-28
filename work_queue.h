/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   work_queue.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdarchiv <tdarchiv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/28 16:35:05 by tdarchiv          #+#    #+#             */
/*   Updated: 2019/02/28 16:35:29 by tdarchiv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WORK_QUEUE_H
# define WORK_QUEUE_H

struct					s_work_queue {
	void				*data;
	void				*(*task)(void*);
	struct s_work_queue	*next;
};
typedef struct s_work_queue	t_work_queue;

void					push_work(t_work_queue **queue, t_work_queue *new_work);
t_work_queue			*pop_work(t_work_queue **queue);

#endif
