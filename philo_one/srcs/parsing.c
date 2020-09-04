#include "philosophers.h"

void    print_value(t_args *philo)
{
	printf("numbers of philo : %d\n", philo->nb_philo);
	printf("time_to_die : %d\n", philo->time_to_starve);
	printf("time to eat : %d\n", philo->time_to_eat);
	printf("time to sleep : %d\n", philo->time_to_sleep);
	printf("numbers of time must eat : %d\n", philo->n_time_must_eat);
	//printf("MAX_THREAD : %d\n", PTHREAD_THREADS_MAX);
}

t_args    *parsing(int ac, char **av)
{
	t_args   *args;

	if (ac <= 4 || ac >= 7)
		return (0);
	if (!(args = malloc(sizeof(t_args))))
		printf("ERROR MALLOC\n");
	args->nb_philo = atoi(av[1]);
	if (args->nb_philo == 0)
		return (0);
	args->time_to_starve = atoi(av[2]);
	args->time_to_eat = atoi(av[3]);
	args->time_to_sleep = atoi(av[4]);
	if (av[5])
		args->n_time_must_eat = atoi(av[5]);
	else
		args->n_time_must_eat = 0;
	//print_value(args);
	return (args);
}
