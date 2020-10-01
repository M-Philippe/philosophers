/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pminne <pminne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/07 17:15:17 by pminne            #+#    #+#             */
/*   Updated: 2020/10/01 17:36:26 by pminne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>
# include <semaphore.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <string.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>

# define FORK 0
# define EATING 1
# define SLEEPING 2
# define THINKING 3

# define MALLOC_WRITE 0
# define MALLOC_FORK 1
# define MALLOC_PHILO 2
# define MALLOC_GBL 3
# define ERROR_SEMAPHORE 4

# define FORKS_NAME "/fork"
# define WRITE_NAME "/write"
# define DEAD_NAME "/dead"
# define DONE_NAME "/done"
# define INIT_NAME "/init"

# define MEAL_LEN 11

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
	sem_t		*sem_write;
}				t_write;

typedef struct	s_fork
{
	sem_t		*sem_forks;
}				t_fork;

typedef struct	s_gbl_var
{
	sem_t		*sem_dead;
	sem_t		*sem_done;
}				t_gbl_var;

typedef struct	s_table
{
	int				id;
	t_write			*write;
	t_gbl_var		*g_mtx;
	pthread_t		th;
	pthread_t		th_meal;
	long			start_program;
	long			time_to_eat;
	long			time_to_sleep;
	long			time_to_starve;
	long			last_meal;
	int				nb_philo;
	int				other_hand;
	int				turns;
	int				stop_meal;
	t_fork			*fork;
	char			sem_name[MEAL_LEN];
	sem_t			*sem_meal;
	sem_t			*sem_init;
}				t_table;

typedef struct	s_mtr
{
	t_table			*head_philo;
	t_args			*args;
	sem_t			*sem_dead;
	sem_t			*sem_init;
	sem_t			*sem_done;
	pid_t			*pid;
	struct s_mtr	*other_mtr;
}				t_mtr;

/*
**		PARSING.C
*/
t_args			*parsing(int ac, char **av);
/*
**		UTILS.C
*/
long			ft_atoi(char *s);
unsigned int	ft_strlen(char *s);
void			ft_putnbr_fd(long n, int fd);
char			*ft_strdup(char *s);
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
int				init_semaphore(t_table **philo, t_args *args,
	t_gbl_var **g_mtx);
void			*allocate_philosophers(t_table **philo, t_args *args);
/*
**		PHILOSOPHIZE.C
*/
void			philosophize(void *arg);
/*
**		FORK.C
*/
void			take_fork(t_table *philo, int id);
void			free_fork(t_table *philo);
/*
**		MEAl_NAME.C
*/
void			meal_name(char *sem_name, int i);
/*
**		ERROR_SEMAPHORE.C
*/
void			unlink_semaphore();
void			*error_allocate(t_table **philo,
	t_write *writing, t_fork *fork, int msg);
int				free_error_semaphore(t_table *philo);
int				error_semaphore(t_table *philo, int nb_philo);
/*
**		MONITOR.C
*/
void			*waiting_done(void *arg);
void			*waiting_death(void *arg);
void			launching_monitors(t_mtr *mtr);
t_mtr			*setup_monitor(t_table *philo, t_args *args, t_gbl_var *g_mtx,
	pid_t *pid);
/*
**		EXIT.c
*/
void			exit_philosophize(t_table *philo);
void			clean_free(t_table *philo, t_args *args);
void			destroy_semaphore(t_table *philo, t_args *args, t_fork *fork);
void			unlink_philo_semaphore(t_table *philo);

#endif
