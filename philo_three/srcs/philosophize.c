/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophize.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pminne <pminne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/19 13:46:21 by pminne            #+#    #+#             */
/*   Updated: 2020/09/25 20:22:00 by pminne           ###   ########lyon.fr   */
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
	take_fork(philo, philo->id);
	sem_wait(philo->sem_meal);
	philo->last_meal = timestamp();
	sem_post(philo->sem_meal);
	print_state(philo, philo->id, EATING);
	waiting(philo->time_to_eat, timestamp());
	free_fork(philo);
	*count += 1;
	return (0);
}

int			checking_turns(t_table *philo, int count)
{
	if (count == philo->turns)
	{
		philo->stop_meal = 1;
		sem_post(philo->sem_meal);
		return (1);
	}
	sem_post(philo->sem_meal);
	return (0);
}

void		re_open_semaphore(t_table *philo)
{
	if (philo->id == philo->nb_philo - 1)
	{
		if ((philo->sem_init = sem_open(INIT_NAME, 0)) == SEM_FAILED)
			exit(0);
		sem_wait(philo->sem_init);
	}
	if ((philo->g_mtx->sem_done = sem_open(DONE_NAME, 0)) == SEM_FAILED)
		exit(0);
	if ((philo->g_mtx->sem_dead = sem_open(DEAD_NAME, 0)) == SEM_FAILED)
		exit(0);
	if ((philo->fork->sem_forks = sem_open(FORKS_NAME, 0)) == SEM_FAILED)
		exit(0);
	if ((philo->write->sem_write = sem_open(WRITE_NAME, 0)) == SEM_FAILED)
		exit(0);
	if ((philo->sem_meal = sem_open(philo->sem_name, 0)) == SEM_FAILED)
		exit(0);
	sem_unlink(philo->sem_name);
}

void		philosophize(void *arg)
{
	t_table		*philo;
	int			count;

	philo = arg;
	re_open_semaphore(philo);
	count = 0;
	philo->stop_meal = 0;
	sem_wait(philo->g_mtx->sem_dead);
	sem_wait(philo->g_mtx->sem_done);
	if (philo->id == philo->nb_philo - 1)
		sem_post(philo->sem_init);
	setup_hand_and_meal(philo);
	while (1)
	{
		if (eat(philo, &count))
			break ;
		sem_wait(philo->sem_meal);
		if (checking_turns(philo, count))
			break ;
		print_state(philo, philo->id, SLEEPING);
		waiting(philo->time_to_sleep, timestamp());
		print_state(philo, philo->id, THINKING);
	}
	sem_wait(philo->sem_meal);
	exit_philosophize(philo);
}
