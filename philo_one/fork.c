#include "philosophers.h"

void        take_fork(t_table *philo)
{
    if (philo->id == philo->nb_philo)
    {
        pthread_mutex_lock(&philo->r_fork->mtx);
        print_state(philo, FORK, philo->r_fork->id_fork);
        pthread_mutex_lock(&philo->prev->r_fork->mtx);
        print_state(philo, FORK, philo->prev->r_fork->id_fork);
    }
    else
    {
    	pthread_mutex_lock(&philo->prev->r_fork->mtx);
    	print_state(philo, FORK, philo->prev->r_fork->id_fork);
    	pthread_mutex_lock(&philo->r_fork->mtx);
    	print_state(philo, FORK, philo->r_fork->id_fork);
    }

}

void free_fork(t_table *philo)
{
    pthread_mutex_unlock(&philo->prev->r_fork->mtx);
    pthread_mutex_unlock(&philo->r_fork->mtx);
}
