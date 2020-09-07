/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_philosophers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pminne <pminne@42lyon.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/07 17:15:26 by pminne            #+#    #+#             */
/*   Updated: 2020/09/08 00:04:58 by pminne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int				swap2(t_table *philo, t_table **tmp)
{
	philo->next = NULL;
	*tmp = philo;
	philo = philo->next;
	return (1);
}

void			last_swap(t_table *tmp, t_table *head)
{
	tmp->next = head;
	head->prev = tmp;
}

int				meal_and_fork(t_table *philo, t_write *write)
{
	if (!(philo->meal = set_meal(philo->start_program,
		philo->time_to_starve)))
		return (1);
	if (set_fork(philo, write))
		return (1);
	return (0);
}

void			*free_table(t_table *philo, t_write *write)
{
	t_table		*tmp;

	tmp = NULL;
	free(write);
	while (philo)
	{
		tmp = philo;
		philo = philo->prev;
		(tmp->meal) ? (free(tmp->meal)) : 0;
		(tmp->r_fork) ? (free(tmp->r_fork)) : 0;
		free(tmp);
		tmp = NULL;
	}
	return (NULL);
}

t_table			*set_philosophers2(t_args *args, t_monitor *mtr,
	t_write *write, int count)
{
	long	start_program;
	t_table	*philo;
	t_table	*head;
	t_table	*tmp;

	tmp = NULL;
	philo = NULL;
	head = NULL;
	start_program = timestamp();
	while (count <= args->nb_philo)
	{
		if (!(philo = malloc(sizeof(t_table))))
			return (free_table(tmp, write));
		(tmp) ? swap1(tmp, philo) : 0;
		(count == 1) ? (philo->prev = NULL) : 0;
		(count == 1) ? (head = philo) : 0;
		copy_args(philo, args, count, start_program);
		philo->monitor = mtr;
		if (meal_and_fork(philo, write))
			return (free_table(philo, write));
		count += swap2(philo, &tmp);
	}
	last_swap(tmp, head);
	return (head);
}

t_table			*set_philosophers(t_args *args, t_monitor *mtr)
{
	int		count;
	t_write	*write;

	count = 1;
	if (!(write = malloc(sizeof(t_write))))
		return (NULL);
	pthread_mutex_init(&write->writing, NULL);
	return (set_philosophers2(args, mtr, write, count));
}
