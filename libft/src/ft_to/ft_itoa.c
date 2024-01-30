/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 15:49:17 by ymeziane          #+#    #+#             */
/*   Updated: 2023/10/17 15:38:15 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static unsigned int	count_digit(long n)
{
	unsigned int	digits;

	digits = 0;
	if (n == 0)
		return (1);
	while (n)
	{
		digits++;
		n /= 10;
	}
	return (digits);
}

char	*ft_itoa(int n)
{
	char			*result;
	unsigned int	digits;
	int				sign;
	long			nb;

	nb = (long)n;
	sign = 0;
	if (nb < 0)
	{
		nb = -nb;
		sign = 1;
	}
	digits = count_digit(nb);
	result = (char *)malloc(digits + sign + 1);
	if (!result)
		return (NULL);
	result[digits + sign] = '\0';
	while (digits)
	{
		result[--digits + sign] = (nb % 10) + '0';
		nb /= 10;
	}
	if (sign)
		result[0] = '-';
	return (result);
}
