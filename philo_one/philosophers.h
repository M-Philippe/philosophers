#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#define MAX_USEC 999999

#define FORK 0
#define EATING 1
#define SLEEPING 2
#define THINKING 3
#define FREE 0
#define TAKEN 1

long time_eat = 200;
long time_sleep = 200;
long time_think = 200;
long time_to_starve = 800;

typedef struct  s_args
{
    int     nb_philo;
    int     time_to_die;
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
    pthread_mutex_t mtx;
}               t_fork;

typedef struct s_table
{
    int         id;
    t_fork      *r_fork;
    pthread_t   th;
    struct s_table     *prev;
    struct s_table     *next;
    int     time_eat;
    int     time_sleep;
    int     time_think;
    int     time_to_starve;
    t_time  *time;
}               t_table;

# endif