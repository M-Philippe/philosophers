#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>
# include <string.h>

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

typedef	struct		s_args
{
	int		nb_philo;
	int		time_to_starve;
	int		time_to_eat;
	int		time_to_sleep;
	int		n_time_must_eat;
}	t_args;

typedef struct		s_time
{
	long		start_program;
	long		timestamp;
	struct timeval	tv;
	pthread_mutex_t	time_lock;
}			t_time;

typedef struct		s_write
{
	pthread_mutex_t	writing;
}			t_write;

typedef struct		s_fork
{
	pthread_mutex_t	fork;
}			t_fork;

typedef struct		s_info
{
	long		last_meal;
	long		time_meal;
	long		time_to_starve;
	long		start_program;
	pthread_mutex_t	mtx;
}			t_info;

typedef struct		s_monitor
{
	int		someone_died;
	int		done;
	int		nb_philo;
	struct s_table	*head;
	pthread_mutex_t	mtx;
}			t_monitor;

typedef struct		s_table
{
	int		id;
	t_fork		*r_fork;
	t_write		*write;
	t_info		*meal;
	pthread_t	th;
	struct s_table	*prev;
	struct s_table	*next;
	t_monitor	*monitor;
	long		start_program;
	int		time_to_eat;
	int		time_to_sleep;
	int		time_to_starve;
	long		time_meal;
	long		last_meal;
	int		nb_philo;
	int		turns;
	int		use_hand;
	t_time		*time;
}			t_table;

void			take_fork(t_table *philo);
void			free_fork(t_table *philo);
t_args			*parsing(int ac, char **av);
t_time			*set_time(void);
t_table			*set_philosophers(t_args *args, t_monitor *mtr);
int			philosophers_done(t_monitor *mtr, int flag);
void			ft_putnbr_fd(int n, int fd);
/*
**	X
*/
int			is_someone_dead(t_table *philo);
int			is_dead(t_table *philo);
/*
**	MONITOR.C
*/
void			*monitoring(void *arg);
int			is_dead(t_table *philo);
int			philosophers_done(t_monitor *mtr, int flag);
int			is_someone_dead(t_table *philo);
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
unsigned int		ft_strlen(char *s);

#endif
