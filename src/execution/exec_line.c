/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 17:50:58 by ymeziane          #+#    #+#             */
/*   Updated: 2024/02/24 12:18:46 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exec_cmd(char **bin_paths, char **args, t_data **data,
		t_token **tokenlist, bool first_command)
{
	char	*path_cmd;

	path_cmd = get_path_cmd(bin_paths, args[0]);
	if (type(args[0], (*data)->env) == BUILTIN)
	{
		state = 1;
		free(path_cmd);
		execute_bultin(tokenlist, data, args[0]);
		if (!first_command)
			exit(EXIT_SUCCESS);
	}
	else if (type(args[0], (*data)->env) == COMMAND)
	{
		state = 1;
		if (execve(path_cmd, args, env_list_to_array((*data)->env)) < 0)
		{
			perror("execve failed");
			exit(EXIT_FAILURE);
		}
	}
}

static void	run_pipe(char **bin_paths, char **argv, t_data **data,
		t_token **tokenlist)
{
	int	pipefd[2];
	int	f;

	if (pipe(pipefd) < 0)
		perror("pipe creation failed");
	f = fork();
	if (f == 0)
	{
		if (dup2(pipefd[1], STDOUT_FILENO) < 0)
			perror("dup2 failed");
		close(pipefd[0]);
	}
	else
	{
		if (dup2(pipefd[0], STDIN_FILENO) < 0)
			perror("dup2 failed");
		close(pipefd[1]);
		exec_cmd(bin_paths, argv, data, tokenlist, 0);
	}
}

static void	handle_pipes(char **args, char **bin_paths, t_data **data,
		t_token **tokenlist)
{
	int		i;
	size_t	nb_args;

	nb_args = count_args(args);
	i = nb_args - 1;
	while (i >= 1)
	{
		if (ft_strcmp(args[i], "|") == 0)
		{
			args[i] = NULL;
			run_pipe(bin_paths, &args[i + 1], data, tokenlist);
			nb_args = i;
		}
		i--;
	}
}

// Gère la création de tous les processus enfants pour les commandes
void	execute_line(t_token **tokenlist, t_data **data)
{
	char	**args;
	pid_t	pid;
	char	**bin_paths;

	bin_paths = find_bin_paths((*data)->env);
	state = 1;
	args = tokens_to_array(tokenlist);
	if (type(*args, (*data)->env) == BUILTIN && (*data)->nb_pipe == 0)
	{
		pid = -1;
		exec_cmd(bin_paths, args, data, tokenlist, 1);
	}
	else
		pid = fork();
	if (pid == 0)
	{
		handle_pipes(args, bin_paths, data, tokenlist);
		exec_cmd(bin_paths, args, data, tokenlist, 0);
	}
	while(wait(NULL) > 0);
	free_double_array(bin_paths);
}
  