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

int	ft_is_number(char *s)
{
	int i;

	i = 0;
	while (s[i])
	{
		if (s[i] < '0' && s[i] > '9')
			return (1);
		i++;
	}
	return (0);
}

int	ft_atoi(char *s)
{
	int	neg;
	int	nb;
	int	i;

	nb = 0;
	neg = 1;
	i = 0;
	while (s[i] == '-' || s[i] == '+')
	{
		if (s[i] == '-')
			neg *= -1;
		i++;
	}
	if (neg == -1)
		return (-1);
	if (ft_is_number(&s[i]))
		return (-1);
	while (s[i])
	{
		nb = (nb * 10) + s[i] - 48;
		i++;
	}
	return (nb);
}

t_args    *parsing(int ac, char **av)
{
	t_args   *args;

	if (ac <= 4 || ac >= 7)
	{
		printf("Error args\n");
		exit(0);
		return (0);
	}
	if (!(args = malloc(sizeof(t_args))))
		printf("ERROR MALLOC\n");
	args->nb_philo = ft_atoi(av[1]);
	if (args->nb_philo == 0)
		return (0);
	args->time_to_starve = ft_atoi(av[2]);
	args->time_to_eat = ft_atoi(av[3]);
	args->time_to_sleep = ft_atoi(av[4]);
	if (av[5])
		args->n_time_must_eat = ft_atoi(av[5]);
	else
		args->n_time_must_eat = 0;
//	print_value(args);
//	exit (0);
	return (args);
}
