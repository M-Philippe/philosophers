/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pminne <pminne@42lyon.student.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/07 17:17:44 by pminne            #+#    #+#             */
/*   Updated: 2020/09/08 00:05:06 by pminne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void			ft_putnbr_fd(long n, int fd)
{
	char c;

	if (n == 0)
		write(fd, "0", 1);
	else if (n < 0)
	{
		write(fd, "-", 1);
		n *= -1;
		ft_putnbr_fd(n, fd);
	}
	else if (n < 10)
	{
		c = n + 48;
		write(fd, &c, 1);
	}
	else
	{
		ft_putnbr_fd(n / 10, fd);
		ft_putnbr_fd(n % 10, fd);
	}
}

unsigned int	ft_strlen(char *s)
{
	int i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

static int		ft_is_number(char *s)
{
	int i;

	i = 0;
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (1);
		i++;
	}
	return (0);
}

int				ft_atoi(char *s)
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
	return (nb == 0 ? -1 : nb);
}

void			*free_table(t_table *philo, t_write *write)
{
	t_table		*tmp;

	tmp = NULL;
	free(write);
	while (philo)
	{
		tmp = philo;
		philo = philo->prev;
		(tmp->meal) ? (free(tmp->meal)) : 0;
		//(tmp->r_fork) ? (free(tmp->r_fork)) : 0;
		free(tmp);
		tmp = NULL;
	}
	return (NULL);
}
