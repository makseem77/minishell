/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 17:56:02 by ymeziane          #+#    #+#             */
/*   Updated: 2024/04/02 12:36:32 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Prints the error message.
void	print_not_found(char *command, char *arg)
{
	int	commandlen;

	if (command)
		commandlen = ft_strlen(command);
	else
		commandlen = 0;
	if (command && commandlen && (command[0] == '/' || command[0] == '.'
			|| command[commandlen] == '/'))
	{
		g_status = 126;
		if (access(command, F_OK) == 0 && access(command, X_OK) == -1)
			print_error(command, arg, "Permission denied");
		else if (access(command, F_OK) == -1)
		{
			g_status = 127;
			print_error(command, arg, "No such file or directory");
		}
		else
			print_error(command, arg, "Is a directory");
	}
	else if (command)
	{
		print_error(command, arg, "command not found");
		g_status = 127;
	}
}

// Prints the error message.
void	print_error(char *command, char *arg, char *error_message)
{
	if (ft_strcmp(error_message, "numeric argument required") != 0
		&& g_status != 127 && g_status != 126)
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

bool	is_a_redir_operator(char *element)
{
	return ((ft_strncmp(element, "<", 1) == 0) || (ft_strncmp(element, "<<",
				2) == 0) || (ft_strncmp(element, ">", 1) == 0)
		|| (ft_strncmp(element, ">>", 2) == 0));
}

int	error_syntax(t_token *tmp, int *nb_pipe)
{
	if (ft_strcmp(tmp->element, ";") == 0 || ft_strcmp(tmp->element, "\\") == 0)
		return (print_error(NULL, NULL,
				"special character ';' or '\\' is not authorized"), 1);
	else if (ft_strcmp(tmp->element, "|") == 0)
	{
		if (!tmp->next || ft_strcmp(tmp->next->element, "|") == 0)
			return (print_error(NULL, NULL,
					"pipe should be followed by a command"), 1);
		else
			(*nb_pipe)++;
	}
	else if (is_a_redir_operator(tmp->element) && !tmp->next)
		return (ft_putstr_fd("minishell: syntax error near unexpected token"
				" `newline'\n", 2), 1);
	else if (is_a_redir_operator(tmp->element)
		&& (ft_strncmp(tmp->next->element, ">", 1) == 0
			|| ft_strncmp(tmp->next->element, "<", 1) == 0))
		return (print_error(NULL, NULL,
				"minishell: syntax error near unexpected token > or <"), 1);
	return (0);
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
