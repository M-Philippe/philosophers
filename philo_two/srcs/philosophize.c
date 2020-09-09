/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophize.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pminne <pminne@42lyon.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/08 00:04:51 by pminne            #+#    #+#             */
/*   Updated: 2020/09/08 00:04:51 by pminne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long		timestamp(void)
{
	struct timeval		tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void		action(t_table *philo, long time, int state)
{
	print_state(philo, philo->id, state);
	usleep(time * 1000);
}

int			checking_death_before_meal(t_table *philo)
{
	if (is_someone_dead(philo) == 1)
		return (1);
	// SEM WAIT
	sem_wait(philo->meal->meal_sem);
	philo->meal->time_meal = (timestamp() - philo->meal->start_program)
		- philo->meal->last_meal;
	if (philo->meal->time_meal > philo->meal->time_to_starve)
	{
		sem_post(philo->meal->meal_sem);
		return (1);
	}
	sem_post(philo->meal->meal_sem);
	return (0);
}

int			checking_death_after_meal(t_table *philo, int *count)
{
	*count += 1;
	if (*count == philo->turns)
		return (1);
	if (is_someone_dead(philo) == 1)
		return (1);
	action(philo, philo->time_to_sleep, SLEEPING);
	if (is_someone_dead(philo) == 1)
		return (1);
	print_state(philo, philo->id, THINKING);
	return (0);
}

void		*philosophize(void *arg)
{
	t_table		*philo;
	int			count;

	philo = arg;
	sem_wait(philo->meal->meal_sem);
	philo->meal->time_meal = 0;
	philo->meal->last_meal = philo->start_program;
	sem_post(philo->meal->meal_sem);
	count = 0;
	while (1)
	{
		if (checking_death_before_meal(philo))
			break ;
		take_fork(philo);
		sem_wait(philo->meal->meal_sem);
		philo->meal->last_meal = timestamp() - philo->start_program;
		philo->meal->time_meal = 0;
		sem_post(philo->meal->meal_sem);
		action(philo, philo->time_to_eat, EATING);
		free_fork(philo);
		if (checking_death_after_meal(philo, &count))
			break ;
	}
	philosophers_done(philo->monitor, SET);
	return (NULL);
}
