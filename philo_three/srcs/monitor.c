/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pminne <pminne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/25 20:18:32 by pminne            #+#    #+#             */
/*   Updated: 2020/09/25 20:20:28 by pminne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void		*waiting_done(void *arg)
{
	int		i;
	t_mtr	*mtr;
	int		nb_philo;

	i = 0;
	mtr = arg;
	nb_philo = mtr->args->nb_philo;
	while (i < nb_philo)
	{
		sem_wait(mtr->sem_done);
		i++;
	}
	clean_free(mtr->head_philo, mtr->args);
	free(mtr->pid);
	free(mtr->other_mtr);
	write(1, "End of simulation\n", ft_strlen("End of simulation\n"));
	exit(0);
}

void		*waiting_death(void *arg)
{
	t_mtr	*mtr;
	int		i;
	int		nb_philo;

	mtr = arg;
	nb_philo = mtr->args->nb_philo;
	i = 0;
	sem_wait(mtr->sem_dead);
	while (i < nb_philo)
	{
		kill(mtr->pid[i], SIGINT);
		i++;
	}
	clean_free(mtr->head_philo, mtr->args);
	free(mtr->pid);
	free(mtr);
	write(1, "Someone is dead\n", ft_strlen("Someone is dead\n"));
	exit(0);
}

/*
**			0 -> Monitor death
**			1 -> Monitor done
*/

void		launching_monitors(t_mtr *mtr)
{
	pthread_t		th[2];
	t_mtr			mtr_bis;

	usleep(100000);
	sem_wait(mtr->sem_init);
	sem_unlink(INIT_NAME);
	sem_close(mtr->sem_init);
	unlink_semaphore();
	mtr_bis.args = mtr->args;
	mtr_bis.head_philo = mtr->head_philo;
	mtr_bis.sem_done = mtr->sem_done;
	mtr_bis.pid = mtr->pid;
	mtr_bis.other_mtr = mtr;
	pthread_create(&th[0], NULL, waiting_death, mtr);
	pthread_create(&th[1], NULL, waiting_done, &mtr_bis);
	pthread_detach(th[0]);
	usleep(50);
	pthread_join(th[1], NULL);
}

t_mtr		*setup_monitor(t_table *philo, t_args *args, t_gbl_var *g_mtx,
	pid_t *pid)
{
	t_mtr	*mtr;

	if (!(mtr = malloc(sizeof(*mtr))))
	{
		free(mtr);
		return (NULL);
	}
	g_mtx = philo[0].g_mtx;
	mtr->head_philo = &philo[0];
	mtr->args = args;
	mtr->sem_dead = philo->g_mtx->sem_dead;
	mtr->sem_done = philo->g_mtx->sem_done;
	mtr->pid = pid;
	if ((mtr->sem_init = sem_open(INIT_NAME, O_CREAT, S_IRUSR | S_IWUSR, 1)) ==
		SEM_FAILED)
	{
		free(mtr);
		return (NULL);
	}
	return (mtr);
}
