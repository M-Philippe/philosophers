/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pminne <pminne@42lyon.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/07 17:15:07 by pminne            #+#    #+#             */
/*   Updated: 2020/09/11 02:41:19 by pminne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int		someone_is_dead(t_info *meal, int id, int t_stamp)
{
	pthread_mutex_lock(&meal->monitor->mtx);
	if (meal->monitor->someone_died == 1)
	{
		pthread_mutex_unlock(&meal->monitor->mtx);
		return (1);
	}
	pthread_mutex_unlock(&meal->monitor->mtx);
	pthread_mutex_lock(&meal->monitor->mtx);
	meal->monitor->someone_died = 1;
	pthread_mutex_lock(&meal->write->writing);
	print_death(id, t_stamp);
	pthread_mutex_unlock(&meal->write->writing);
	pthread_mutex_unlock(&meal->monitor->mtx);
	return (0);
}

int		is_someone_dead(t_table *philo)
{
	int		ret;
	
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

void	*monitoring(void *arg)
{
	t_monitor		*monitor;
	int				nb_philo;
	int				ret;

	monitor = arg;
	pthread_mutex_lock(&monitor->mtx);
	nb_philo = monitor->nb_philo;
	pthread_mutex_unlock(&monitor->mtx);
	ret = 0;
	while (ret != nb_philo)
	{
		ret = philosophers_done(monitor, ASK);
		usleep(100);
	}
	free_all(monitor->head);
	return (NULL);
}
