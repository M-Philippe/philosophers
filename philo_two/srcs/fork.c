/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pminne <pminne@42lyon.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/07 17:14:57 by pminne            #+#    #+#             */
/*   Updated: 2020/09/08 00:04:34 by pminne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	take_fork(t_table *philo)
{
	if (philo->id == philo->nb_philo)
	{
		//pthread_mutex_lock(&philo->r_fork->fork);
		sem_wait(philo->forks);
		print_state(philo, philo->id, FORK);
		sem_wait(philo->forks);
		//pthread_mutex_lock(&philo->prev->r_fork->fork);
		print_state(philo, philo->id, FORK);
	}
	else
	{
		//pthread_mutex_lock(&philo->prev->r_fork->fork);
		sem_wait(philo->forks);
		print_state(philo, philo->id, FORK);
		//pthread_mutex_lock(&philo->r_fork->fork);
		sem_wait(philo->forks);
		print_state(philo, philo->id, FORK);
	}
}

void	free_fork(t_table *philo)
{
	sem_post(philo->forks);
	sem_post(philo->forks);
	//pthread_mutex_unlock(&philo->prev->r_fork->fork);
	//pthread_mutex_unlock(&philo->r_fork->fork);
}
