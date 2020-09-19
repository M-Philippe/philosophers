/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   meal.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pminne <pminne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/19 13:36:56 by pminne            #+#    #+#             */
/*   Updated: 2020/09/19 14:00:14 by pminne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int			checking_death(t_table *philo, long t_stamp)
{
	if (t_stamp - philo->last_meal > philo->time_to_starve)
	{
		pthread_mutex_lock(&philo->g_mtx->g_dead);
		if (g_someone_is_dead == 1)
		{
			pthread_mutex_unlock(&philo->g_mtx->g_dead);
			pthread_mutex_unlock(&philo->meal);
			return (1);
		}
		g_someone_is_dead = 1;
		pthread_mutex_unlock(&philo->g_mtx->g_dead);
		print_death(philo, t_stamp - philo->start_program);
		pthread_mutex_unlock(&philo->meal);
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
		pthread_mutex_lock(&philo->meal);
		t_stamp = timestamp();
		if (philo->stop_meal == 1)
		{
			pthread_mutex_unlock(&philo->meal);
			break ;
		}
		if (checking_death(philo, t_stamp))
			break ;
		pthread_mutex_unlock(&philo->meal);
		usleep(1000);
	}
	pthread_mutex_lock(&philo->g_mtx->g_meals);
	g_meals_are_done++;
	pthread_mutex_unlock(&philo->g_mtx->g_meals);
	return (NULL);
}
