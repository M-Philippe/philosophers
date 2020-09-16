/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pminne <pminne@42lyon.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/07 17:15:20 by pminne            #+#    #+#             */
/*   Updated: 2020/09/16 14:32:55 by pminne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	print_state(t_table *philo, int id, int state, int fork_id)
{
	pthread_mutex_lock(&philo->write->writing);
	/*if (is_someone_dead(philo) == 1)
	{
		pthread_mutex_unlock(&philo->write->writing);
		return ;
	}*/
	ft_putnbr_fd((timestamp() - philo->start_program), 1);
	write(1, " ", 1);
	ft_putnbr_fd(id, 1);
	if (state == FORK)
	{
		write(1, " has taken a fork ", ft_strlen(" has taken a fork\n"));
		ft_putnbr_fd(fork_id, 1);
		write(1, "\n", 1);
	}
	else if (state == EATING)
		write(1, " is eating\n", ft_strlen(" is eating\n"));
	else if (state == SLEEPING)
		write(1, " is sleeping\n", ft_strlen(" is sleeping\n"));
	else if (state == THINKING)
		write(1, " is thinking\n", ft_strlen(" is thinking\n"));
	pthread_mutex_unlock(&philo->write->writing);
}

void	print_death(t_table *philo, long t_stamp)
{
	pthread_mutex_lock(&philo->write->writing);
	ft_putnbr_fd(t_stamp, 1);
	write(1, " ", 1);
	ft_putnbr_fd(philo->id + 1, 1);
	write(1, " is dead\n", 9);
	pthread_mutex_unlock(&philo->write->writing);
}
