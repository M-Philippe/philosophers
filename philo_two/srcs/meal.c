/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   meal.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pminne <pminne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/19 13:36:56 by pminne            #+#    #+#             */
/*   Updated: 2020/09/20 22:55:27 by pminne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int			checking_death(t_table *philo, long t_stamp)
{
	if (t_stamp - philo->last_meal > philo->time_to_starve)
	{
		//pthread_mutex_lock(&philo->g_mtx->g_dead);
		sem_wait(philo->g_mtx->sem_dead);
		if (g_someone_is_dead == 1)
		{
			//pthread_mutex_unlock(&philo->g_mtx->g_dead);
			sem_post(philo->g_mtx->sem_dead);
			//pthread_mutex_unlock(&philo->meal);
			sem_post(philo->sem_meal);
			return (1);
		}
		g_someone_is_dead = 1;
		//pthread_mutex_unlock(&philo->g_mtx->g_dead);
		sem_post(philo->g_mtx->sem_dead);
		print_death(philo, t_stamp - philo->start_program);
		//pthread_mutex_unlock(&philo->meal);
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
		//pthread_mutex_lock(&philo->meal);
		sem_wait(philo->sem_meal);
		t_stamp = timestamp();
		if (philo->stop_meal == 1)
		{
			sem_post(philo->sem_meal);
			//pthread_mutex_unlock(&philo->meal);
			break ;
		}
		if (checking_death(philo, t_stamp))
			break ;
		sem_post(philo->sem_meal);
		//pthread_mutex_unlock(&philo->meal);
		usleep(1000);
	}
	//pthread_mutex_lock(&philo->g_mtx->g_meals);
	sem_wait(philo->g_mtx->sem_meals);
	g_meals_are_done++;
	//pthread_mutex_unlock(&philo->g_mtx->g_meals);
	sem_post(philo->g_mtx->sem_meals);
	return (NULL);
}
