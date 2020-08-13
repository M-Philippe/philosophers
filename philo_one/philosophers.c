#include <stdio.h>
#include <stdlib.h>
#include "philosophers.h"

void    print_value(t_philo *philo)
{
    printf("numbers of philo : %d\n", philo->n_philo);
    printf("time_to_die : %d\n", philo->time_to_die);
    printf("time to eat : %d\n", philo->time_to_eat);
    printf("time to sleep : %d\n", philo->time_to_sleep);
    printf("numbers of time must eat : %d\n", philo->n_time_must_eat);
}

int     main(int ac, char **av)
{
    t_philo     philo;

    if (ac <= 4 || ac >= 7)
        return (0);
    philo.n_philo = atoi(av[1]);
    philo.time_to_die = atoi(av[2]);
    philo.time_to_eat = atoi(av[3]);
    philo.time_to_sleep = atoi(av[4]);
    if (av[5])
        philo.n_time_must_eat = atoi(av[5]);
    else
        philo.n_time_must_eat = 0;
    print_value(&philo);
    return (0);
}