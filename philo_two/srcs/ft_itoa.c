/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_itoa.c                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: pminne <marvin@le-101.fr>                  +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/14 16:41:16 by pminne       #+#   ##    ##    #+#       */
/*   Updated: 2019/10/14 16:45:00 by pminne      ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include <stdlib.h>

static	int		nblen(int n)
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

char			*ft_itoa(int n)
{
	char	*dst;
	int		i;

	i = nblen(n);
	if (n == 0)
	{
		if (!(dst = malloc(sizeof(char) * 2)))
			return (NULL);
		dst[0] = '0';
		dst[1] = '\0';
		return (dst);
	}
	if (!(dst = malloc(sizeof(char) * (i + 1))))
		return (NULL);
	dst[i] = '\0';
	while (n > 0)
	{
		dst[--i] = 48 + (n % 10);
		n = n / 10;
	}
	return (dst);
}
