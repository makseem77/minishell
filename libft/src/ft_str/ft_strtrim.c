/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 19:38:07 by ymeziane          #+#    #+#             */
/*   Updated: 2023/10/17 15:37:54 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	find_char(const char *str, char c)
{
	while (*str)
	{
		if (*str == c)
			return (1);
		str++;
	}
	return (0);
}

char	*ft_strtrim(const char *s1, const char *set)
{
	char	*result;
	size_t	start;
	size_t	end;
	size_t	len;

	if (!s1 || !set)
		return (NULL);
	start = 0;
	while (s1[start] && find_char(set, s1[start]))
		start++;
	end = ft_strlen(s1);
	if (!end--)
		return (ft_strdup(""));
	while (end >= start && find_char(set, s1[end]))
		end--;
	len = end - start + 1;
	result = (char *)malloc(len + 1);
	if (!result)
		return (NULL);
	len = 0;
	while (start <= end)
		result[len++] = s1[start++];
	result[len] = '\0';
	return (result);
}
