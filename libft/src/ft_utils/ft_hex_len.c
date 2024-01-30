/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hex_len.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 16:39:37 by ymeziane          #+#    #+#             */
/*   Updated: 2023/10/27 13:14:01 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_hex_len(unsigned int val)
{
	int	len;

	len = 0;
	if (val == 0)
		return (1);
	while (val)
	{
		val /= 16;
		len++;
	}
	return (len);
}
