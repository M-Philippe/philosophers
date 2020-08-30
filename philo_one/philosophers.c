#include "philosophers.h"

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
    printf("%ld [%ld-%ld] %d have died\n", philo->time->timestamp / 1000, philo->time_meal, philo->last_meal, philo->id);
    pthread_mutex_unlock(&philo->time->time_lock);
    exit(0);
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
        philo_action(philo, philo->time_to_eat, EATING);
        free_fork(philo);
        if (philo->turn != 0)
            count++;
        if (count == philo->turn)
            break;
        philo_action(philo, philo->time_to_sleep, SLEEPING);
        print_state(philo, THINKING, 0);
    }
    return (NULL);
}

t_table     *set_philosophers(t_args *args)
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
        tmp->turn = args->n_time_must_eat;
        if (!(tmp->r_fork = malloc(sizeof(t_fork))))
            printf("ERROR MALLOC\n");
        tmp->r_fork->state = FREE;
        tmp->r_fork->id_fork = count;
        tmp->nb_philo = args->nb_philo;
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
    t_args  *args;

    args = parsing(ac, av);
    philo = set_philosophers(args);
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
        philo = philo->next;
    }
    while (1)
    {}
    return (0);
}