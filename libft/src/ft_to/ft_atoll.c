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


long long int	ft_atoll(const char *nptr, bool *overflow)
{
	int sign;
	long long int result;

	sign = 1;
	result = 0;
	while ((*nptr >= '\t' && *nptr <= '\r') || *nptr == ' ')
		nptr++;
	if (ft_strcmp(nptr, "-9223372036854775808") == 0)
		return (*overflow = false, LLONG_MIN);
	if (*nptr == '+' || *nptr == '-')
	{
		if (*nptr == '-')
			sign = -1;
		nptr++;
	}
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
