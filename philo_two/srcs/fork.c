/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pminne <pminne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/19 13:47:56 by pminne            #+#    #+#             */
/*   Updated: 2020/09/20 22:42:58 by pminne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void		take_fork(t_table *philo, int id, int other_hand)
{
	sem_wait(philo->fork->sem_forks);
	print_state(philo, id, FORK);
	sem_wait(philo->fork->sem_forks);
	print_state(philo, id, FORK);
}

void		free_fork(t_table *philo, int id, int other_hand)
{
	sem_post(philo->fork->sem_forks);
	sem_post(philo->fork->sem_forks);
}
