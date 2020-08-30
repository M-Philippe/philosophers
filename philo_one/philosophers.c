#include "philosophers.h"

long time_eat = 200;
long time_sleep = 200;
long time_think = 200;
long time_to_starve = 410;

/*
**      The goal is to go for the lowest id_fork first
*/



/*
**      Return timestamp in microseconds
*/

/*void    print_state(t_table *philo, int state)
{
    long     ret;

    ret = actualize_timestamp(philo) / 1000;
    if (state == FORK)
        printf("%ld %d has taken a fork\n", ret, philo->id);
    else if (state == EATING)
        printf("%ld %d is eating\n", ret, philo->id);
    else if (state == SLEEPING)
        printf("%ld %d is sleeping\n", ret, philo->id);
    else if (state == THINKING)
        printf("%ld %d is thinking\n", ret, philo->id);
}*/

void print_state(t_table *philo, int state, int fork_id)
{
    long ret;
    char    *buffer;

    buffer = malloc(100);
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

void    quit_program(t_table *philo)
{
    actualize_timestamp(philo);
    pthread_mutex_lock(&philo->time->time_lock);
    printf("%ld [%ld-%ld] %d should have died\n", philo->time->timestamp / 1000, philo->time_meal, philo->last_meal, philo->id);
    philo->last_meal = philo->time->timestamp / 1000;
    pthread_mutex_unlock(&philo->time->time_lock);
    //exit(0);
}

void    philo_action(t_table *philo, long time, int state)
{
    long     action_time;
    long    tmp;
    long    ret;

    ret = 0;
    action_time = 0;
    print_state(philo, state, 0);
    tmp = actualize_timestamp(philo);
    if (state == EATING)
        philo->last_meal = tmp / 1000;
    philo->time_meal = tmp / 1000;
    if (philo->time_meal - philo->last_meal > philo->time_to_starve)
        quit_program(philo);
    while (action_time <= time)
    {
        ret = actualize_timestamp(philo);
        philo->time_meal = ret / 1000;
        if (philo->time_meal - philo->last_meal > philo->time_to_starve)
            quit_program(philo);
        if (ret >= tmp)
            action_time = ret - tmp;
        else if (ret < tmp)
            action_time = ((MAX_USEC - tmp) + ret);
        action_time /= 1000;
    }
}

void *philo_state(void *arg)
{
    t_table *philo;
    int     count;

    philo = arg;
    philo->last_meal = 0;
    philo->time_meal = 0;
    count = -1;
    while (count <= philo->turn)
    {
        take_fork(philo);
        philo_action(philo, philo->time_eat, EATING);
        free_fork(philo);
        if (philo->turn != 0)
            count++;
        if (count == philo->turn)
            break;
        philo_action(philo, philo->time_sleep, SLEEPING);
        philo_action(philo, philo->time_think, THINKING);
    }
    return (NULL);
}

t_table     *set_philosophers(int nb_philosophers)
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
    while (count <= nb_philosophers)
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
        tmp->time_eat = time_eat;
        tmp->time_sleep = time_sleep;
        tmp->time_think = time_think;
        tmp->time_to_starve = time_to_starve;
        tmp->turn = 0;
        if (!(tmp->r_fork = malloc(sizeof(t_fork))))
            printf("ERROR MALLOC\n");
        tmp->r_fork->state = FREE;
        tmp->r_fork->id_fork = count;
        tmp->n_philo = nb_philosophers;
        if (count % 2 == 0)
            tmp->use_hand = RIGHT;
        else
            tmp->use_hand = LEFT;
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
    int n_philo = 4;

    philo = set_philosophers(n_philo);
    pthread_mutex_lock(&philo->time->time_lock);
    gettimeofday(&philo->time->tv, NULL);
    philo->time->timestamp = 0;
    philo->time->start_program = philo->time->tv.tv_usec;
    pthread_mutex_unlock(&philo->time->time_lock);
    while (philo)
    {
        if (pthread_create(&philo->th, NULL, philo_state, philo) == -1)
            printf("ERROR THREAD\n");
        pthread_detach(philo->th);
        usleep(20);
        philo = philo->next;
    }
    while (1)
    {}
    return (0);
}

/*void    print_value(t_philo *philo)
{
    printf("numbers of philo : %d\n", philo->n_philo);
    printf("time_to_die : %d\n", philo->time_to_die);
    printf("time to eat : %d\n", philo->time_to_eat);
    printf("time to sleep : %d\n", philo->time_to_sleep);
    printf("numbers of time must eat : %d\n", philo->n_time_must_eat);
    //printf("MAX_THREAD : %d\n", PTHREAD_THREADS_MAX);
}*/


    /*t_philo     philo;

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
    print_value(&philo);*/