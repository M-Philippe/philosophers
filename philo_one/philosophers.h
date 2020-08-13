#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

typedef struct  s_philo
{
    int     n_philo;
    int     time_to_die;
    int     time_to_eat;
    int     time_to_sleep;
    int     n_time_must_eat;
}               t_philo;

# endif