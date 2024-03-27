/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoll.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2024/02/09 15:01:38 by ymeziane          #+#    #+#             */
/*   Updated: 2024/02/09 15:01:38 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static long long int	parse_number(const char *nptr, int sign,
		bool *overflow)
{
	long long int	result;

	result = 0;
	while (ft_isdigit(*nptr))
	{
		result = result * 10 + (*nptr++ - '0');
		if (result < 0)
		{
			if (sign == 1)
				return (*overflow = true, LLONG_MAX);
			else
				return (*overflow = true, LLONG_MIN);
		}
	}
	return (result * sign);
}

long long int	ft_atoll(const char *nptr, bool *overflow)
{
	int	sign;

	sign = 1;
	*overflow = false;
	while ((*nptr >= '\t' && *nptr <= '\r') || *nptr == ' ')
		nptr++;
	if (ft_strcmp(nptr, "-9223372036854775808") == 0)
		return (LLONG_MIN);
	if (*nptr == '+' || *nptr == '-')
	{
		if (*nptr == '-')
			sign = -1;
		nptr++;
	}
	return (parse_number(nptr, sign, overflow));
}
