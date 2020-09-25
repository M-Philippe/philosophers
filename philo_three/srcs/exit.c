/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pminne <pminne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/25 20:21:44 by pminne            #+#    #+#             */
/*   Updated: 2020/09/25 20:28:37 by pminne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void		exit_philosophize(t_table *philo)
{
	philo->stop_meal = 1;
	sem_post(philo->sem_meal);
	sem_post(philo->g_mtx->sem_done);
	exit(0);
}

void		clean_free(t_table *philo, t_args *args)
{
	destroy_semaphore(philo, args, philo[0].fork);
	free(args);
	free(philo[0].fork);
	free(philo[0].g_mtx);
	free(philo[0].write);
	free(philo);
}

void		destroy_semaphore(t_table *philo, t_args *args, t_fork *fork)
{
	int		i;

	i = 0;
	sem_close(philo[0].g_mtx->sem_dead);
	sem_close(philo[0].g_mtx->sem_done);
	sem_close(fork->sem_forks);
	while (i < args->nb_philo)
	{
		sem_close(philo[i].sem_meal);
		i++;
	}
	sem_close(philo[0].write->sem_write);
}

void		unlink_philo_semaphore(t_table *philo)
{
	int		i;

	i = 0;
	while (i < philo[0].nb_philo)
	{
		sem_unlink(philo[i].sem_name);
		i++;
	}
}
