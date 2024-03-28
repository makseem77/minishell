/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 12:24:09 by ymeziane          #+#    #+#             */
/*   Updated: 2024/03/28 08:53:31 by ymeziane         ###   ########.fr       */
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
		if (ft_strcmp(expression[0], "exit") != 0)
		{
			saved_stdout = dup(STDOUT_FILENO);
			saved_stdin = dup(STDIN_FILENO);
		}
		if (configure_io(tokenlist, 0, data))
		{
			exec_builtins(tokenlist, data, expression, args);
			if (ft_strcmp(expression[0], "exit") != 0)
			{
				dup2(saved_stdout, STDOUT_FILENO);
				dup2(saved_stdin, STDIN_FILENO);
				close(saved_stdout);
				close(saved_stdin);
			}
		}
		else
			g_status = 1;
		return (true);
	}
	else
		return (false);
}

void	free_after_execution(t_token **tokenlist, t_data **data, char **args,
		char **expression)
{
	free_double_array(expression);
	free_double_array(args);
	free_fds_array((*data)->pipe_fds, (*data)->nb_pipe);
	free_data_struct(*data);
	free_token_list(tokenlist);
}

void	exec_command(t_data **data, char **expression, char **env)
{
	char	*path_cmd;

	path_cmd = ft_strdup((*data)->path_cmd);
	free_data_struct(*data);
	execve(path_cmd, expression, env);
	g_status = 1;
	exit(g_status);
}

void	process_invalid(t_token **tokenlist, t_data **data, char **expression,
		char **args)
{
	print_not_found(expression[0], NULL);
	free_after_execution(tokenlist, data, args, expression);
	exit(g_status);
}

bool	is_minishell(char *cmd)
{
	if (!cmd || ft_strlen(cmd) < ft_strlen("/minishell"))
		return (false);
	if (access(cmd, X_OK) == 0 && ft_strcmp(cmd + (ft_strlen(cmd)
				- ft_strlen("/minishell")), "/minishell") == 0)
		return (true);
	return (false);
}

pid_t	exec_expression(t_token **tokenlist, t_data **data, int index,
		char **args)
{
	pid_t	pid;
	pid_t	pid_right;
	char	**expression;

	if (check_and_exec_single_builtin(tokenlist, data, args))
		return 0;
	expression = cut_arrays_into_expression(args, index);
	free((*data)->path_cmd);
	(*data)->path_cmd = get_path_cmd((*data)->bin_paths, expression[0]);
	pid = fork();
	if((*data)->nb_pipe == index)
		pid_right = pid;
	if (pid && is_minishell(expression[0]))
		signal(SIGINT, SIG_IGN);
	if (pid == 0)
	{
		if (configure_io(tokenlist, index, data))
		{
			if (type(expression[0], (*data)->env) == BUILTIN)
				exec_builtins(tokenlist, data, expression, args);
			else if (type(expression[0], (*data)->env) == COMMAND)
				exec_command(data, expression, env_list_to_array((*data)->env));
			else if (type(expression[0], (*data)->env) == -1)
				process_invalid(tokenlist, data, expression, args);
		}
		else
		{
			close_all_pipes(tokenlist, (*data)->pipe_fds, (*data)->nb_pipe);
			free_after_execution(tokenlist, data, args, expression);
		}
		g_status = 1;
		exit(g_status);
	}
	free_double_array(expression);
	return (pid_right);
}

void	execute_line(t_token **tokenlist, t_data **data)
{
	int		i;
	char	**args;
	int		status;
	pid_t	right_pid;

	if (!(g_status == 130 && (*data)->here_doc == true))
	{
		args = tokens_to_array(tokenlist);
		g_status = -1;
		(*data)->pipe_fds = init_pipes(data);
		i = (*data)->nb_pipe;
		while (i >= 0)
		{
			if ((*data)->nb_pipe == i)
				right_pid = exec_expression(tokenlist, data, i, args);
			else
				exec_expression(tokenlist, data, i, args);	
			i--;
		}
		i = (*data)->nb_pipe;
		while (i > 0)
		{
			close((*data)->pipe_fds[i - 1][1]);
			i--;
		}
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
