/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_redirects.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 15:23:35 by ymeziane          #+#    #+#             */
/*   Updated: 2024/04/02 12:46:04 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	create_or_append(t_token *tmp, t_token *command_token, t_data **data)
{
	int	fd;

	fd = -1;
	if (command_token)
	{
		if (create_or_append_helper(tmp, command_token, data))
			return (1);
	}
	else
	{
		fd = open(tmp->next->element, O_CREAT | O_RDWR | O_APPEND, 0644);
		if (fd != -1)
			close(fd);
		else
			return (print_error(NULL, tmp->next->element, strerror(errno)),
				g_status = 1, 0);
	}
	if (command_token && command_token->fd_out == -1)
		return (print_error(NULL, tmp->next->element, strerror(errno)),
			g_status = 1, 0);
	return (1);
}

int	create_or_truncate(t_token *tmp, t_token *command_token, t_data **data)
{
	int	fd;

	fd = -1;
	if (command_token)
	{
		if (create_or_truncate_helper(tmp, command_token, data))
			return (1);
	}
	else
	{
		fd = open(tmp->next->element, O_CREAT | O_RDWR | O_TRUNC, 0644);
		if (fd != -1)
			close(fd);
		else
			return (print_error(NULL, tmp->next->element, strerror(errno)),
				g_status = 1, 0);
	}
	if (command_token && command_token->fd_out == -1)
		return (print_error(NULL, tmp->next->element, strerror(errno)),
			g_status = 1, 0);
	return (1);
}

void	create_and_read_from_heredoc(t_token *tmp, t_token *command_token,
		t_data **data, t_token **tokenlist)
{
	int	fd;

	fd = open(".tmp", O_CREAT | O_RDWR | O_TRUNC, 0644);
	set_up_heredoc(fd, tmp->next->element);
	if (!command_token)
		fd = write_to_heredoc(false, data, tokenlist);
	else
	{
		if (command_token->fd_in > 1)
			close(command_token->fd_in);
		command_token->fd_in = fd;
		fd = write_to_heredoc(true, data, tokenlist);
	}
	if ((command_token && command_token->fd_in == -1) || fd == -1)
	{
		if (command_token && command_token->fd_in == -1
			&& command_token->fd_out != -1)
		{
			print_error(NULL, tmp->next->element, strerror(errno));
			command_token->fd_out = -1;
		}
	}
}

int	read_from_file(t_token *tmp, t_token *command_token, t_data **data)
{
	int	fd;

	fd = 0;
	if (command_token)
	{
		if (command_token->fd_in > 1)
			close(command_token->fd_in);
		command_token->fd_in = open(tmp->next->element, O_RDWR, 0644);
	}
	else
	{
		if (tmp->next->next)
			tmp->next->next->fd_in = -1;
		fd = open(tmp->next->element, O_RDWR, 0644);
		if (fd == -1)
			return (print_error(NULL, tmp->next->element, strerror(errno)), 0);
		else
			close(fd);
	}
	return (read_from_file_helper(tmp, command_token, data));
}
