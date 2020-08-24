#include "philosophers.h"

#define FORK 0
#define EATING 1
#define SLEEPING 2
#define THINKING 3
#define FREE 0
#define TAKEN 1

void    print_state(t_table *philo, int state)
{
    long     ret;

    ret = 0;
    pthread_mutex_lock(&philo->time->time_lock);
    gettimeofday(&philo->time->tv, NULL);
    if (philo->time->tv.tv_usec >= philo->time->start_program)
        philo->time->timestamp += philo->time->tv.tv_usec - philo->time->start_program;
    else if (philo->time->tv.tv_usec < philo->time->start_program)
        philo->time->timestamp += ((MAX_USEC- philo->time->start_program) + philo->time->tv.tv_usec);
    ret = philo->time->timestamp / 1000;
    if (state == FORK)
        printf("%ld %d has taken a fork\n", ret, philo->id);
    else if (state == EATING)
        printf("%ld %d is eating\n", ret, philo->id);
    else if (state == SLEEPING)
        printf("%ld %d is sleeping\n", ret, philo->id);
    else if (state == THINKING)
        printf("%ld %d is thinking\n", ret, philo->id);
    philo->time->start_program = philo->time->tv.tv_usec;
    pthread_mutex_unlock(&philo->time->time_lock);
}

void    philo_eat(t_table *philo, long time, int state)
{
    long     action_time;
    long    tmp;
    struct timeval tv;

    print_state(philo, state);
    gettimeofday(&tv, NULL);
    tmp = tv.tv_usec;
    action_time = 0;
    while (action_time <= time)
    {
        gettimeofday(&tv, NULL);
        if (tv.tv_usec >= tmp)
            action_time = tv.tv_usec - tmp;
        else if (tv.tv_usec < tmp)
            action_time = ((MAX_USEC - tmp) + tv.tv_usec);
        action_time /= 1000;
        usleep(1);
    }
}


void    philo_action(t_table *philo, long time, int state)
{
    long     action_time;
    long    tmp;
    struct timeval tv;

    print_state(philo, state);
    gettimeofday(&tv, NULL);
    tmp = tv.tv_usec;
    action_time = 0;
    while (action_time <= time)
    {
        gettimeofday(&tv, NULL);
        if (tv.tv_usec >= tmp)
            action_time = tv.tv_usec - tmp;
        else if (tv.tv_usec < tmp)
            action_time = ((MAX_USEC - tmp) + tv.tv_usec);
        action_time /= 1000;
        usleep(1);
    }
}

void        take_fork(t_table *philo)
{
    int fork;

    fork = 0;
    while (fork != 2)
    {
        pthread_mutex_lock(&philo->prev->r_fork->mtx);
        if (philo->prev->r_fork->state == FREE)
        {
            philo->prev->r_fork->state = TAKEN;
            fork++;
            print_state(philo, FORK);
        }
        pthread_mutex_unlock(&philo->prev->r_fork->mtx);
        pthread_mutex_lock(&philo->r_fork->mtx);
        if (philo->r_fork->state == FREE)
        {
            philo->r_fork->state = TAKEN;
            fork++;
            print_state(philo, FORK);
        }
        pthread_mutex_unlock(&philo->r_fork->mtx);
    }
}

void free_fork(t_table *philo)
{
    pthread_mutex_lock(&philo->prev->r_fork->mtx);
    philo->prev->r_fork->state = FREE;
    pthread_mutex_unlock(&philo->prev->r_fork->mtx);
    pthread_mutex_lock(&philo->r_fork->mtx);
    philo->r_fork->state = FREE;
    pthread_mutex_unlock(&philo->r_fork->mtx);
}

void *philo_state(void *arg)
{
    t_table *philo = arg;
    int fork;
    long    last_meal;
    long    time_meal;

    fork = 0;
    last_meal = 0;
    time_meal = 0;
    while (1)
    {
        pthread_mutex_lock(&philo->time->time_lock);
        time_meal = philo->time->timestamp / 1000;
        pthread_mutex_unlock(&philo->time->time_lock);
        if (time_meal - last_meal > philo->time_to_starve)
        {
            printf("%d %ld %ld died\n", philo->id, time_meal, last_meal);
            exit(0);
        }
        take_fork(philo);
        philo_eat(philo, philo->time_eat, EATING);
        pthread_mutex_lock(&philo->time->time_lock);
        last_meal = philo->time->timestamp / 1000;
        pthread_mutex_unlock(&philo->time->time_lock);
        free_fork(philo);
        philo_action(philo, philo->time_sleep, SLEEPING);
        philo_action(philo, philo->time_think, THINKING);
    }
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
        if (!(tmp->r_fork = malloc(sizeof(t_fork))))
            printf("ERROR MALLOC\n");
        tmp->r_fork->state = FREE;
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

    int n_philo = 5;
    philo = set_philosophers(n_philo);
    while (philo)
    {
        if (pthread_create(&philo->th, NULL, philo_state, philo) == -1)
            printf("ERROR THREAD\n");
        pthread_detach(philo->th);
        usleep(1);
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