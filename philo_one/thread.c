#include "philosophers.h"

void banker_algorithm(t_table *philo)
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
            fork++;
        pthread_mutex_unlock(&philo->prev->r_fork->mtx);
        pthread_mutex_lock(&philo->r_fork->mtx);
        if (philo->r_fork->state == FREE)
            fork++;
        pthread_mutex_unlock(&philo->r_fork->mtx);
        if (fork != 2)
            fork = 0;
    }
}