/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pminne <pminne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/19 13:37:39 by pminne            #+#    #+#             */
/*   Updated: 2020/09/19 13:46:02 by pminne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void		copy_args(t_table *philo, t_args *args,
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

void		*error_allocate(t_table **philo,
	t_write *writing, t_fork *fork, int msg)
{
	(writing) ? (free(writing)) : 0;
	(fork) ? (free(fork)) : 0;
	(*philo) ? (free(*philo)) : 0;
	if (msg == MALLOC_WRITE)
		write(1, "Error in write malloc\n",
			ft_strlen("Error in write malloc\n"));
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

int			init_mutex(t_table **philo, t_args *args,
	t_fork *fork, t_gbl_var **g_mtx)
{
	int			i;
	int			ret;

	i = 0;
	ret = 0;
	if (!((*g_mtx) = malloc(sizeof(t_gbl_var))))
	{
		free(*g_mtx);
		return (1);
	}
	ret += pthread_mutex_init(&(*philo)[0].write->writing, NULL);
	ret += pthread_mutex_init(&(*g_mtx)->g_dead, NULL);
	ret += pthread_mutex_init(&(*g_mtx)->g_done, NULL);
	ret += pthread_mutex_init(&(*g_mtx)->g_meals, NULL);
	while (i < args->nb_philo)
	{
		(*philo)[i].fork = fork;
		ret += pthread_mutex_init(&fork[i].fork, NULL);
		copy_args(&(*philo)[i], args, i);
		(*philo)[i].write = (*philo)[0].write;
		ret += pthread_mutex_init(&(*philo)[i].meal, NULL);
		(*philo)[i].g_mtx = (*g_mtx);
		i++;
	}
	return (ret);
}

void		*allocate_philosophers(t_table **philo, t_args *args)
{
	t_write		*write;
	t_fork		*fork;
	t_gbl_var	*g_mtx;

	write = NULL;
	fork = NULL;
	g_mtx = NULL;
	if (!(write = malloc(sizeof(*write))))
		return (error_allocate(philo, write, fork, MALLOC_WRITE));
	if (!(*philo = malloc(sizeof(**philo) * args->nb_philo)))
		return (error_allocate(philo, write, fork, MALLOC_PHILO));
	if (!(fork = malloc(sizeof(*fork) * args->nb_philo)))
		return (error_allocate(philo, write, fork, MALLOC_FORK));
	(*philo)[0].write = write;
	(*philo)[0].fork = fork;
	(*philo)[0].fork = fork;
	if (init_mutex(philo, args, fork, &g_mtx))
		return (error_allocate(philo, write, fork, ERROR_MUTEX));
	return (*philo);
}
