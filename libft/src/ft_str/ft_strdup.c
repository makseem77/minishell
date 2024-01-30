/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 12:06:57 by ymeziane          #+#    #+#             */
/*   Updated: 2023/10/17 15:38:02 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*result;
	int		i;
	size_t	s_len;

	i = 0;
	s_len = ft_strlen(s);
	result = (char *)malloc(s_len + 1);
	if (!result)
		return (NULL);
	while (*s)
		result[i++] = *s++;
	result[i] = '\0';
	return (result);
}
