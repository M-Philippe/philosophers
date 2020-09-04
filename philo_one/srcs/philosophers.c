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
	int	count;

	philo = arg;
	long	time_meal = 0;
	long last_meal = philo->start_program;
	count = 0;
	while (1)
	{
		time_meal = (timestamp(philo) - philo->start_program) - last_meal;
		if (time_meal > philo->time_to_starve)
		{
			pthread_mutex_lock(&philo->write->writing);
			printf("%ld %d Died\n", timestamp(philo) - philo->start_program,philo->id);
		exit(0);
		}
		take_fork(philo);
		last_meal = timestamp(philo) - philo->start_program;
		action(philo, philo->time_to_eat, EATING);
		count++;
		if (count == philo->turns)
			break ;
		free_fork(philo);
		action(philo, philo->time_to_sleep, SLEEPING);
		print_state(philo, philo->id, THINKING);
	}
	philosophers_done(philo->monitor, SET);
	return (NULL);
}

int	philosophers_done(t_monitor *mtr, int flag)
{
	int	ret;

	ret = 0;
	if (flag == SET)
	{
		pthread_mutex_lock(&mtr->mtx);
		mtr->done++;
		pthread_mutex_unlock(&mtr->mtx);
	}
	else if (flag == ASK)
	{
		pthread_mutex_lock(&mtr->mtx);
		ret = mtr->done;
		pthread_mutex_unlock(&mtr->mtx);
	}
	return (ret);
}

void	*monitoring(void *arg)
{
	t_monitor	*monitor;
	int		nb_philo;

	monitor = arg;
	pthread_mutex_lock(&monitor->mtx);
	nb_philo = monitor->nb_philo;
	pthread_mutex_unlock(&monitor->mtx);
	while (philosophers_done(monitor, ASK) != nb_philo)
	{
		usleep(1 * 100000);
	}
	printf("##############\n");
	return (NULL);
}

int main(int ac, char **av)
{
	t_args	*args;
	t_table	*philo;
	t_monitor	*monitor;

	monitor = NULL;
	monitor = malloc(sizeof(t_monitor));
	monitor->someone_died = 0;
	monitor->done = 0;
	pthread_mutex_init(&monitor->mtx, NULL);
	args = parsing(ac, av);
	philo = set_philosophers(args, monitor);
	/*for (int i = 0; i <= args->nb_philo; i++)
	{
		printf("%d\n", philo->monitor->done);
		philo->monitor->done++;
		philo = philo->next;
	}
	exit(0);*/
	monitor->nb_philo = args->nb_philo;
	for (int i = 1; i <= args->nb_philo; i++)
	{
		if (pthread_create(&philo->th, NULL, philosophize, philo) != 0)
			printf("Error Thread\n");
		pthread_detach(philo->th);
		philo = philo->next;
	}
	//pthread_t	*monitoring;
	//pthread_create(monitor
	//while (1);
	pthread_t	mtr;
	pthread_create(&mtr, NULL, monitoring, monitor);
	pthread_join(mtr, NULL);
	return (0);
}
