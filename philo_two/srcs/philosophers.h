/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pminne <pminne@42lyon.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/07 17:15:17 by pminne            #+#    #+#             */
/*   Updated: 2020/09/08 00:04:47 by pminne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <semaphore.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <errno.h>
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
	//			SEM
	char		*sem_name;
	sem_t		*write_sem;
}				t_write;

typedef struct	s_fork
{
	//			SEM
	char		*sem_nam;
	sem_t		*cnt_sem;
}				t_fork;

typedef struct	s_info
{
	long			last_meal;
	long			time_meal;
	long			time_to_starve;
	long			start_program;
	//				SEM
	char			*sem_name;
	sem_t			*meal_sem;
}				t_info;

typedef struct	s_monitor
{
	int				someone_died;
	int				done;
	int				nb_philo;
	struct s_table	*head;
	//				SEM
	char			*sem_name;
	sem_t			*mtr_sem;
}				t_monitor;

typedef struct	s_table
{
	int				id;
	//t_fork			*fork;
	// TEST SEM OUTSIDE STRUCT
	sem_t			*forks;
	t_write			*write;
	t_info			*meal;
	pthread_t		th;
	struct s_table	*prev;
	struct s_table	*next;
	t_monitor		*monitor;
	long			start_program;
	int				time_to_eat;
	int				time_to_sleep;
	int				time_to_starve;
	long			time_meal;
	long			last_meal;
	int				nb_philo;
	int				turns;
	int				use_hand;
}				t_table;

void			take_fork(t_table *philo);
void			free_fork(t_table *philo);
t_args			*parsing(int ac, char **av);
t_table			*set_philosophers(t_args *args, t_monitor *mtr);
int				philosophers_done(t_monitor *mtr, int flag);
void			ft_putnbr_fd(long n, int fd);
/*
**	SET_PHILOSOPHERS2.C
*/
long			set_time(void);
t_info			*set_meal(long start_program, long time_to_starve, int count);
void			copy_args(t_table *philo, t_args *args,
	int count, long start_program);
void			swap1(t_table *tmp, t_table *philo);
int				set_fork(t_table *philo, t_write *write);
/*
**	X
*/
int				is_someone_dead(t_table *philo);
int				is_dead(t_table *philo);
/*
**	MONITOR.C
*/
void			*monitoring(void *arg);
int				is_dead(t_table *philo);
int				philosophers_done(t_monitor *mtr, int flag);
int				is_someone_dead(t_table *philo);
/*
**	PRINT.C
*/
void			print_state(t_table *philo, int id, int state);
void			print_death(int id, long t_stamp);
/*
**	X
*/
long			timestamp(void);
void			action(t_table *philo, long time, int state);
void			free_all(t_table *philo);
unsigned int	ft_strlen(char *s);
void			*philosophize(void *arg);
int				ft_atoi(char *s);
void			*free_table(t_table *philo, t_write *write);

char			*ft_itoa(int n);

#endif
