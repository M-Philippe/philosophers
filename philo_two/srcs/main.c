/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pminne <pminne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/07 17:15:14 by pminne            #+#    #+#             */
/*   Updated: 2020/09/21 13:48:24 by pminne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void		waiting_end_simulation(t_args *args, t_gbl_var *g_mtx)
{
	int		done;

	done = 0;
	sem_post(g_mtx->sem_done);
	while (done < args->nb_philo)
	{
		usleep(1000);
		sem_wait(g_mtx->sem_done);
		done = g_philos_are_done;
		sem_post(g_mtx->sem_done);
	}
	done = 0;
	while (done < args->nb_philo)
	{
		usleep(1000);
		sem_wait(g_mtx->sem_meals);
		done = g_meals_are_done;
		sem_post(g_mtx->sem_meals);
	}
}

void		start_philosophers(t_table *philo, t_args *args)
{
	int			i;
	long		start_program;
	t_gbl_var	*g_mtx;

	i = 0;
	g_mtx = philo[0].g_mtx;
	start_program = timestamp();
	while (i < args->nb_philo)
	{
		philo[i].start_program = start_program;
		pthread_create(&philo[i].th, NULL, philosophize, &philo[i]);
		(i == args->nb_philo - 1) ? (pthread_join(philo[i].th, NULL)) :
			(pthread_detach(philo[i].th));
		usleep(50);
		i++;
	}
	sem_wait(g_mtx->sem_done);
	waiting_end_simulation(args, g_mtx);
	write(1, "End of simulation\n", ft_strlen("End of simulation\n"));
}

void		destroy_semaphore(t_table *philo, t_args *args, t_fork *fork)
{
	int		i;

	i = 0;
	sem_close(philo[0].g_mtx->sem_dead);
	free(philo[0].g_mtx->dead_name);
	sem_close(philo[0].g_mtx->sem_done);
	free(philo[0].g_mtx->done_name);
	sem_close(philo[0].g_mtx->sem_meals);
	free(philo[0].g_mtx->meals_name);
	sem_close(fork->sem_forks);
	free(fork->sem_name);
	while (i < args->nb_philo)
	{
		sem_close(philo[i].sem_meal);
		i++;
	}
	sem_close(philo[0].write->sem_write);
	free(philo[0].write->sem_name);
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
	destroy_semaphore(philo, args, philo[0].fork);
	free(args);
	free(philo[0].fork);
	free(philo[0].g_mtx);
	free(philo[0].write);
	free(philo);
	return (0);
}
