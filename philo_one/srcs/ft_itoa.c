#include "philosophers.h"

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

static char		*int_min(void)
{
	long int	nb;
	char		*str;
	int			i;

	nb = 2147483648;
	i = 11;
	if (!(str = malloc(sizeof(char) * 12)))
		return (NULL);
	str[0] = '-';
	str[i] = '\0';
	i--;
	while (i > 0)
	{
		str[i--] = 48 + (nb % 10);
		nb = nb / 10;
	}
	return (str);
}

static	char	*neg_itoa(int n)
{
	char	*dst;
	int		i;

	if (n == -2147483648)
		return (int_min());
	n = n * -1;
	i = nblen(n);
	if (!(dst = malloc(sizeof(char) * (i + 2))))
		return (NULL);
	dst[0] = '-';
	dst[i + 1] = '\0';
	while (i > 0)
	{
		dst[i--] = 48 + (n % 10);
		n = n / 10;
	}
	return (dst);
}

char			*ft_itoa(int n)
{
	char	*dst;
	int		i;

	if (n < 0)
		return (neg_itoa(n));
	i = nblen(n);
	if (n == 0)
	{
		if (!(dst = malloc(sizeof(char) * 2)))
			return (NULL);
		dst[0] = '0';
		dst[1] = '\0';
		return (dst);
	}
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
