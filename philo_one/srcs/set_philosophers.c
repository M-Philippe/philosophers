#include "philosophers.h"

t_time *set_time(void)
{
	t_time		*time;
	struct timeval	tv;

	if (!(time = malloc(sizeof(t_time))))
		printf("Error malloc time\n");
	pthread_mutex_init(&time->time_lock, NULL);
	gettimeofday(&tv, NULL);
	// Check here
	time->start_program = (tv.tv_sec * 1000 + tv.tv_usec / 1000);
	return (time);
}

t_table *set_philosophers(t_args *args, t_monitor *mtr)
{
	int	count;
	t_table	*head;
	t_table	*tmp;
	t_table	*philo;
	t_time	*time;
	t_write *write;

	count = 1;
	head = NULL;
	tmp = NULL;
	philo = NULL;
	time = set_time();
	if (!(write = malloc(sizeof(t_write))))
		printf("Error malloc write\n");
	pthread_mutex_init(&write->writing, NULL);
	while (count <= args->nb_philo)
	{
		if (!(philo = malloc(sizeof(t_table))))
			printf("Error malloc philo\n");
		if (tmp)
		{
			tmp->next = philo;
			philo->prev = tmp;
		}
		if (count == 1)
			head = philo;
		philo->id = count;
		philo->nb_philo = args->nb_philo;
		philo->time_to_eat = args->time_to_eat;
		philo->time_to_sleep = args->time_to_sleep;
		philo->time_to_starve = args->time_to_starve;
		philo->turns = args->n_time_must_eat;
		philo->start_program = time->start_program;
		// MONITOR
		philo->monitor = mtr;
		//set_monitor(philo, tmp, count);
		philo->write = write;
		if (philo->turns == 0)
			philo->turns = -1;
		if (!(philo->r_fork = malloc(sizeof(t_fork))))
			printf("Error malloc fork_n");
		pthread_mutex_init(&philo->r_fork->fork, NULL);
		philo->time = time;
		philo->next = NULL;
		tmp = philo;
		philo = philo->next;
		count++;
	}
	tmp->next = head;
	head->prev = tmp;
	//mtr = head->monitor;
	return (head);
}
