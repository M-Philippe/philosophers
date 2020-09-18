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

void		*error_allocate(t_table **philo, t_write *writing, t_fork *fork, int msg)
{
	(writing) ? (free(writing)) : 0;
	(fork) ? (free(fork)) : 0;
	(*philo) ? (free(*philo)) : 0;
	if (msg == MALLOC_WRITE)
		write(1, "Error in write malloc\n", ft_strlen("Error in write malloc\n"));
	else if (msg == MALLOC_PHILO)
		write(1, "Error in philo fork\n", ft_strlen("Error in philo fork\n"));
	else if (msg == MALLOC_FORK)
		write(1, "Error in fork malloc\n", ft_strlen("Error in fork malloc\n"));
	else if (msg == MALLOC_GBL)
		write(1, "Error in gbl malloc\n", ft_strlen("Error in gbl malloc\n"));
	else if (msg == ERROR_MUTEX)
		write(1, "Error in mutex init\n", ft_strlen("Error in mutex init\n"));
	return (NULL);
}

int			init_mutex(t_table **philo, t_args *args, t_fork *fork)
{
	int			i;
	t_gbl_var	*g_mtx;
	int			ret;

	g_mtx = NULL;
	i = 0;
	ret = 0;
	if (!(g_mtx = malloc(sizeof(t_gbl_var))))
	{
		free(g_mtx);
		return (1);
	}
	ret += pthread_mutex_init(&(*philo)[0].write->writing, NULL);
	ret += pthread_mutex_init(&g_mtx->g_dead, NULL);
	ret += pthread_mutex_init(&g_mtx->g_done, NULL);
	while (i < args->nb_philo)
	{
		(*philo)[i].fork = fork;
		ret += pthread_mutex_init(&fork[i].fork, NULL);
		copy_args(&(*philo)[i], args, i);
		(*philo)[i].write = (*philo)[0].write;
		ret += pthread_mutex_init(&(*philo)[i].meal, NULL);
		(*philo)[i].g_mtx = g_mtx;
		i++;
	}
	return (ret);
}

void		*allocate_philosophers(t_table **philo, t_args *args)
{
	t_write		*write;
	t_fork		*fork;

	write = NULL;
	fork = NULL;
	if (!(write = malloc(sizeof(*write))))
		return (error_allocate(philo, write, fork, MALLOC_WRITE));
	if (!(*philo = malloc(sizeof(**philo) * args->nb_philo)))
		return (error_allocate(philo, write, fork, MALLOC_PHILO));
	if (!(fork = malloc(sizeof(*fork) * args->nb_philo)))
		return (error_allocate(philo, write, fork, MALLOC_FORK));
	(*philo)[0].write = write;
	(*philo)[0].fork = fork;
	(*philo)[0].fork = fork;
	if (init_mutex(philo, args, fork))
		return (error_allocate(philo, write, fork, ERROR_MUTEX));
	return (*philo);
}

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
		if (philo->stop_meal == 1)
		{
			pthread_mutex_unlock(&philo->meal);
			return (NULL);
		}
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
	philo->stop_meal = 0;
	if (philo->id == 0)
		philo->other_hand = philo->nb_philo - 1;
	else
		philo->other_hand = philo->id - 1;
	philo->last_meal = philo->start_program;
	pthread_create(&philo->th_meal, NULL, philo_meal, philo);
	pthread_detach(philo->th_meal);
	while (1)
	{
		take_fork(philo, philo->id, philo->other_hand);
		pthread_mutex_lock(&philo->meal);
		philo->last_meal = timestamp();
		pthread_mutex_unlock(&philo->meal);
		print_state(philo, philo->id, EATING);
		ft_usleep(philo->time_to_eat, timestamp());
		free_fork(philo, philo->id, philo->other_hand);
		count++;
		pthread_mutex_lock(&philo->g_mtx->g_dead);
		if (g_someone_is_dead == 1)
		{
			pthread_mutex_unlock(&philo->g_mtx->g_dead);
			break ;
		}
		pthread_mutex_lock(&philo->meal);
		if (count == philo->turns)
		{
			philo->stop_meal = 1;
			pthread_mutex_unlock(&philo->meal);
			pthread_mutex_unlock(&philo->g_mtx->g_dead);
			break ;
		}
		pthread_mutex_unlock(&philo->meal);
		pthread_mutex_unlock(&philo->g_mtx->g_dead);
		print_state(philo, philo->id, SLEEPING);
		ft_usleep(philo->time_to_sleep, timestamp());
		print_state(philo, philo->id, THINKING);
	}
	pthread_mutex_lock(&philo->g_mtx->g_done);
	g_philos_are_done++;
	pthread_mutex_unlock(&philo->g_mtx->g_done);
	return (NULL);
}

int			start_philosophers(t_table *philo, t_args *args)
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
	usleep(10000);
	write(1, "End of simulation\n", ft_strlen("End of simulation\n"));
	return (0);
}

void		destroy_mutex(t_table *philo, t_args *args, t_fork *fork)
{
	int		i;

	i = 0;
	pthread_mutex_destroy(&philo[0].g_mtx->g_dead);
	pthread_mutex_destroy(&philo[0].g_mtx->g_done);
	while (i < args->nb_philo)
	{
		pthread_mutex_destroy(&fork[i].fork);
		pthread_mutex_destroy(&philo[i].meal);
		i++;
	}
	pthread_mutex_destroy(&philo[0].write->writing);
}

int			main(int ac, char **av)
{
	t_args		*args;
	t_table		*philo;

	args = NULL;
	philo = NULL;
	if (!(args = parsing(ac, av)))
	{
		free(args);
		return (0);
	}
	if (!(allocate_philosophers(&philo, args)))
	{
		free(args);
		return (1);
	}
	start_philosophers(philo, args);
	destroy_mutex(philo, args, philo[0].fork);
	free(args);
	free(philo[0].fork);
	free(philo[0].g_mtx);
	free(philo[0].write);
	free(philo);
	return (0);
}
