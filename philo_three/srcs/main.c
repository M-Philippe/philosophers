/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pminne <pminne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/07 17:15:14 by pminne            #+#    #+#             */
/*   Updated: 2020/09/25 20:22:24 by pminne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void		start_philosophers(t_table *philo, t_args *args)
{
	int			i;
	long		start_program;
	pid_t		*pid;
	t_gbl_var	*g_mtx;
	t_mtr		*mtr;

	i = 0;
	pid = NULL;
	if (!(pid = malloc(sizeof(*pid) * args->nb_philo)))
		return ;
	g_mtx = philo[0].g_mtx;
	if (!(mtr = setup_monitor(&philo[0], args, g_mtx, pid)))
		return (free(pid));
	start_program = timestamp();
	while (i < args->nb_philo)
	{
		philo[i].start_program = start_program;
		pid[i] = fork();
		if (pid[i] == 0)
			philosophize(&philo[i]);
		usleep(50);
		i++;
	}
	launching_monitors(mtr);
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
	write(1, "Error in start philosophers\n",
		ft_strlen("Error in start philosophers\n"));
	unlink_semaphore();
	unlink_philo_semaphore(philo);
	clean_free(philo, args);
	return (0);
}
