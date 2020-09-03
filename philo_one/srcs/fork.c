#include "philosophers.h"

void        take_fork(t_table *philo)
{
	if (philo->id == philo->nb_philo)
	{
		pthread_mutex_lock(&philo->r_fork->fork);
		print_state(philo, philo->id, FORK);
		pthread_mutex_lock(&philo->prev->r_fork->fork);
		print_state(philo, philo->id, FORK);
	}
	else
	{
		pthread_mutex_lock(&philo->prev->r_fork->fork);
		print_state(philo, philo->id, FORK);
		pthread_mutex_lock(&philo->r_fork->fork);
		print_state(philo, philo->id, FORK);
	}

}

void free_fork(t_table *philo)
{
	pthread_mutex_unlock(&philo->prev->r_fork->fork);
	pthread_mutex_unlock(&philo->r_fork->fork);
}
