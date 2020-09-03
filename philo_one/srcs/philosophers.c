#include "philosophers.h"

// MONITOR THREAD

long	timestamp(t_table *philo)
{
	long		ret;
	struct timeval	tv;

	ret = 0;
	gettimeofday(&tv, NULL);
	ret = (tv.tv_sec * 1000 + tv.tv_usec / 1000);
	return (ret);
}

void	print_state(t_table *philo, int id, int state)
{
	pthread_mutex_lock(&philo->write->writing);
	ft_putnbr_fd((int)(timestamp(philo) - philo->start_program), 1);
	write(1, " ", 1);
	ft_putnbr_fd(id, 1);
	if (state == FORK)
		write(1, " has taken a fork\n", strlen(" has taken a fork\n"));
	else if (state == EATING)
		write(1, " is eating\n", strlen(" is eating\n"));
	else if (state == SLEEPING)
		write(1, " is sleeping\n", strlen(" is sleeping\n"));
	else if (state == THINKING)
		write(1, " is thinking\n", strlen(" is thinking\n"));
	pthread_mutex_unlock(&philo->write->writing);
}

void	action(t_table *philo, long time,  int state)
{
	long	action_time;
	long	start;

	print_state(philo, philo->id, state);
	start = timestamp(philo) - philo->start_program;
	action_time = 0;
	usleep(time * 1000);
}

void	*philosophize(void *arg)
{
	t_table *philo;

	philo = arg;
	long	time_meal = 0;
	long last_meal = philo->start_program;
	while (1)
	{
		time_meal = (timestamp(philo) - philo->start_program) - last_meal;
		if (time_meal > philo->time_to_starve)
		{
			printf("%ld %d Died\n", timestamp(philo) - philo->start_program,philo->id);
			exit(0);
		}
		take_fork(philo);
		last_meal = timestamp(philo) - philo->start_program;
		action(philo, philo->time_to_eat, EATING);
		free_fork(philo);
		action(philo, philo->time_to_sleep, SLEEPING);
		print_state(philo, philo->id, THINKING);
	}
	return (NULL);
}

int main(int ac, char **av)
{
	t_args	*args;
	t_table	*philo;
	t_monitor	*monitor;

	args = parsing(ac, av);
	philo = set_philosophers(args, &monitor);
	for (int i = 1; i <= args->nb_philo; i++)
	{
		if (pthread_create(&philo->th, NULL, philosophize, philo) != 0)
			printf("Error Thread\n");
		pthread_detach(philo->th);
		philo = philo->next;
	}
	//pthread_t	*monitoring;
	//pthread_create(monitor
	while(1);
	//printf("Everything is Finished\n");
	return (0);
}
