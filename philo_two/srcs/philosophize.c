/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophize.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pminne <pminne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/19 13:46:21 by pminne            #+#    #+#             */
/*   Updated: 2020/09/20 22:48:42 by pminne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void		setup_hand_and_meal(t_table *philo)
{
	philo->stop_meal = 0;
	(philo->id == 0) ? (philo->other_hand = philo->nb_philo - 1) :
		(philo->other_hand = philo->id - 1);
	philo->last_meal = philo->start_program;
	pthread_create(&philo->th_meal, NULL, philo_meal, philo);
	pthread_detach(philo->th_meal);
}

int			eat(t_table *philo, int *count)
{
	take_fork(philo, philo->id, philo->other_hand);
	//pthread_mutex_lock(&philo->meal);
	sem_wait(philo->sem_meal);
	philo->last_meal = timestamp();
	//pthread_mutex_unlock(&philo->meal);
	sem_post(philo->sem_meal);
	print_state(philo, philo->id, EATING);
	waiting(philo->time_to_eat, timestamp());
	free_fork(philo, philo->id, philo->other_hand);
	*count += 1;
	//pthread_mutex_lock(&philo->g_mtx->g_dead);
	sem_wait(philo->g_mtx->sem_dead);
	if (g_someone_is_dead == 1)
	{
		//pthread_mutex_unlock(&philo->g_mtx->g_dead);
		sem_post(philo->g_mtx->sem_dead);
		return (1);
	}
	//pthread_mutex_unlock(&philo->g_mtx->g_dead);
	sem_post(philo->g_mtx->sem_dead);
	return (0);
}

int			checking_turns(t_table *philo, int count)
{
	if (count == philo->turns)
	{
		philo->stop_meal = 1;
		//pthread_mutex_unlock(&philo->meal);
		sem_post(philo->sem_meal);
		return (1);
	}
	//pthread_mutex_unlock(&philo->meal);
	sem_post(philo->sem_meal);
	return (0);
}

void		*philosophize(void *arg)
{
	t_table		*philo;
	int			count;

	philo = arg;
	count = 0;
	philo->stop_meal = 0;
	setup_hand_and_meal(philo);
	while (1)
	{
		if (eat(philo, &count))
			break ;
		//pthread_mutex_lock(&philo->meal);
		sem_wait(philo->sem_meal);
		if (checking_turns(philo, count))
			break ;
		print_state(philo, philo->id, SLEEPING);
		waiting(philo->time_to_sleep, timestamp());
		print_state(philo, philo->id, THINKING);
	}
	//pthread_mutex_lock(&philo->g_mtx->g_done);
	sem_wait(philo->g_mtx->sem_done);
	g_philos_are_done++;
	//pthread_mutex_unlock(&philo->g_mtx->g_done);
	sem_post(philo->g_mtx->sem_done);
	return (NULL);
}
