/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 20:29:50 by ymeziane          #+#    #+#             */
/*   Updated: 2024/02/09 16:47:48 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	check_valid_flag(char *flag)
{
	if (ft_strncmp(flag, "-n", 2) == 0)
	{
		flag += 2;
		while (*flag)
		{
			if (*flag != 'n')
				return (false);
			flag++;
		}
		return (true);
	}
	return (false);
}

void	echo(t_token **tokenlist)
{
	t_token	*tmp;
	int		i;
	int		flag;

	tmp = *tokenlist;
	i = 0;
	flag = check_valid_flag(tmp->element);
	while(tmp && check_valid_flag(tmp->element) == true)
		tmp = tmp->next;
	while (tmp)
	{
		if (i > 0)
			write(1, " ", 1);
        write(1, tmp->element, ft_strlen(tmp->element));
		tmp = tmp->next;
		i++;
	}
	if (flag != 1)
		write(1, "\n", 1);
}
