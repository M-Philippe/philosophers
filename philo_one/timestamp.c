#include "philosophers.h"

long    actualize_timestamp(t_table *philo)
{
    long    ret;

    ret = 0;
    pthread_mutex_lock(&philo->time->time_lock);
    gettimeofday(&philo->time->tv, NULL);
    if (philo->time->tv.tv_usec >= philo->time->start_program)
        philo->time->timestamp += philo->time->tv.tv_usec - philo->time->start_program;
    else if (philo->time->tv.tv_usec < philo->time->start_program)
        philo->time->timestamp += ((MAX_USEC- philo->time->start_program) + philo->time->tv.tv_usec);
    ret = philo->time->timestamp;
    philo->time->start_program = philo->time->tv.tv_usec;
    pthread_mutex_unlock(&philo->time->time_lock);
    return (ret);
}