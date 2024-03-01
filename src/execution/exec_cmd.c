/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 13:21:20 by ymeziane          #+#    #+#             */
/*   Updated: 2024/03/01 16:19:40 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	configure_io(int index, int **fds, t_data **data)
{
	if (index == 0)
		dup2(fds[index][1], STDOUT_FILENO);
	else if (index == (*data)->nb_pipe)
		dup2(fds[index - 1][0], STDIN_FILENO);
	else
	{
		dup2(fds[index - 1][0], STDIN_FILENO);
		dup2(fds[index][1], STDOUT_FILENO);
	}
}

void	exec_cmd(t_data **data, int index, int **fds, char **expression)
{
	char **bin_paths;
	char *path_cmd;
	pid_t pid;
	// int      status;

	bin_paths = find_bin_paths((*data)->env);
	// for (int i = 0; args[i]; i++)
	// 	printf("args[%d] = %s\n", i, args[i]);
	// for (int i = 0; expression[i]; i++)
	// 	printf("expression[%d] = %s\n", i, expression[i]);
	path_cmd = get_path_cmd(bin_paths, expression[0]);
	if (!path_cmd)
		print_error(expression[0], NULL, "command not found");
	pid = fork();
	if (pid == 0)
	{
		if ((*data)->nb_pipe > 0)
			configure_io(index, fds, data);
		close_all_pipes(fds, (*data)->nb_pipe);
		if (execve(path_cmd, expression, env_list_to_array((*data)->env)) == -1)
			exit(1);
	}
	// else
	//     waitpid(pid, &status, 0);
}