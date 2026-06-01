/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   number_str.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esmaeil <esaleh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 00:00:00 by esmaeil           #+#    #+#             */
/*   Updated: 2026/05/27 00:00:00 by esmaeil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_atoi(const char *s)
{
	int		sign;
	long	result;

	sign = 1;
	result = 0;
	while (*s == ' ' || (*s >= '\t' && *s <= '\r'))
		s++;
	if (*s == '-' || *s == '+')
	{
		if (*s == '-')
			sign = -1;
		s++;
	}
	while (*s >= '0' && *s <= '9')
		result = result * 10 + (*s++ - '0');
	return ((int)(result * sign));
}

static int	number_len(long nb)
{
	int	len;

	len = 1;
	if (nb < 0)
	{
		nb = -nb;
		len++;
	}
	while (nb >= 10)
	{
		nb /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char	*str;
	long	nb;
	int		len;

	nb = n;
	len = number_len(nb);
	if (nb < 0)
		nb = -nb;
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	str[len] = '\0';
	while (len-- > 0)
	{
		str[len] = (nb % 10) + '0';
		nb /= 10;
	}
	if (n < 0)
		str[0] = '-';
	return (str);
}
