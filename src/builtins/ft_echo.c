/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 20:29:50 by ymeziane          #+#    #+#             */
/*   Updated: 2024/02/24 00:02:01 by maxborde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Checks if the flag is valid.
// i.e. if it is -n or not.
static bool	check_valid_flag(char *flag)
{
	if (!flag)
		return (false);
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
void	echo(char **args)
{
	int		i;
	int		j;
	int		flag;

	i = 0;
	j = 1;
	if (!*args)
	{
		write(1, "\n", 1);
		return ;
	}
	flag = check_valid_flag(args[j]);
	while (check_valid_flag(args[j]) == true)
		j++;
	while (args[j])
	{
		if (i > 0)
			write(1, " ", 1);
		write(1, args[j], ft_strlen(args[j]));
		j++;
		i++;
	}
	if (flag != 1)
		write(1, "\n", 1);
}
