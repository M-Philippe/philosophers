#include "philosophers.h"

void        left_handed(t_table *philo)
{
    int fork;

    fork = 0;
    while (fork != 2)
    {
        philo->time_meal = actualize_timestamp(philo) / 1000;
        if (philo->time_meal - philo->last_meal > philo->time_to_starve)
            quit_program(philo);
        pthread_mutex_lock(&philo->prev->r_fork->mtx);
        if (philo->prev->r_fork->state == FREE)
        {
            philo->prev->r_fork->state = TAKEN;
            fork++;
            print_state(philo, FORK, philo->prev->r_fork->id_fork);
        }
        pthread_mutex_unlock(&philo->prev->r_fork->mtx);
        pthread_mutex_lock(&philo->r_fork->mtx);
        if (philo->r_fork->state == FREE)
        {
            philo->r_fork->state = TAKEN;
            fork++;
            print_state(philo, FORK, philo->r_fork->id_fork);
        }
        pthread_mutex_unlock(&philo->r_fork->mtx);
    }
}

void        right_handed(t_table *philo)
{
    int fork;

    fork = 0;
    while (fork != 2)
    {
        philo->time_meal = actualize_timestamp(philo) / 1000;
        if (philo->time_meal - philo->last_meal > philo->time_to_starve)
            quit_program(philo);
        pthread_mutex_lock(&philo->r_fork->mtx);
        if (philo->r_fork->state == FREE)
        {
            philo->r_fork->state = TAKEN;
            fork++;
            print_state(philo, FORK, philo->r_fork->id_fork);
        }
        pthread_mutex_unlock(&philo->r_fork->mtx);
        pthread_mutex_lock(&philo->prev->r_fork->mtx);
        if (philo->prev->r_fork->state == FREE)
        {
            philo->prev->r_fork->state = TAKEN;
            fork++;
            print_state(philo, FORK, philo->prev->r_fork->id_fork);
        }
        pthread_mutex_unlock(&philo->prev->r_fork->mtx);
    }
}

void        take_fork(t_table *philo)
{
    int fork;

    fork = 0;
    while (fork == 0)
    {
        philo->time_meal = actualize_timestamp(philo) / 1000;
        if (philo->time_meal - philo->last_meal > philo->time_to_starve)
            quit_program(philo);
        pthread_mutex_lock(&philo->prev->r_fork->mtx);
        pthread_mutex_lock(&philo->r_fork->mtx);
        if (philo->prev->r_fork->state == FREE && philo->r_fork->state == FREE)
        {
            philo->prev->r_fork->state = TAKEN;
            print_state(philo, FORK, philo->prev->r_fork->id_fork);
            philo->r_fork->state = TAKEN;
            print_state(philo, FORK, philo->r_fork->id_fork);
            fork++;
        }
        pthread_mutex_unlock(&philo->prev->r_fork->mtx);
        pthread_mutex_unlock(&philo->r_fork->mtx);
    }
}

void free_fork(t_table *philo)
{
    pthread_mutex_lock(&philo->prev->r_fork->mtx);
    philo->prev->r_fork->state = FREE;
    pthread_mutex_unlock(&philo->prev->r_fork->mtx);
    pthread_mutex_lock(&philo->r_fork->mtx);
    philo->r_fork->state = FREE;
    pthread_mutex_unlock(&philo->r_fork->mtx);
}