/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_philosophers2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pminne <pminne@42lyon.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/07 20:04:49 by pminne            #+#    #+#             */
/*   Updated: 2020/09/08 00:05:01 by pminne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

char			*copy_name(char *s1, char *s2)
{
	int		i;
	int		w;
	char	*ret;

	i = 0;
	w = 0;
	ret = NULL;
	// PROTECT
	ret = malloc(sizeof(ft_strlen(s1) + ft_strlen(s2) + 1));
	while (s1[i])
	{
		ret[i] = s1[i];
		i++;
	}
	while (s2[w])
	{
		ret[i] = s2[w];
		w++;
		i++;
	}
	ret[i] = '\0';
	return (ret);
}

t_info			*set_meal(long start_program, long time_to_starve, int count)
{
	t_info	*meal;

	meal = NULL;
	if (!(meal = malloc(sizeof(t_info))))
		return (NULL);
	meal->sem_name = copy_name("/meal ", ft_itoa(count));
	meal->meal_sem = sem_open(meal->sem_name, O_CREAT, 777, 1);
	if (meal->meal_sem == SEM_FAILED)
		exit(0);
	meal->last_meal = 0;
	meal->time_meal = 0;
	meal->start_program = start_program;
	meal->time_to_starve = time_to_starve;
	return (meal);
}

void			copy_args(t_table *philo, t_args *args,
	int count, long start_program)
{
	philo->id = count;
	philo->nb_philo = args->nb_philo;
	philo->time_to_eat = args->time_to_eat;
	philo->time_to_sleep = args->time_to_sleep;
	philo->time_to_starve = args->time_to_starve;
	philo->turns = args->n_time_must_eat;
	if (philo->turns == 0)
		philo->turns = -1;
	philo->start_program = start_program;
}

void			swap1(t_table *tmp, t_table *philo)
{
	if (tmp)
	{
		tmp->next = philo;
		philo->prev = tmp;
	}
}
// Use this function if block
int				set_fork(t_table *philo, t_write *write)
{
	philo->write = write;
	// PUT SEM
	return (0);
}
