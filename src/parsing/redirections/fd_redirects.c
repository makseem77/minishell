/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_redirects.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 15:23:35 by ymeziane          #+#    #+#             */
/*   Updated: 2024/03/25 18:52:40 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	create_or_append(t_token *tmp, t_token *command_token)
{
	int	fd;

	fd = -1;
	if (command_token)
	{
		if (command_token->fd_out > 1)
			close(command_token->fd_out);
		command_token->fd_out = open(tmp->next->element,
				O_CREAT | O_RDWR | O_APPEND, 0644);
	}
	else
		fd = open(tmp->next->element, O_CREAT | O_RDWR | O_APPEND, 0644);
	if (fd != -1)
		close(fd);
	if (command_token)
	{
		if (command_token->fd_out == -1)
			print_error(NULL, tmp->next->element, strerror(errno));
	}
	return (-1);
}

int	create_or_truncate(t_token *tmp, t_token *command_token)
{
	int	fd;

	fd = -1;
	if (command_token)
	{
		if (command_token->fd_out > 1)
			close(command_token->fd_out);
		command_token->fd_out = open(tmp->next->element,
				O_CREAT | O_RDWR | O_TRUNC, 0644);
	}
	else
		fd = open(tmp->next->element, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd != -1)
		close(fd);
	if (command_token)
	{
		if (command_token->fd_out == -1)
			print_error(NULL, tmp->next->element, strerror(errno));
	}
	return (-1);
}

void	create_and_read_from_heredoc(t_token *tmp, t_token *command_token,
		t_data **data, t_token **tokenlist)
{
	int	fd;

	fd = 0;
	if (!command_token)
		fd = write_to_heredoc(open(".tmp", O_CREAT | O_RDWR | O_TRUNC, 0644),
				tmp->next->element, false, data, tokenlist);
	else
	{
		if (command_token->fd_in > 1)
			close(command_token->fd_in);
		command_token->fd_in = open(".tmp", O_CREAT | O_RDWR | O_TRUNC, 0644);
		fd = write_to_heredoc(command_token->fd_in, tmp->next->element, true,
				data, tokenlist);
	}
	if ((command_token && command_token->fd_in == -1) || fd == -1)
	{
		if (command_token && command_token->fd_in == -1 && command_token->fd_out != -1)
		{
			print_error(NULL, tmp->next->element, strerror(errno));
			command_token->fd_out = -1;
		}
	}
}

void	read_from_file(t_token *tmp, t_token *command_token)
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
		print_error(NULL, tmp->next->element, "No such file or directory");
	}
	if ((command_token && command_token->fd_in == -1) || fd == -1)
	{
		if (command_token && command_token->fd_in == -1 && command_token->fd_out != -1)
		{
			print_error(NULL, tmp->next->element, strerror(errno));
			command_token->fd_out = -1;
		}
	}
}
