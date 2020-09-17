/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pminne <pminne@42lyon.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/07 17:15:14 by pminne            #+#    #+#             */
/*   Updated: 2020/09/16 14:30:03 by pminne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long		timestamp(void)
{
	struct timeval		tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void			copy_args(t_table *philo, t_args *args,
	int count)
{
	philo->id = count;
	philo->nb_philo = args->nb_philo;
	philo->time_to_eat = args->time_to_eat;
	philo->time_to_sleep = args->time_to_sleep;
	philo->time_to_starve = args->time_to_starve;
	philo->turns = args->n_time_must_eat;
	if (philo->turns == 0)
		philo->turns = -1;
}

void		*allocate_philosophers(t_table **philo, t_args *args)
{
	t_write		*write;
	t_fork		*fork;
	t_gbl_var	*g_mtx;
	int			i;

	i = 0;
	if (!(write = malloc(sizeof(*write))))
		return (NULL);
	pthread_mutex_init(&write->writing, NULL);
	if (!(*philo = malloc(sizeof(**philo) * args->nb_philo)))
		return (NULL);
	if (!(fork = malloc(sizeof(*fork) * args->nb_philo)))
		printf("Error malloc\n");
	if (!(g_mtx = malloc(sizeof(t_gbl_var))))
		printf("Error malloc\n");
	pthread_mutex_init(&g_mtx->g_dead, NULL);
	pthread_mutex_init(&g_mtx->g_done, NULL);
	for (int i = 0; i < args->nb_philo; i++)
	{
		pthread_mutex_init(&fork[i].fork, NULL);
		fork[i].id = i;
	}
	while (i < args->nb_philo)
	{
		copy_args(&(*philo)[i], args, i);
		(*philo)[i].write = write;
		//pthread_mutex_init(&(*philo)[i].r_fork.fork, NULL);
		//(*philo)[i].r_fork.id = i;
		pthread_mutex_init(&(*philo)[i].meal, NULL);
		(*philo)[i].fork = fork;
		(*philo)[i].g_mtx = g_mtx;
		i++;
	}
	printf("Allocate Done\n");
	return (*philo);
}

void		take_fork(t_table *philo, int id, int other_hand)
{
	if (philo->id == philo->nb_philo - 1)
	{
		pthread_mutex_lock(&philo->fork[id].fork);
		print_state(philo, id, FORK, philo->fork[id].id);
		pthread_mutex_lock(&philo->fork[other_hand].fork);
		print_state(philo, id, FORK, philo->fork[other_hand].id);
	}
	else
	{
		pthread_mutex_lock(&philo->fork[other_hand].fork);
		print_state(philo, id, FORK, philo->fork[other_hand].id);
		pthread_mutex_lock(&philo->fork[id].fork);
		print_state(philo, id, FORK, philo->fork[id].id);
	}
}

void		free_fork(t_table *philo, int id, int other_hand)
{
	if (philo->id == philo->nb_philo -1)
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

void		*philo_meal(void *arg)
{
	t_table *philo;
	long	t_stamp;

	philo = arg;
	while (1)
	{
		pthread_mutex_lock(&philo->meal);
		t_stamp = timestamp();
		if (t_stamp - philo->last_meal > philo->time_to_starve)
		{
			pthread_mutex_lock(&philo->g_mtx->g_dead);
			if (g_someone_is_dead == 1)
			{
				pthread_mutex_unlock(&philo->g_mtx->g_dead);
				pthread_mutex_unlock(&philo->meal);
				return (NULL);
			}
			g_someone_is_dead = 1;
			pthread_mutex_unlock(&philo->g_mtx->g_dead);
			print_death(philo, t_stamp - philo->start_program);
			pthread_mutex_unlock(&philo->meal);
			return (NULL);
		}
		pthread_mutex_unlock(&philo->meal);
		usleep(1000);
	}
}

void ft_usleep(long time, long timestamp)
{
	struct timeval tv;

	while (1)
	{
		gettimeofday(&tv, NULL);
		if ((tv.tv_sec * 1000 + tv.tv_usec /1000) - timestamp >= time)
			break ;
	}
}

void		*philosophize(void *arg)
{
	t_table		*philo;
	int			count;

	philo = arg;
	count = 0;
	if (philo->id == 0)
		philo->other_hand = philo->nb_philo - 1;
	else
		philo->other_hand = philo->id - 1;
	philo->last_meal = philo->start_program;
	pthread_create(&philo->th_meal, NULL, philo_meal, philo);
	pthread_detach(philo->th_meal);
	while (count != philo->turns)
	{
		take_fork(philo, philo->id, philo->other_hand);
		pthread_mutex_lock(&philo->meal);
		philo->last_meal = timestamp();
		pthread_mutex_unlock(&philo->meal);
		print_state(philo, philo->id, EATING, 0);
		ft_usleep(philo->time_to_eat, timestamp());
		free_fork(philo, philo->id, philo->other_hand);
		print_state(philo, philo->id, SLEEPING, 0);
		ft_usleep(philo->time_to_sleep, timestamp());
		print_state(philo, philo->id, THINKING, 0);
		count++;
		pthread_mutex_lock(&philo->g_mtx->g_dead);
		if (g_someone_is_dead == 1)
			count = philo->turns;
		pthread_mutex_unlock(&philo->g_mtx->g_dead);
	}
	pthread_mutex_lock(&philo->g_mtx->g_done);
	g_philos_are_done++;
	pthread_mutex_unlock(&philo->g_mtx->g_done);
	return (NULL);
}

void		start_philosophers(t_table *philo, t_args *args)
{
	int		i;
	long	start_program;
	int		done;
	t_gbl_var		*g_mtx;

	i = 0;
	done = 0;
	g_mtx = philo[0].g_mtx;
	start_program = timestamp();
	while (i < args->nb_philo)
	{
		philo[i].start_program = start_program;
		pthread_create(&philo[i].th, NULL, philosophize, &philo[i]);
		(i == args->nb_philo - 1) ? (pthread_join(philo[i].th, NULL)) : (pthread_detach(philo[i].th));
		i++;
	}
	while (done < args->nb_philo)
	{
		usleep(100);
		pthread_mutex_lock(&g_mtx->g_done);
		done = g_philos_are_done;
		pthread_mutex_unlock(&g_mtx->g_done);
	}
	printf("Everyone is done\n");
	exit(0);
}

int			main(int ac, char **av)
{
	t_args		*args;
	t_table		*philo;

	args = NULL;
	philo = NULL;
	g_someone_is_dead = 0;
	g_philos_are_done = 0;
	args = parsing(ac, av);
	if (!args)
		return (0);
	allocate_philosophers(&philo, args);
	start_philosophers(philo, args);
	free(args);
	free(philo[0].write);
	free(philo);
	return (0);
}
