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

int				meal_and_fork(t_table *philo, t_write *write, sem_t *sem)
{
	if (!(philo->meal = set_meal(philo->start_program,
		philo->time_to_starve, philo->id)))
		return (1);
	//if (set_fork(philo, write, sem))
	//	return (1);
	set_fork(philo, write);
	philo->forks = sem;
	return (0);
}

t_table			*set_philosophers2(t_args *args, t_monitor *mtr,
	t_write *write, int count)
{
	long	start_program;
	t_table	*philo;
	t_table	*head;
	t_table	*tmp;
	sem_t	*forks;

	tmp = NULL;
	philo = NULL;
	head = NULL;
	// FORKS SEMAPHORE
	forks = sem_open("/fork", O_CREAT, 777, args->nb_philo);
	if (forks == SEM_FAILED)
		exit(0);
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
		if (meal_and_fork(philo, write, forks))
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
	write->write_sem = sem_open("/write", O_CREAT, 777, 1);
	if (write->write_sem == SEM_FAILED)
		exit(0);
	return (set_philosophers2(args, mtr, write, count));
}
