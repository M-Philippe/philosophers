/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   meal.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pminne <pminne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/19 13:36:56 by pminne            #+#    #+#             */
/*   Updated: 2020/09/21 13:51:07 by pminne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int			checking_death(t_table *philo, long t_stamp)
{
	if (t_stamp - philo->last_meal > philo->time_to_starve)
	{
		sem_wait(philo->g_mtx->sem_dead);
		if (g_someone_is_dead == 1)
		{
			sem_post(philo->g_mtx->sem_dead);
			sem_post(philo->sem_meal);
			return (1);
		}
		g_someone_is_dead = 1;
		sem_post(philo->g_mtx->sem_dead);
		print_death(philo, t_stamp - philo->start_program);
		sem_post(philo->sem_meal);
		return (1);
	}
	return (0);
}

void		*philo_meal(void *arg)
{
	t_table *philo;
	long	t_stamp;

	philo = arg;
	while (1)
	{
		sem_wait(philo->sem_meal);
		t_stamp = timestamp();
		if (philo->stop_meal == 1)
		{
			sem_post(philo->sem_meal);
			break ;
		}
		if (checking_death(philo, t_stamp))
			break ;
		sem_post(philo->sem_meal);
		usleep(1000);
	}
	sem_wait(philo->g_mtx->sem_meals);
	g_meals_are_done++;
	sem_post(philo->g_mtx->sem_meals);
	return (NULL);
}
