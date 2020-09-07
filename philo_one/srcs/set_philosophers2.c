/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_philosophers2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pminne <pminne@42lyon.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/07 20:04:49 by pminne            #+#    #+#             */
/*   Updated: 2020/09/08 00:05:01 by pminne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_info			*set_meal(long start_program, long time_to_starve)
{
	t_info	*meal;

	meal = NULL;
	if (!(meal = malloc(sizeof(t_info))))
		return (NULL);
	pthread_mutex_init(&meal->mtx, NULL);
	meal->last_meal = 0;
	meal->time_meal = 0;
	meal->start_program = start_program;
	meal->time_to_starve = time_to_starve;
	return (meal);
}

void			copy_args(t_table *philo, t_args *args,
	int count, long start_program)
{
	philo->id = count;
	philo->nb_philo = args->nb_philo;
	philo->time_to_eat = args->time_to_eat;
	philo->time_to_sleep = args->time_to_sleep;
	philo->time_to_starve = args->time_to_starve;
	philo->turns = args->n_time_must_eat;
	if (philo->turns == 0)
		philo->turns = -1;
	philo->start_program = start_program;
}

void			swap1(t_table *tmp, t_table *philo)
{
	if (tmp)
	{
		tmp->next = philo;
		philo->prev = tmp;
	}
}

int				set_fork(t_table *philo, t_write *write)
{
	philo->write = write;
	if (!(philo->r_fork = malloc(sizeof(t_fork))))
		return (1);
	pthread_mutex_init(&philo->r_fork->fork, NULL);
	return (0);
}
