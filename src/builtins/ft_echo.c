/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 20:29:50 by ymeziane          #+#    #+#             */
/*   Updated: 2024/02/22 16:52:41 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Checks if the flag is valid.
// i.e. if it is -n or not.
static bool	check_valid_flag(char *flag)
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

// Prints the arguments.
// If the first argument is -n, we will not print a newline at the end.
void	echo(t_token **tokenlist)
{
	t_token	*tmp;
	int		i;
	int		flag;

	tmp = *tokenlist;
	i = 0;
	if (!tmp)
	{
		write(1, "\n", 1);
		return ;
	}
	flag = check_valid_flag(tmp->element);
	while (tmp && check_valid_flag(tmp->element) == true)
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
