/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pminne <pminne@42lyon.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/07 17:15:17 by pminne            #+#    #+#             */
/*   Updated: 2020/09/16 13:34:36 by pminne           ###   ########lyon.fr   */
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
# define FREE 0
# define TAKEN 1
# define LEFT 0
# define RIGHT 1
# define SET 0
# define ASK 1

int		g_philo_is_dead;
int		g_philo_are_done;

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
	int				id;
	pthread_mutex_t	fork;
}				t_fork;

typedef struct	s_info
{
	long			last_meal;
	long			time_meal;
	long			time_to_starve;
	long			start_program;
	int				is_done;
	int				id;
	t_write			*write;
	struct s_monitor		*monitor;
	pthread_mutex_t	mtx;
}				t_info;

typedef struct	s_table
{
	int				id;
	t_fork			r_fork;
	t_write			*write;
	//t_info			*meal;
	pthread_t		th;
	pthread_t		th_meal;
	long			start_program;
	int				time_to_eat;
	int				time_to_sleep;
	int				time_to_starve;
	long			last_meal;
	int				nb_philo;
	int				left_hand;
	int				turns;
	pthread_mutex_t	meal;
}				t_table;

t_args			*parsing(int ac, char **av);
int				ft_atoi(char *s);
unsigned int	ft_strlen(char *s);
void			ft_putnbr_fd(long n, int fd);
void	print_state(t_table *philo, int id, int state, int f_id);
long		timestamp(void);
void	print_death(t_table *philo, long t_stamp);

#endif
