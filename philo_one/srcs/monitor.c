/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pminne <pminne@42lyon.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/07 17:15:07 by pminne            #+#    #+#             */
/*   Updated: 2020/09/08 00:04:40 by pminne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int		is_someone_dead(t_table *philo)
{
	int ret;

	ret = 0;
	pthread_mutex_lock(&philo->monitor->mtx);
	ret = philo->monitor->someone_died;
	pthread_mutex_unlock(&philo->monitor->mtx);
	return (ret);
}

int		philosophers_done(t_monitor *mtr, int flag)
{
	int	ret;

	ret = 0;
	if (flag == SET)
	{
		pthread_mutex_lock(&mtr->mtx);
		mtr->done++;
		pthread_mutex_unlock(&mtr->mtx);
	}
	else if (flag == ASK)
	{
		pthread_mutex_lock(&mtr->mtx);
		ret = mtr->done;
		pthread_mutex_unlock(&mtr->mtx);
	}
	return (ret);
}

int		is_dead(t_table *philo)
{
	long	t_stamp;
	int		ret;

	ret = 0;
	pthread_mutex_lock(&philo->meal->mtx);
	pthread_mutex_lock(&philo->monitor->mtx);
	t_stamp = timestamp();
	philo->meal->time_meal = (t_stamp - philo->meal->start_program)
		- philo->meal->last_meal;
	if (philo->meal->time_meal > philo->meal->time_to_starve
		&& philo->monitor->someone_died == 0)
	{
		philo->monitor->someone_died = 1;
		ret = 1;
		print_death(philo->id, t_stamp - philo->meal->start_program);
	}
	pthread_mutex_unlock(&philo->monitor->mtx);
	pthread_mutex_unlock(&philo->meal->mtx);
	return (ret);
}

int		checking_death(t_table *philo, int nb_philo)
{
	int	i;

	i = 0;
	while (i < nb_philo)
	{
		if (is_dead(philo))
			return (1);
		philo = philo->next;
		i++;
	}
	return (0);
}

void	*monitoring(void *arg)
{
	t_monitor		*monitor;
	int				nb_philo;
	int				ret;
	t_table			*philo;
	int				dead;

	monitor = arg;
	philo = monitor->head;
	pthread_mutex_lock(&monitor->mtx);
	nb_philo = monitor->nb_philo;
	pthread_mutex_unlock(&monitor->mtx);
	dead = 0;
	ret = 0;
	while (ret != nb_philo)
	{
		if (dead == 0)
			dead = checking_death(philo, nb_philo);
		ret = philosophers_done(monitor, ASK);
		usleep(100);
	}
	free_all(monitor->head);
	return (NULL);
}
