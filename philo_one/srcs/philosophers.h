/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pminne <pminne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/07 17:15:17 by pminne            #+#    #+#             */
/*   Updated: 2020/09/19 13:48:34 by pminne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

# define FORK 0
# define EATING 1
# define SLEEPING 2
# define THINKING 3

# define MALLOC_WRITE 0
# define MALLOC_FORK 1
# define MALLOC_PHILO 2
# define MALLOC_GBL 3
# define ERROR_MUTEX 4

int		g_someone_is_dead;
int		g_philos_are_done;
int		g_meals_are_done;

typedef	struct	s_args
{
	int		nb_philo;
	int		time_to_starve;
	int		time_to_eat;
	int		time_to_sleep;
	int		n_time_must_eat;
}				t_args;

typedef struct	s_write
{
	pthread_mutex_t	writing;
}				t_write;

typedef struct	s_fork
{
	pthread_mutex_t	fork;
}				t_fork;

typedef struct	s_gbl_var
{
	pthread_mutex_t		g_dead;
	pthread_mutex_t		g_done;
	pthread_mutex_t		g_meals;
}				t_gbl_var;

typedef struct	s_table
{
	int				id;
	t_write			*write;
	t_gbl_var		*g_mtx;
	pthread_t		th;
	pthread_t		th_meal;
	long			start_program;
	int				time_to_eat;
	int				time_to_sleep;
	int				time_to_starve;
	long			last_meal;
	int				nb_philo;
	int				other_hand;
	int				turns;
	int				stop_meal;
	t_fork			*fork;
	pthread_mutex_t	meal;
}				t_table;

/*
**		PARSING.C
*/
t_args			*parsing(int ac, char **av);
/*
**		UTILS.C
*/
int				ft_atoi(char *s);
unsigned int	ft_strlen(char *s);
void			ft_putnbr_fd(long n, int fd);
/*
**		MEAL.C
*/
void			*philo_meal(void *arg);
/*
**		PRINT.C
*/
void			print_state(t_table *philo, int id, int state);
void			print_death(t_table *philo, long t_stamp);
/*
**		TIME.C
*/
void			waiting(long time, long timestamp);
long			timestamp(void);
/*
**		INITIALIZATION.C
*/
void			copy_args(t_table *philo, t_args *args,
	int count);
void			*error_allocate(t_table **philo,
	t_write *writing, t_fork *fork, int msg);
int				init_mutex(t_table **philo, t_args *args,
	t_fork *fork, t_gbl_var **g_mtx);
void			*allocate_philosophers(t_table **philo, t_args *args);
/*
**		PHILOSOPHIZE.C
*/
void			*philosophize(void *arg);
/*
**		FORK.C
*/
void			take_fork(t_table *philo, int id, int other_hand);
void			free_fork(t_table *philo, int id, int other_hand);

#endif
