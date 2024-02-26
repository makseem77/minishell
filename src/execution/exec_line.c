/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 17:50:58 by ymeziane          #+#    #+#             */
/*   Updated: 2024/02/26 02:38:18 by maxborde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exec_cmd(char **bin_paths, char **args, t_data **data,
		t_token **tokenlist, bool only_cmd)
{
	char	*path_cmd;

	path_cmd = get_path_cmd(bin_paths, args[0]);
	if (type(args[0], (*data)->env) == BUILTIN)
	{
		state = 1;
		free(path_cmd);
		execute_bultin(tokenlist, data, args[0]);
		if (!only_cmd)
		{
			//printf("EXIT BI\n");
			exit(EXIT_SUCCESS);
		}
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
	else if(type(args[0], (*data)->env) == -1)
	{
		print_error(args[0], NULL, "command not found");
		exit(127);
	}
}

static void	run_pipe(char **bin_paths, char **argv, t_data **data,
		t_token **tokenlist)
{
	int	pipefd[2];
	pid_t	pid;

	if (pipe(pipefd) < 0)
		perror("pipe creation failed");
	pid = fork();
	if (pid == 0)
	{
		//printf("JUST AFTER FORK IN RUN PIPE: PID = %ld PPID = %ld\n\n\n", (long)getpid(), (long)getppid());
		if (dup2(pipefd[1], STDOUT_FILENO) < 0)
			perror("dup2 failed");
	}
	else
	{
		if (dup2(pipefd[0], STDIN_FILENO) < 0)
			perror("dup2 failed");
		close(pipefd[1]);
		printf("JUST BEFORE EXEC CMD = %s, IN RUN PIPE: PID = %ld PPID = %ld\n\n\n", *argv, (long)getpid(), (long)getppid());
		int w = wait(&pid);
		printf("%d\n", w);
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
	while (i >= 0)
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
	//printf("ARG = %s\n", *args);
	if (type(*args, (*data)->env) == BUILTIN && (*data)->nb_pipe == 0)
	{
		pid = -1;
		//printf("ONLY CMD\n");
		exec_cmd(bin_paths, args, data, tokenlist, 1);
	}
	else
		pid = fork();
	//printf("JUST AFTER FORK: PID = %ld PPID = %ld\n\n\n", (long)getpid(), (long)getppid());
	if (pid == 0)
	{
		//printf("JUST AFTER FORK IN EXEC LINE: PID = %ld PPID = %ld\n\n\n", (long)getpid(), (long)getppid());
		handle_pipes(args, bin_paths, data, tokenlist);
		//printf("JUST BEFORE EXEC CMD = %s IN EXEC LINE: PID = %ld PPID = %ld\n\n\n", *args, (long)getpid(), (long)getppid());
		printf("ARG IN MAIN = %s\n", *args);
		exec_cmd(bin_paths, args, data, tokenlist, 0);
		exit(EXIT_SUCCESS);
	}
	//printf("I'M WAITING THE PID = %ld\n", (long)pid);
	wait(NULL);
	//while((waitpid(pid, NULL, 0)) > 0);
	//printf("JUST AFTER WAIT\n");
	//printf("I CONTINUE\n");
	free_double_array(bin_paths);
	free(args);
	(*data)->nb_pipe = 0;
}
  
