/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pminne <pminne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/19 13:47:56 by pminne            #+#    #+#             */
/*   Updated: 2020/09/19 13:49:23 by pminne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void		take_fork(t_table *philo, int id, int other_hand)
{
	if (philo->id == philo->nb_philo - 1)
	{
		pthread_mutex_lock(&philo->fork[id].fork);
		print_state(philo, id, FORK);
		pthread_mutex_lock(&philo->fork[other_hand].fork);
		print_state(philo, id, FORK);
	}
	else
	{
		pthread_mutex_lock(&philo->fork[other_hand].fork);
		print_state(philo, id, FORK);
		pthread_mutex_lock(&philo->fork[id].fork);
		print_state(philo, id, FORK);
	}
}

void		free_fork(t_table *philo, int id, int other_hand)
{
	if (philo->id == philo->nb_philo - 1)
	{
		pthread_mutex_unlock(&philo->fork[id].fork);
		pthread_mutex_unlock(&philo->fork[other_hand].fork);
	}
	else
	{
		pthread_mutex_unlock(&philo->fork[other_hand].fork);
		pthread_mutex_unlock(&philo->fork[id].fork);
	}
}
