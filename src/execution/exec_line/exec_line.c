/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 12:24:09 by ymeziane          #+#    #+#             */
/*   Updated: 2024/03/28 12:54:37 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_after_execution(t_token **tokenlist, t_data **data, char **args,
		char **expression)
{
	free_double_array(expression);
	free_double_array(args);
	free_fds_array((*data)->pipe_fds, (*data)->nb_pipe);
	free_data_struct(*data);
	free_token_list(tokenlist);
}

static pid_t	exec_all_cmds(t_token **tokenlist, t_data **data, char **args)
{
	int		i;
	pid_t	right_pid;

	i = (*data)->nb_pipe;
	while (i >= 0)
	{
		if ((*data)->nb_pipe == i)
			right_pid = exec_expression(tokenlist, data, i, args);
		else
			exec_expression(tokenlist, data, i, args);
		i--;
	}
	return (right_pid);
}

void	close_all_write_pipes(t_data **data)
{
	int	i;

	i = (*data)->nb_pipe;
	while (i > 0)
	{
		close((*data)->pipe_fds[i - 1][1]);
		i--;
	}
}

void	execute_line(t_token **tokenlist, t_data **data)
{
	char	**args;
	int		status;
	pid_t	right_pid;

	if (!(g_status == 130 && (*data)->here_doc == true))
	{
		args = tokens_to_array(tokenlist);
		g_status = -1;
		(*data)->pipe_fds = init_pipes(data);
		right_pid = exec_all_cmds(tokenlist, data, args);
		close_all_write_pipes(data);
		waitpid(right_pid, &status, 0);
		while (wait(NULL) > 0)
			;
		if (!(type(args[0], (*data)->env) == BUILTIN && (*data)->nb_pipe == 0)
			&& g_status != 130)
			g_status = exited_status(status);
		close_all_pipes(tokenlist, (*data)->pipe_fds, (*data)->nb_pipe);
		free_double_array(args);
		free_fds_array((*data)->pipe_fds, (*data)->nb_pipe);
	}
	(*data)->nb_pipe = 0;
}
