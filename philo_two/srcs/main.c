/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pminne <pminne@42lyon.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/07 17:15:14 by pminne            #+#    #+#             */
/*   Updated: 2020/09/08 00:04:37 by pminne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void		free_all(t_table *philo)
{
	t_table	*tmp;

	tmp = NULL;
	//pthread_mutex_destroy(&philo->write->writing);
	free(philo->write);
	philo->prev->next = NULL;
	while (philo)
	{
		tmp = philo;
		philo = philo->next;
		//pthread_mutex_destroy(&tmp->r_fork->fork);
		//pthread_mutex_destroy(&tmp->meal->mtx);
		free(tmp->meal);
		//free(tmp->r_fork);
		free(tmp);
	}
}

void		start_philosophers(t_args *args, t_table *philo)
{
	int i;

	i = 0;
	while (i < args->nb_philo)
	{
		pthread_create(&philo->th, NULL, philosophize, philo);
		pthread_detach(philo->th);
		usleep(50);
		philo = philo->next;
		i++;
	}
}

void		start_monitoring(t_monitor *monitor)
{
	pthread_t	mtr;

	pthread_create(&mtr, NULL, monitoring, (void*)monitor);
	pthread_join(mtr, NULL);
}

int			set_monitor(t_monitor *monitor, t_table *philo, t_args *args)
{
	monitor->someone_died = 0;
	monitor->done = 0;
	monitor->nb_philo = args->nb_philo;
	monitor->head = philo;
	start_philosophers(args, philo);
	start_monitoring(monitor);
	//pthread_mutex_destroy(&monitor->mtx);
	free(monitor);
	free(args);
	return (0);
}

int			main(int ac, char **av)
{
	t_args		*args;
	t_table		*philo;
	t_monitor	*monitor;

	monitor = NULL;
	args = NULL;
	philo = NULL;
	args = parsing(ac, av);
	if (!args)
		return (0);
	if (!(monitor = malloc(sizeof(t_monitor))))
	{
		free(args);
		return (0);
	}
	//	SEM MONITOR
	monitor->mtr_sem = sem_open("/monitor", O_CREAT, 777, 1);
	if (monitor->mtr_sem == SEM_FAILED)
		exit(1);
	philo = set_philosophers(args, monitor);
	if (!philo)
	{
		free(args);
		free(monitor);
		return (0);
	}
	return (set_monitor(monitor, philo, args));
}
