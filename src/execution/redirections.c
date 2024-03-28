/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 04:28:22 by maxborde          #+#    #+#             */
/*   Updated: 2024/03/27 10:23:37 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_output_fd(t_token **tokenlist, int index)
{
	t_token	*tmp;
	int		i;

	tmp = *tokenlist;
	i = 0;
	while (tmp)
	{
		if (tmp->ttype == COMMAND || tmp->ttype == BUILTIN)
		{
			if (i == index)
				return (tmp->fd_out);
			else
				i++;
		}
		tmp = tmp->next;
	}
	return (1);
}

int	get_input_fd(t_token **tokenlist, int index)
{
	t_token	*tmp;
	int		i;

	tmp = *tokenlist;
	i = 0;
	while (tmp)
	{
		if (tmp->ttype == COMMAND || tmp->ttype == BUILTIN || tmp->fd_in == -1)
		{
			if (i == index)
				return (tmp->fd_in);
			else
				i++;
		}
		tmp = tmp->next;
	}
	return (0);
}

void	configure_io_helper(int *fd_in_and_out, t_data **data, int index
		, int cases)
{
	if (cases == 0)
	{
		dup2((*data)->pipe_fds[0][1], STDOUT_FILENO);
		dup2(fd_in_and_out[0], STDIN_FILENO);
		dup2(fd_in_and_out[1], STDOUT_FILENO);
	}
	else if (cases == 1)
	{
		dup2((*data)->pipe_fds[index - 1][0], STDIN_FILENO);
		dup2((*data)->pipe_fds[index][1], STDOUT_FILENO);
		dup2(fd_in_and_out[0], STDIN_FILENO);
		dup2(fd_in_and_out[1], STDOUT_FILENO);
	}
	else if (cases == 2)
	{
		dup2((*data)->pipe_fds[(*data)->nb_pipe - 1][0], STDIN_FILENO);
		dup2(fd_in_and_out[0], STDIN_FILENO);
		dup2(fd_in_and_out[1], STDOUT_FILENO);
	}
	else if (cases == 3)
	{
		dup2(fd_in_and_out[1], STDOUT_FILENO);
		dup2(fd_in_and_out[0], STDIN_FILENO);
		dup2(fd_in_and_out[1], STDOUT_FILENO);
	}
}

int	configure_io(t_token **tokenlist, int index, t_data **data)
{
	int	*fd_in_and_out;

	fd_in_and_out = malloc(sizeof(int) * 2);
	fd_in_and_out[0] = get_input_fd(tokenlist, index);
	fd_in_and_out[1] = get_output_fd(tokenlist, index);
	if (fd_in_and_out[0] == -1 || fd_in_and_out[1] == -1)
	{
		free(fd_in_and_out);
		return (0);
	}
	if (index == 0 && (*data)->nb_pipe > 0)
		configure_io_helper(fd_in_and_out, data, index, 0);
	else if (index > 0 && index < (*data)->nb_pipe)
		configure_io_helper(fd_in_and_out, data, index, 1);
	else if (index == (*data)->nb_pipe && (*data)->nb_pipe > 0)
		configure_io_helper(fd_in_and_out, data, index, 2);
	else if (index == 0 && (*data)->nb_pipe == 0)
		configure_io_helper(fd_in_and_out, data, index, 3);
	close_all_pipes(tokenlist, (*data)->pipe_fds, (*data)->nb_pipe);
	free(fd_in_and_out);
	return (1);
}
