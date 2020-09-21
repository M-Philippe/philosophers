/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   meal_name.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pminne <pminne@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/21 18:26:18 by pminne            #+#    #+#             */
/*   Updated: 2020/09/21 18:46:42 by pminne           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int			nblen(int n)
{
	int i;

	i = 0;
	while (n > 0)
	{
		n = n / 10;
		i++;
	}
	return (i);
}

char		*ft_itoa(int n)
{
	char	*dst;
	int		i;

	i = nblen(n);
	dst = malloc(sizeof(char) * (i + 1));
	if (dst == NULL)
		return (NULL);
	dst[i] = '\0';
	while (n > 0)
	{
		dst[--i] = 48 + (n % 10);
		n = n / 10;
	}
	return (dst);
}

char		*ft_strcat(char *dest, const char *src)
{
	int i;
	int j;

	i = 0;
	j = 0;
	while (dest[i] != '\0')
	{
		i++;
	}
	while (src[j] != '\0')
	{
		dest[i] = src[j];
		i++;
		j++;
	}
	dest[i] = '\0';
	return (dest);
}

void		meal_name(char *sem_name, int i)
{
	char *ita;

	ita = NULL;
	memset(sem_name, '\0', MEAL_LEN - 1);
	ft_strcat(sem_name, "/meal ");
	ita = ft_itoa(i);
	ft_strcat(sem_name, ita);
	free(ita);
}
