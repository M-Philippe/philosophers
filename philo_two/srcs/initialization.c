/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pminne <pminne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/19 13:37:39 by pminne            #+#    #+#             */
/*   Updated: 2020/09/21 14:06:52 by pminne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static	int		nblen(int n)
{
	int i;

	i = 0;
	while (n > 0)
	{
		n = n / 10;
		i++;
	}
	return (i);
}

static	char	*neg_itoa(int n)
{
	char	*dst;
	int		i;

	n = n * -1;
	i = nblen(n);
	if (!(dst = malloc(sizeof(char) * (i + 2))))
		return (NULL);
	dst[0] = '-';
	dst[i + 1] = '\0';
	while (i > 0)
	{
		dst[i--] = 48 + (n % 10);
		n = n / 10;
	}
	return (dst);
}

char			*ft_itoa(int n)
{
	char	*dst;
	int		i;

	if (n < 0)
		return (neg_itoa(n));
	i = nblen(n);
	if (n == 0)
	{
		if (!(dst = malloc(sizeof(char) * 2)))
			return (NULL);
		dst[0] = '0';
		dst[1] = '\0';
		return (dst);
	}
	dst = malloc(sizeof(char) * (i + 1));
	if (dst == NULL)
		return (NULL);
	dst[i] = '\0';
	while (n > 0)
	{
		dst[--i] = 48 + (n % 10);
		n = n / 10;
	}
	return (dst);
}

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

char	*ft_strcat(char *dest, const char *src)
{
	int i;
	int j;

	i = 0;
	j = 0;
	while (dest[i] != '\0')
	{
		i++;
	}
	while (src[j] != '\0')
	{
		dest[i] = src[j];
		i++;
		j++;
	}
	dest[i] = '\0';
	return (dest);
}



void		meal_name(char *sem_name, int i)
{
	char *ita;

	ita = NULL;
	memset(sem_name, '\0', MEAL_LEN - 1);
	ft_strcat(sem_name, "/meal ");
	ita = ft_itoa(i);
	ft_strcat(sem_name, ita);
	free(ita);
}

int			init_semaphore(t_table **philo, t_args *args,
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
	(*g_mtx)->dead_name = ft_strdup("/dead");
	(*g_mtx)->done_name = ft_strdup("/done");
	(*g_mtx)->meals_name = ft_strdup("/done");
	(*philo)[0].g_mtx = *g_mtx;
	(*philo)[0].write->sem_write = sem_open((*philo)[0].write->sem_name, O_CREAT, S_IRUSR | S_IWUSR, 1);
	if ((*philo)[0].write->sem_write == SEM_FAILED)
		printf("Error semaphore write\n");
	(*philo)[0].fork->sem_forks = sem_open((*philo)[0].fork->sem_name, O_CREAT, S_IRUSR | S_IWUSR, args->nb_philo);
	if ((*philo)[0].fork->sem_forks == SEM_FAILED)
		printf("Error semaphore forks\n");
	(*philo)[0].g_mtx->sem_dead = sem_open((*philo)[0].g_mtx->dead_name, O_CREAT, S_IRUSR | S_IWUSR, 1);
	if ((*philo)[0].g_mtx->sem_dead == SEM_FAILED)
		printf("Error semaphore global\n");
	(*philo)[0].g_mtx->sem_done = sem_open((*philo)[0].g_mtx->done_name, O_CREAT, S_IRUSR | S_IWUSR, 1);
	if ((*philo)[0].g_mtx->sem_done == SEM_FAILED)
		printf("Error semaphore global\n");
	(*philo)[0].g_mtx->sem_meals = sem_open((*philo)[0].g_mtx->meals_name, O_CREAT, S_IRUSR | S_IWUSR, 1);
	if ((*philo)[0].g_mtx->sem_meals == SEM_FAILED)
		printf("Error semaphore global\n");
	while (i < args->nb_philo)
	{
		(*philo)[i].fork = fork;
		copy_args(&(*philo)[i], args, i);
		(*philo)[i].write = (*philo)[0].write;
		meal_name((*philo)[i].sem_name, i);
		(*philo)[i].sem_meal = sem_open((*philo)[i].sem_name, O_CREAT, S_IRUSR | S_IWUSR, 1);
		sem_unlink((*philo)[i].sem_name);
		(*philo)[i].g_mtx = (*g_mtx);
		i++;
	}
	// UNLINK SEMAPHORE
	sem_unlink((*philo)[0].g_mtx->dead_name);
	sem_unlink((*philo)[0].g_mtx->done_name);
	sem_unlink((*philo)[0].g_mtx->meals_name);
	sem_unlink((*philo)[0].fork->sem_name);
	sem_unlink((*philo)[0].write->sem_name);
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
	if (!(fork = malloc(sizeof(*fork))))
		return (error_allocate(philo, write, fork, MALLOC_FORK));
	(*philo)[0].write = write;
	(*philo)[0].fork = fork;
	(*philo)[0].fork->sem_name = ft_strdup("/forks");
	(*philo)[0].write->sem_name = ft_strdup("/write");
	if (init_semaphore(philo, args, fork, &g_mtx))
		return (error_allocate(philo, write, fork, ERROR_MUTEX));
	return (*philo);
}
