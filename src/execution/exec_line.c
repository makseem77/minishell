/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 12:24:09 by ymeziane          #+#    #+#             */
/*   Updated: 2024/03/17 12:24:19 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	check_and_exec_single_builtin(t_token **tokenlist, t_data **data,
		char **args)
{
	char	**expression;
	int		saved_stdout;
	int		saved_stdin;

	expression = args;
	if (type(expression[0], (*data)->env) == BUILTIN && (*data)->nb_pipe == 0)
	{
		saved_stdout = dup(STDOUT_FILENO);
		saved_stdin = dup(STDIN_FILENO);
		configure_io(tokenlist, 0, NULL, 0);
		execute_bultin(tokenlist, data, expression, args);
		dup2(saved_stdout, STDOUT_FILENO);
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdout);
		close(saved_stdin);
		return (true);
	}
	else
		return (false);
}

void	free_after_execution(t_token **tokenlist, t_data **data, int **fds,
		char **args, char **expression, char *path_cmd)
{
	free(path_cmd);
	free_double_array(expression);
	free_double_array(args);
	free_fds_array(fds, (*data)->nb_pipe);
	free_data_struct(*data);
	free_token_list(tokenlist);
}

void	exec(t_token **tokenlist, t_data **data, int index, int **fds,
		char **args)
{
	char	*path_cmd;
	pid_t	pid;
	char	**expression;

	if (check_and_exec_single_builtin(tokenlist, data, args))
		return ;
	expression = cut_arrays_into_expression(args, index);
	path_cmd = get_path_cmd((*data)->bin_paths, expression[0]);
	pid = fork();
	if (pid == 0)
	{
		configure_io(tokenlist, index, fds, (*data)->nb_pipe);
		if (type(expression[0], (*data)->env) == BUILTIN)
		{
			execute_bultin(tokenlist, data, expression, args);
			free_after_execution(tokenlist, data, fds, args, expression,
				path_cmd);
			exit(g_status);
		}
		if (type(expression[0], (*data)->env) == COMMAND)
		{
			execve(path_cmd, expression, env_list_to_array((*data)->env));
			g_status = 1;
			exit(g_status);
		}
		else
		{
			print_not_found(expression[0], NULL);
			free_after_execution(tokenlist, data, fds, args, expression,
				path_cmd);
			g_status = 127;
			exit(g_status);
		}
	}
	free_double_array(expression);
	free(path_cmd);
}

void	execute_line(t_token **tokenlist, t_data **data)
{
	int		i;
	int		**fds;
	char	**args;
	int		status;

	args = tokens_to_array(tokenlist);
	state = 1;
	fds = init_pipes(data);
	i = (*data)->nb_pipe;
	while (i >= 0)
	{
		exec(tokenlist, data, i, fds, args);
		i--;
	}
	i = (*data)->nb_pipe;
	while (i > 0)
	{
		close(fds[i - 1][1]);
		i--;
	}
	while (wait(&status) > 0)
		;
	if (!(type(args[0], (*data)->env) == BUILTIN && (*data)->nb_pipe == 0)
		&& g_status != 130)
		g_status = exited_status(status);
	close_all_pipes(fds, (*data)->nb_pipe);
	free_double_array(args);
	free_fds_array(fds, (*data)->nb_pipe);
	if ((*data)->here_doc)
		unlink("tmp");
	(*data)->nb_pipe = 0;
	state = 0;
}
