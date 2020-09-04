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
	if (is_someone_dead(philo) == 1)
	{
		pthread_mutex_unlock(&philo->write->writing);
		return ;
	}
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
		philo->meal->time_meal = (timestamp(philo) - philo->meal->start_program) - philo->meal->last_meal;
		if (philo->meal->time_meal > philo->meal->time_to_starve)
		{
			printf("%ld %ld\n", philo->meal->time_meal, philo->meal->last_meal);
			pthread_mutex_unlock(&philo->meal->mtx);
			break ;
		}
		pthread_mutex_unlock(&philo->meal->mtx);
		take_fork(philo);
		pthread_mutex_lock(&philo->meal->mtx);
		philo->meal->last_meal = timestamp(philo) - philo->start_program;
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
	pthread_mutex_lock(&philo->monitor->mtx);
	philosophers_done(philo->monitor, SET);
	pthread_mutex_unlock(&philo->monitor->mtx);
	return (NULL);
}

int	philosophers_done(t_monitor *mtr, int flag)
{
	int	ret;

	ret = 0;
	if (flag == SET)
		mtr->done++;
	else if (flag == ASK)
	{
		pthread_mutex_lock(&mtr->mtx);
		ret = mtr->done;
		pthread_mutex_unlock(&mtr->mtx);
	}
	return (ret);
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

int	is_someone_dead(t_table *philo)
{
	int ret;

	ret = 0;
	pthread_mutex_lock(&philo->monitor->mtx);
	ret = philo->monitor->someone_died;
	pthread_mutex_unlock(&philo->monitor->mtx);
	return (ret);
}

int	is_dead(t_table *philo)
{
	long	t_stamp;
	int	ret;

	ret = 0;
	pthread_mutex_lock(&philo->meal->mtx);
	pthread_mutex_lock(&philo->monitor->mtx);
	t_stamp = timestamp(NULL);
	philo->meal->time_meal = (t_stamp - philo->meal->start_program) - philo->meal->last_meal;
	if (philo->meal->time_meal > philo->meal->time_to_starve && philo->monitor->someone_died == 0)
	{
		philo->monitor->someone_died = 1;
		ret = 1;
		printf("%ld %d is dead\n", t_stamp - philo->meal->start_program, philo->id);
	}
	pthread_mutex_unlock(&philo->monitor->mtx);
	pthread_mutex_unlock(&philo->meal->mtx);
	return (ret);
}

void	*monitoring(void *arg)
{
	/*	SCHEME
		while (someone_is_dead)
			iterate between philo and calculate death
				if death print and set variable
				if everyone_is_done
				break
		free*/
	t_monitor	*monitor;
	int		nb_philo;
	int		ret;
	t_table		*philo;

	monitor = arg;
	philo = monitor->head;
	pthread_mutex_lock(&monitor->mtx);
	nb_philo = monitor->nb_philo;
	pthread_mutex_unlock(&monitor->mtx);
	int dead = 0;
	while (ret != nb_philo)
	{	if (dead == 0)
		{
			for (int i = 1; i < nb_philo; i++)
			{
				if (is_dead(philo) == 1)
				{
					dead = 1;
					break;
				}
				philo = philo->next;
			}
		}
		ret = philosophers_done(monitor, ASK);
		usleep(100);
	}
	free_all(monitor->head);
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
	monitor->head = philo;
	for (int i = 1; i <= args->nb_philo; i++)
	{
		if (pthread_create(&philo->th, NULL, philosophize, philo) != 0)
			printf("Error Thread\n");
		pthread_detach(philo->th);
		philo = philo->next;
	}
	//printf("id %d\n", philo->id);
	//pthread_t	*monitoring;
	//pthread_create(monitor
	//while (1);
	pthread_t	mtr;
	pthread_create(&mtr, NULL, monitoring, (void*)monitor);
	pthread_join(mtr, NULL);
	pthread_mutex_destroy(&monitor->mtx);
	free(monitor);
	free(args);
	return (0);
}
