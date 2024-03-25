/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 12:24:09 by ymeziane          #+#    #+#             */
/*   Updated: 2024/03/25 18:54:02 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	check_and_exec_single_builtin(t_token **tokenlist, t_data **data,
		char **args)
{
	char	**expression;
	// int		saved_stdout;
	// int		saved_stdin;

	expression = args;
	if (type(expression[0], (*data)->env) == BUILTIN && (*data)->nb_pipe == 0)
	{
		// saved_stdout = dup(STDOUT_FILENO);
		// saved_stdin = dup(STDIN_FILENO);
		if (configure_io(tokenlist, 0, data))
		{
			execute_bultin(tokenlist, data, expression, args);
			// dup2(saved_stdout, STDOUT_FILENO);
			// dup2(saved_stdin, STDIN_FILENO);
			// close(saved_stdout);
			// close(saved_stdin);
		}
		return (true);
	}
	else
		return (false);
}

void	free_after_execution(t_token **tokenlist, t_data **data, char **args, char **expression)
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

void	exec_builtin(t_token **tokenlist, t_data **data, char **expression, char **args)
{
	execute_bultin(tokenlist, data, expression, args);
	free_after_execution(tokenlist, data, args, expression);
	exit(g_status);
}

void	process_invalid(t_token **tokenlist, t_data **data, char **expression, char **args)
{
	print_not_found(expression[0], NULL);
	free_after_execution(tokenlist, data, args, expression);
	exit(g_status);
}

void	process_empty(t_token **tokenlist, t_data **data, char **expression, char **args)
{
	free_after_execution(tokenlist, data, args, expression);
	g_status = 0;
	exit(g_status);
}

bool is_minishell(char *cmd)
{
	if(access(cmd, X_OK) == 0 && ft_strcmp(cmd + (ft_strlen(cmd) - ft_strlen("/minishell")), "/minishell") == 0)
		return (true);
	return (false);
}

void	exec_expression(t_token **tokenlist, t_data **data, int index, char **args)
{
	pid_t	pid;
	char	**expression;

	if (check_and_exec_single_builtin(tokenlist, data, args))
		return ;
	expression = cut_arrays_into_expression(args, index);
	free((*data)->path_cmd);
	(*data)->path_cmd = get_path_cmd((*data)->bin_paths, expression[0]);
	pid = fork();
	if (pid && is_minishell(expression[0]))
		signal(SIGINT, SIG_IGN);
	if (pid == 0)
	{
		if (configure_io(tokenlist, index, data))
		{
			if (type(expression[0], (*data)->env) == BUILTIN)
				exec_builtin(tokenlist, data, expression, args);
			else if (type(expression[0], (*data)->env) == COMMAND)
				exec_command(data, expression, env_list_to_array((*data)->env));
			else if (type(expression[0], (*data)->env) == -1)
				process_invalid(tokenlist, data, expression, args);
			else if ((*data)->path_cmd == NULL)
				process_empty(tokenlist, data, expression, args);
		}
		else
			free_after_execution(tokenlist, data, args, expression);
		g_status = 1;
		exit(g_status);
	}
	free_double_array(expression);
}

void	execute_line(t_token **tokenlist, t_data **data)
{
	int		i;
	char	**args;
	int		status;

	if (!(g_status == 130 && (*data)->here_doc == true))
	{
		args = tokens_to_array(tokenlist);
		g_status = -1;
		(*data)->pipe_fds = init_pipes(data);
		i = (*data)->nb_pipe;
		while (i >= 0)
		{
			exec_expression(tokenlist, data, i, args);
			i--;
		}
		i = (*data)->nb_pipe;
		while (i > 0)
		{
			close((*data)->pipe_fds[i - 1][1]);
			i--;
		}
		while (wait(&status) > 0)
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
