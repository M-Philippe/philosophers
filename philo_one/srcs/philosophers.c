#include "philosophers.h"


/*
 **      only take the fork by pair -> Doesn't work
 */

void print_state(t_table *philo, int state, int fork_id)
{
	long ret;
	char    *buffer;

	if (is_philo_dead(philo, ASK))
		return ;
	buffer = memset(malloc(100), '\0', 99);
	ret = actualize_timestamp(philo) / 1000;
	strcat(buffer, ft_itoa((int)ret));
	strcat(buffer, " ");
	strcat(buffer, ft_itoa(philo->id));
	if (state == FORK)
	{
		strcat(buffer, " has taken a fork number ");
		strcat(buffer, ft_itoa(fork_id));
		strcat(buffer, "\n");
	}
	else if (state == EATING)
		strcat(buffer, " is eating\n");
	else if (state == SLEEPING)
		strcat(buffer, " is sleeping\n");
	else if (state == THINKING)
		strcat(buffer, " is thinking\n");
	write(1, buffer, strlen(buffer));
}

void    print_death(t_table *philo)
{
	long ret;
	char    *buffer;

	buffer = memset(malloc(100), '\0', 99);
	ret = actualize_timestamp(philo) / 1000;
	strcat(buffer, ft_itoa((int)ret));
	strcat(buffer, " ");
	strcat(buffer, ft_itoa(philo->id));
	strcat(buffer, " is dead\n");
	write(1, buffer, strlen(buffer));
}

void    philo_action(t_table *philo, long time, int state)
{
	long     action_time;
	long    tmp;
	long    ret;

	if (is_philo_dead(philo, ASK))
		return ;
	ret = 0;
	action_time = 0;
	tmp = actualize_timestamp(philo);
	print_state(philo, state, 0);
	if (state == EATING)
		philo->last_meal = tmp / 1000;
	philo->time_meal = tmp / 1000;
	if (philo->time_meal - philo->last_meal > philo->time_to_starve)
		is_philo_dead(philo, SET);
	while (action_time <= time)
	{
		ret = actualize_timestamp(philo);
		philo->time_meal = ret / 1000;
		if (philo->time_meal - philo->last_meal > philo->time_to_starve)
			is_philo_dead(philo, SET);
		if (ret >= tmp)
			action_time = ret - tmp;
		else if (ret < tmp)
			action_time = ((MAX_USEC - tmp) + ret);
		action_time /= 1000;
	}
}

int     is_philo_dead(t_table *philo, int flag)
{
	int ret;

	ret = 0;
	pthread_mutex_lock(&philo->monitor->mtx);
	if (flag == SET && philo->monitor->someone_died != 1)
	{
		philo->monitor->someone_died = 1;
		print_death(philo);
	}
	else if (flag == ASK)
		ret = philo->monitor->someone_died;
	pthread_mutex_unlock(&philo->monitor->mtx);
	return (ret);
}

int     is_philo_done(t_info *monitor, int flag)
{
	int ret;

	ret = 0;
	if (flag == SET)
	{
		pthread_mutex_lock(&monitor->mtx);
		monitor->done++;
		pthread_mutex_unlock(&monitor->mtx);
	}
	else if (flag == ASK)
	{
		pthread_mutex_lock(&monitor->mtx);
		ret = monitor->done;
		pthread_mutex_unlock(&monitor->mtx);
	}
	return (ret);
}

void *philosophize(void *arg)
{
	t_table *philo;
	int     count;

	philo = arg;
	philo->last_meal = 0;
	philo->time_meal = 0;
	count = 0;
	while (is_philo_dead(philo, ASK) == 0)
	{
		take_fork(philo);
		philo_action(philo, philo->time_to_eat, EATING);
		free_fork(philo);
		count++;
		if (count == philo->turns)
			break ;
		philo_action(philo, philo->time_to_sleep, SLEEPING);
		print_state(philo, THINKING, 0);
	}
	is_philo_done(philo->monitor, SET);
	return (NULL);
}

t_table     *set_philosophers(t_args *args, t_info *monitor)
{
	int     count;
	t_table *head;
	t_table *tmp;
	t_table *link;

	count = 1;
	tmp = NULL;
	head = NULL;
	link = NULL;
	t_time *time;
	time = malloc(sizeof(t_time));
	gettimeofday(&time->tv, NULL);
	time->timestamp = 0;
	time->start_program = time->tv.tv_usec;
	pthread_mutex_init(&time->time_lock, NULL);
	while (count <= args->nb_philo)
	{
		tmp = malloc(sizeof(t_table));
		if (link)
		{
			link->next = tmp;
			tmp->prev = link;
		}
		if (count == 1)
			head = tmp;
		tmp->id = count;
		tmp->time_to_eat = args->time_to_eat;
		tmp->time_to_sleep = args->time_to_sleep;
		tmp->time_to_starve = args->time_to_starve;
		tmp->turns = args->n_time_must_eat;
		tmp->monitor = monitor;
		if (tmp->turns == 0)
			tmp->turns = -1;
		if (!(tmp->r_fork = malloc(sizeof(t_fork))))
			printf("ERROR MALLOC\n");
		tmp->r_fork->state = FREE;
		tmp->r_fork->id_fork = count;
		tmp->nb_philo = args->nb_philo;
		pthread_mutex_init(&tmp->r_fork->mtx, NULL);
		tmp->time = time;
		tmp->next = NULL;
		link = tmp;
		tmp = tmp->next;
		count++;
	}
	head->prev = link;
	return (head);
}

int     main(int ac, char **av)
{
	t_table *philo;
	t_args  *args;
	t_info  *monitor;

	args = parsing(ac, av);
	if (!(monitor = malloc(sizeof(t_info))))
		printf("Error malloc\n");
	monitor->done = 0;
	monitor->someone_died = 0;
	pthread_mutex_init(&monitor->mtx, NULL);
	philo = set_philosophers(args, monitor);
	pthread_mutex_lock(&philo->time->time_lock);
	gettimeofday(&philo->time->tv, NULL);
	philo->time->timestamp = 0;
	philo->time->start_program = philo->time->tv.tv_usec;
	pthread_mutex_unlock(&philo->time->time_lock);
	while (philo)
	{
		if (pthread_create(&philo->th, NULL, philosophize, philo) != 0)
			printf("ERROR THREAD\n");
		pthread_detach(philo->th);
		philo = philo->next;
	}
	/*Put the check of death here*/
	while (is_philo_done(monitor, ASK) != args->nb_philo)
	{
		/* FREE */
		usleep(10);
	}
	printf("#######################################\n");
	return (0);
}
