/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 17:56:02 by ymeziane          #+#    #+#             */
/*   Updated: 2024/03/16 16:19:41 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Prints the error message.
void	print_not_found(char *command, char *arg)
{
	if (command[0] == '/')
	{
		print_error(command, arg, "No such file or directory");
		g_status = 1;
	}
	else
	{
		print_error(command, arg, "command not found");
		g_status = 127;		
	}
}


// Prints the error message.
void	print_error(char *command, char *arg, char *error_message)
{
	g_status = 1;
	ft_putstr_fd("minishell: ", 2);
	if (command)
	{
		ft_putstr_fd(command, 2);
		ft_putstr_fd(": ", 2);
	}
	if (arg)
	{
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putstr_fd(error_message, 2);
	ft_putstr_fd("\n", 2);
}

// Prints the export list.
void	print_export(t_env_list **export_variables)
{
	t_env_list	*tmp;

	tmp = *export_variables;
	if (!tmp)
		return ;
	while (tmp)
	{
		ft_putstr_fd(tmp->variable, 1);
		write(1, "\n", 1);
		tmp = tmp->next;
	}
}
