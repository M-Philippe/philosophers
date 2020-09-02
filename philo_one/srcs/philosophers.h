#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#include <string.h>


#define MAX_USEC 999999

#define FORK 0
#define EATING 1
#define SLEEPING 2
#define THINKING 3
#define FREE 0
#define TAKEN 1
#define LEFT 0
#define RIGHT 1
#define SET 0
#define ASK 1

typedef struct  s_args
{
	int     nb_philo;
	int     time_to_starve;
	int     time_to_eat;
	int     time_to_sleep;
	int     n_time_must_eat;
}               t_args;

typedef struct s_time
{
	long start_program;
	long timestamp;
	struct timeval tv;
	pthread_mutex_t time_lock;
}               t_time;

typedef struct s_fork
{
	int     state;
	int     id_fork;
	pthread_mutex_t mtx;
}               t_fork;

typedef struct s_info
{
	int     done;
	int     someone_died;
	pthread_mutex_t mtx;
}               t_info;

typedef struct s_table
{
	int         id;
	t_fork      *r_fork;
	pthread_t   th;
	struct s_table     *prev;
	struct s_table     *next;
	t_info  *monitor;
	int     time_to_eat;
	int     time_to_sleep;
	int     time_to_starve;
	long    time_meal;
	long    last_meal;
	int     nb_philo;
	int     turns;
	int     use_hand;
	t_time  *time;
}               t_table;

void banker_algorithm(t_table *philo);
void        take_fork(t_table *philo);
void free_fork(t_table *philo);
long    actualize_timestamp(t_table *philo);
void    print_state(t_table *philo, int state, int fork_id);
void    quit_program(t_table *philo);
char			*ft_itoa(int n);
void    assign_fork(t_table *philo, t_fork **first, t_fork **second);
t_args    *parsing(int ac, char **av);

int     is_philo_dead(t_table *philo, int flag);
int     is_philo_done(t_info *monitor, int flag);

# endif
