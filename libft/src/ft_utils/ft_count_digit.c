/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_count_digit.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/25 12:23:13 by ymeziane          #+#    #+#             */
/*   Updated: 2023/10/25 12:32:56 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

unsigned int	ft_count_digit(int n)
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
