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
	int			i;

	i = 0;
	if (!(write = malloc(sizeof(*write))))
		return (NULL);
	pthread_mutex_init(&write->writing, NULL);
	if (!(*philo = malloc(sizeof(**philo) * args->nb_philo)))
		return (NULL);
	while (i < args->nb_philo)
	{
		copy_args(&(*philo)[i], args, i);
		(*philo)[i].write = write;
		pthread_mutex_init(&(*philo)[i].r_fork.fork, NULL);
		(*philo)[i].r_fork.id = i;
		pthread_mutex_init(&(*philo)[i].meal, NULL);
		i++;
	}
	return (*philo);
}

void		take_fork(t_table *philo)
{
	if (philo->id == philo->nb_philo - 1)
	{
		pthread_mutex_lock(&philo->write->writing);
		//printf("%d here\n", philo->id + 1);
		pthread_mutex_unlock(&philo->write->writing);
		pthread_mutex_lock(&(philo[philo->left_hand].r_fork.fork));
		print_state(philo, philo->id, FORK, philo[philo->left_hand].r_fork.id);
		pthread_mutex_lock(&philo->r_fork.fork);
		print_state(philo, philo->id, FORK, philo->r_fork.id);
	}
	else
	{
		pthread_mutex_lock(&philo->r_fork.fork);
		print_state(philo, philo->id, FORK, philo->r_fork.id);
		pthread_mutex_lock(&(philo[philo->left_hand].r_fork.fork));
		print_state(philo, philo->id, FORK, philo[philo->left_hand].r_fork.id);
	}
}

void		free_fork(t_table *philo)
{
	pthread_mutex_unlock(&philo[philo->left_hand].r_fork.fork);
	pthread_mutex_unlock(&philo->r_fork.fork);
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
			print_death(philo, t_stamp - philo->start_program);
			exit (0);
		}
		pthread_mutex_unlock(&philo->meal);
		usleep(100);
	}
}

void		*philosophize(void *arg)
{
	t_table		*philo;
	int			count;

	philo = arg;
	count = 0;
	if (philo->id == 0)
	{
		philo->left_hand = philo->nb_philo - 1;
		pthread_mutex_lock(&philo->write->writing);
		//printf("X %d %d\n", philo->id, philo->left_hand);
		pthread_mutex_unlock(&philo->write->writing);
	}
	else
	{
		philo->left_hand = philo->id - 1;
		pthread_mutex_lock(&philo->write->writing);
		//printf("XXX %d %d\n", philo->id, philo->left_hand);
		pthread_mutex_unlock(&philo->write->writing);
	}
	philo->last_meal = philo->start_program;
	pthread_create(&philo->th_meal, NULL, philo_meal, philo);
	pthread_detach(philo->th_meal);
	while (count < 3)
	{
		take_fork(philo);
		pthread_mutex_lock(&philo->meal);
		philo->last_meal = timestamp();
		pthread_mutex_unlock(&philo->meal);
		print_state(philo, philo->id, EATING, 0);
		usleep(philo->time_to_eat * 1000);
		free_fork(philo);
		print_state(philo, philo->id, SLEEPING, 0);
		usleep(philo->time_to_sleep * 1000);
		print_state(philo, philo->id, THINKING, 0);
		count = 0;
	}
	return (NULL);
}

void		start_philosophers(t_table *philo, t_args *args)
{
	int		i;
	long	start_program;

	i = 0;
	start_program = timestamp();
	while (i < args->nb_philo)
	{
		philo[i].start_program = start_program;
		pthread_create(&philo[i].th, NULL, philosophize, &philo[i]);
		pthread_detach(philo[i].th);
		i++;
	}
	sleep(50);
}

int			main(int ac, char **av)
{
	t_args		*args;
	t_table		*philo;

	args = NULL;
	philo = NULL;
	args = parsing(ac, av);
	if (!args)
		return (0);
	allocate_philosophers(&philo, args);
	/*for (int i = 0; i < args->nb_philo; i++)
	{
		printf("%d\n", philo[i].id);
	}*/
	start_philosophers(philo, args);
	free(args);
	free(philo[0].write);
	free(philo);
	return (0);
}
