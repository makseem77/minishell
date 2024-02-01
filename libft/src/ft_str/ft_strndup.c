/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 12:06:57 by ymeziane          #+#    #+#             */
/*   Updated: 2024/02/01 15:53:01 by maxborde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(const char *s, size_t n)
{
	char	*result;
	size_t	i;
	size_t	s_len;

	i = 0;
	s_len = ft_strlen(s);
	if (!n)
		return (NULL);
	if (n < s_len)
		s_len = n;
	result = (char *)malloc(s_len + 1);
	if (!result)
		return (NULL);
	while (*s && i < s_len)
		result[i++] = *s++;
	result[i] = '\0';
	return (result);
}
