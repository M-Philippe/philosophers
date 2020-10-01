/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pminne <pminne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/07 17:17:44 by pminne            #+#    #+#             */
/*   Updated: 2020/10/01 17:35:52 by pminne           ###   ########lyon.fr   */
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

long			ft_atoi(char *s)
{
	int		neg;
	long	nb;
	int		i;

	nb = 0;
	neg = 1;
	i = 0;
	while (s[i] == '-' || s[i] == '+')
	{
		if (s[i] == '-')
			neg *= -1;
		i++;
	}
	if (neg == -1 || ft_strlen(&s[i]) >= 10)
		return (-1);
	if (ft_is_number(&s[i]))
		return (-1);
	while (s[i])
	{
		nb = (nb * 10) + s[i] - 48;
		i++;
	}
	return (nb <= 0 ? -1 : nb);
}

char			*ft_strdup(char *s)
{
	char	*dst;
	int		i;

	i = 0;
	if (!(dst = malloc(ft_strlen(s) + 1)))
		return (NULL);
	while (s[i])
	{
		dst[i] = s[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}
