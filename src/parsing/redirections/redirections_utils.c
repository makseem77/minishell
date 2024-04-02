/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 12:25:55 by ymeziane          #+#    #+#             */
/*   Updated: 2024/04/02 12:25:57 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_or_append_helper(t_token *tmp, t_token *command_token, t_data **data)
{
	if (command_token->fd_out > 1)
		close(command_token->fd_out);
	if (tmp->next)
		command_token->fd_out = open(tmp->next->element,
				O_CREAT | O_RDWR | O_APPEND, 0644);
	if ((*data)->invalid_file)
	{
		if (command_token->fd_out > 1)
			close(command_token->fd_out);
		return (command_token->fd_out = -1, 1);
	}
	return (0);
}

int	create_or_truncate_helper(t_token *tmp, t_token *command_token,
		t_data **data)
{
	if (command_token->fd_out > 1)
		close(command_token->fd_out);
	command_token->fd_out = open(tmp->next->element, O_CREAT | O_RDWR | O_TRUNC,
			0644);
	if ((*data)->invalid_file)
	{
		if (command_token->fd_out > 1)
			close(command_token->fd_out);
		return (command_token->fd_out = -1, 1);
	}
	return (0);
}

int	read_from_file_helper(t_token *tmp, t_token *command_token, t_data **data)
{
	if ((command_token && command_token->fd_in == -1))
	{
		if (command_token && command_token->fd_in == -1
			&& command_token->fd_out != -1)
		{
			return (print_error(NULL, tmp->next->element, strerror(errno)), 0);
			if (command_token->fd_out > 1)
				close(command_token->fd_out);
			command_token->fd_out = -1;
		}
		if ((*data)->invalid_file)
		{
			if (command_token->fd_out > 1)
				close(command_token->fd_out);
			return (command_token->fd_out = -1, 1);
		}
	}
	return (1);
}
