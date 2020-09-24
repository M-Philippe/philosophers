/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pminne <pminne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/07 17:15:14 by pminne            #+#    #+#             */
/*   Updated: 2020/09/24 21:58:49 by pminne           ###   ########lyon.fr   */
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

	mtr_bis.args = mtr->args;
	mtr_bis.head_philo = mtr->head_philo;
	mtr_bis.sem_done = mtr->sem_done;
	pthread_create(&th[0], NULL, waiting_death, mtr);
	pthread_create(&th[1], NULL, waiting_done, &mtr_bis);
	pthread_detach(th[0]);
	usleep(50);
	pthread_join(th[1], NULL);
}

void		start_philosophers(t_table *philo, t_args *args)
{
	int			i;
	long		start_program;
	pid_t		*pid;
	t_gbl_var	*g_mtx;
	t_mtr		mtr;

	i = 0;
	// PROTECT
	pid = malloc(sizeof(*pid) * args->nb_philo);
	g_mtx = philo[0].g_mtx;
	start_program = timestamp();
	mtr.head_philo = &philo[0];
	mtr.args = args;
	mtr.sem_dead = philo->g_mtx->sem_dead;
	mtr.sem_done = philo->g_mtx->sem_done;
	mtr.pid = pid;
	// INIT A SEM TO WAIT FOR LAST PROCESS
	mtr.sem_init = sem_open(INIT_NAME, O_CREAT,S_IRUSR | S_IWUSR, 1);
	while (i < args->nb_philo)
	{
		philo[i].start_program = start_program;
		pid[i] = fork();
		if (pid[i] == 0)
			philosophize(&philo[i]);
		usleep(50);
		i++;
	}
	usleep(100000);
	sem_wait(mtr.sem_init);
	sem_unlink(INIT_NAME);
	unlink_semaphore(philo);
	launching_monitors(&mtr);
}

void		destroy_semaphore(t_table *philo, t_args *args, t_fork *fork)
{
	int		i;

	i = 0;
	sem_close(philo[0].g_mtx->sem_dead);
	sem_close(philo[0].g_mtx->sem_done);
	sem_close(fork->sem_forks);
	while (i < args->nb_philo)
	{
		sem_close(philo[i].sem_meal);
		i++;
	}
	sem_close(philo[0].write->sem_write);
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
