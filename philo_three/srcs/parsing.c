/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pminne <pminne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/07 17:15:11 by pminne            #+#    #+#             */
/*   Updated: 2020/09/29 19:43:12 by pminne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void		*perror_parsing(char *msg_error, t_args *args)
{
	write(1, msg_error, ft_strlen(msg_error));
	if (args)
		free(args);
	return (NULL);
}

static t_args	*fifth_args(char *av, t_args *args)
{
	args->n_time_must_eat = ft_atoi(av);
	if (args->n_time_must_eat == -1)
		return (perror_parsing("Time_eat must be positive (zero out)\n", args));
	return (args);
}

t_args			*parsing(int ac, char **av)
{
	t_args	*args;

	args = NULL;
	if (ac < 5 || ac > 6)
		return (perror_parsing("Should have only 4 or 5 arguments\n", args));
	if (!(args = malloc(sizeof(t_args))))
		return (perror_parsing("Error malloc args\n", NULL));
	args->nb_philo = ft_atoi(av[1]);
	if (args->nb_philo == -1 || args->nb_philo == 1 || args->nb_philo > 200)
		return (perror_parsing("Nb_philo must be positive ([2:200])\n", args));
	args->time_to_starve = ft_atoi(av[2]);
	if (args->time_to_starve == -1)
		return (perror_parsing("Time_starve -> positive (zero out)\n", args));
	args->time_to_eat = ft_atoi(av[3]);
	if (args->time_to_eat == -1)
		return (perror_parsing("Time_to_eat not positive (zero out)\n", args));
	args->time_to_sleep = ft_atoi(av[4]);
	if (args->time_to_sleep == -1)
		return (perror_parsing("Time_sleep not positive (zero out)\n", args));
	if (av[5])
		return (fifth_args(av[5], args));
	else
		args->n_time_must_eat = 0;
	return (args);
}
