/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 17:12:28 by ymeziane          #+#    #+#             */
/*   Updated: 2023/10/17 15:38:19 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "stdint.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	unsigned char	*result;
	size_t			total_size;

	if (nmemb == 0 || size == 0)
	{
		nmemb = 1;
		size = 1;
	}
	if (SIZE_MAX / nmemb < size)
		return (NULL);
	total_size = nmemb * size;
	result = (unsigned char *)malloc(total_size);
	if (!result)
		return (NULL);
	ft_bzero(result, total_size);
	return ((void *)result);
}
