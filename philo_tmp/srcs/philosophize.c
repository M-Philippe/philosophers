/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophize.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pminne <pminne@42lyon.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/08 00:04:51 by pminne            #+#    #+#             */
/*   Updated: 2020/09/11 02:52:34 by pminne           ###   ########lyon.fr   */
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
	/*pthread_mutex_lock(&philo->meal->mtx);
	philo->meal->time_meal = (timestamp() - philo->meal->start_program)
		- philo->meal->last_meal;
	if (philo->meal->time_meal > philo->meal->time_to_starve)
	{
		pthread_mutex_unlock(&philo->meal->mtx);
		return (1);
	}
	pthread_mutex_unlock(&philo->meal->mtx);*/
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

void		*calculate_meal(void *arg)
{
	t_info	*meal;
	int		dead;

	meal = arg;
	dead = 0;
	while (dead == 0)
	{	
		pthread_mutex_lock(&meal->mtx);
		meal->time_meal = (timestamp() - meal->start_program) - meal->last_meal;
		if (meal->time_meal >= meal->time_to_starve)
		{
			dead = 1;
			someone_is_dead(meal, meal->id, timestamp() - meal->start_program);
		}
		if (meal->is_done == 1)
		{
			pthread_mutex_unlock(&meal->mtx);
			dead = 1;
			break ;
		}
		pthread_mutex_unlock(&meal->mtx);
	}
	return (NULL);
}

void		*philosophize(void *arg)
{
	t_table		*philo;
	int			count;

	philo = arg;
	pthread_mutex_lock(&philo->meal->mtx);
	philo->meal->time_meal = 0;
	philo->meal->last_meal = philo->start_program;
	philo->meal->id = philo->id;
	philo->meal->monitor = philo->monitor;
	philo->meal->write = philo->write;
	philo->meal->is_done = 0;
	pthread_mutex_unlock(&philo->meal->mtx);
	pthread_create(&philo->th_meal, NULL, calculate_meal, philo->meal);
	pthread_detach(philo->th_meal);
	count = 0;
	while (1)
	{
		if (checking_death_before_meal(philo))
			break ;
		take_fork(philo);
		pthread_mutex_lock(&philo->meal->mtx);
		philo->meal->last_meal = timestamp() - philo->start_program;
		philo->meal->time_meal = 0;
		pthread_mutex_unlock(&philo->meal->mtx);
		action(philo, philo->time_to_eat, EATING);
		free_fork(philo);
		if (checking_death_after_meal(philo, &count))
			break ;
	}
	philosophers_done(philo->monitor, SET);
	return (NULL);
}
