#include "philosophers.h"

long	timestamp(void)
{
	long		ret;
	struct timeval	tv;

	ret = 0;
	gettimeofday(&tv, NULL);
	ret = (tv.tv_sec * 1000 + tv.tv_usec / 1000);
	return (ret);
}

void	action(t_table *philo, long time,  int state)
{
	print_state(philo, philo->id, state);
	usleep(time * 1000);
}

void	*philosophize(void *arg)
{
	t_table *philo;
	int	count;

	philo = arg;
	pthread_mutex_lock(&philo->meal->mtx);
	philo->meal->time_meal = 0;
	philo->meal->last_meal = philo->start_program;
	pthread_mutex_unlock(&philo->meal->mtx);
	count = 0;
	while (1)
	{
		if (is_someone_dead(philo) == 1)
			break ;
		pthread_mutex_lock(&philo->meal->mtx);
		philo->meal->time_meal = (timestamp() - philo->meal->start_program) - philo->meal->last_meal;
		if (philo->meal->time_meal > philo->meal->time_to_starve)
		{
			pthread_mutex_unlock(&philo->meal->mtx);
			break ;
		}
		pthread_mutex_unlock(&philo->meal->mtx);
		take_fork(philo);
		pthread_mutex_lock(&philo->meal->mtx);
		philo->meal->last_meal = timestamp() - philo->start_program;
		philo->meal->time_meal = 0;
		pthread_mutex_unlock(&philo->meal->mtx);
		action(philo, philo->time_to_eat, EATING);
		free_fork(philo);
		count++;
		if (count == philo->turns)
			break ;
		if (is_someone_dead(philo) == 1)
			break ;
		action(philo, philo->time_to_sleep, SLEEPING);
		if (is_someone_dead(philo) == 1)
			break ;
		print_state(philo, philo->id, THINKING);
	}
	philosophers_done(philo->monitor, SET);
	return (NULL);
}

void	free_all(t_table *philo)
{
	t_table	*tmp;

	tmp = NULL;
	pthread_mutex_destroy(&philo->write->writing);
	free(philo->write);
	pthread_mutex_destroy(&philo->time->time_lock);
	free(philo->time);
	philo->prev->next = NULL;
	while (philo)
	{
		tmp = philo;
		philo = philo->next;
		pthread_mutex_destroy(&tmp->r_fork->fork);
		pthread_mutex_destroy(&tmp->meal->mtx);
		free(tmp->meal);
		free(tmp->r_fork);
		free(tmp);
	}
}

void	start_philosophers(t_args *args, t_table *philo)
{
	int i;

	i = 0;
	while (i < args->nb_philo)
	{
		if (pthread_create(&philo->th, NULL, philosophize, philo) != 0)
			printf("Error Thread\n");
		pthread_detach(philo->th);
		philo = philo->next;
		i++;
	}
}

void	start_monitoring(t_monitor *monitor)
{
	pthread_t	mtr;

	pthread_create(&mtr, NULL, monitoring, (void*)monitor);
	pthread_join(mtr, NULL);
}

int main(int ac, char **av)
{
	t_args	*args;
	t_table	*philo;
	t_monitor	*monitor;

	monitor = NULL;
	args = NULL;
	philo = NULL;
	args = parsing(ac, av);
	if (!args)
		return (0);
	if (!(monitor = malloc(sizeof(t_monitor))))
	{
		free(args);
		return (0);
	}
	monitor->someone_died = 0;
	monitor->done = 0;
	pthread_mutex_init(&monitor->mtx, NULL);
	philo = set_philosophers(args, monitor);
	if (!philo)
	{
		free(args);
		free(philo);
		return (0);
	}
	monitor->nb_philo = args->nb_philo;
	monitor->head = philo;
	start_philosophers(args, philo);
	start_monitoring(monitor);
	pthread_mutex_destroy(&monitor->mtx);
	free(monitor);
	free(args);
	return (0);
}
