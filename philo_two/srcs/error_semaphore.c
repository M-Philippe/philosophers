/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_semaphore.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pminne <pminne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/21 18:47:43 by pminne            #+#    #+#             */
/*   Updated: 2020/09/21 18:49:05 by pminne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void		unlink_semaphore(t_table *philo)
{
	sem_unlink(philo->g_mtx->dead_name);
	sem_unlink(philo->g_mtx->done_name);
	sem_unlink(philo->g_mtx->meals_name);
	sem_unlink(philo->fork->sem_name);
	sem_unlink(philo->write->sem_name);
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
	else if (msg == ERROR_SEMAPHORE)
		write(1, "Error in sem init\n", ft_strlen("Error in sem init\n"));
	return (NULL);
}

int			free_error_semaphore(t_table *philo)
{
	char		*err_msg;

	err_msg = NULL;
	err_msg = ft_strdup("Error in semaphore initialization\n");
	free(philo->fork->sem_name);
	free(philo->write->sem_name);
	free(philo->g_mtx->dead_name);
	free(philo->g_mtx->done_name);
	free(philo->g_mtx->meals_name);
	free(philo->g_mtx);
	write(1, err_msg, ft_strlen(err_msg));
	free(err_msg);
	return (1);
}

int			error_semaphore(t_table *philo, int nb_philo)
{
	int		i;

	(nb_philo == 0) ? (i = 1) :
		(i = -1);
	sem_unlink(philo->g_mtx->dead_name);
	sem_close(philo->g_mtx->sem_dead);
	sem_unlink(philo->g_mtx->done_name);
	sem_close(philo->g_mtx->sem_done);
	sem_unlink(philo->g_mtx->meals_name);
	sem_close(philo->g_mtx->sem_meals);
	sem_unlink(philo->fork->sem_name);
	sem_close(philo->fork->sem_forks);
	sem_unlink(philo->write->sem_name);
	sem_close(philo->write->sem_write);
	while (i++ < nb_philo)
	{
		sem_unlink(philo[i].sem_name);
		sem_close(philo[i].sem_meal);
	}
	return (free_error_semaphore(philo));
}
