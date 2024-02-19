/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 17:50:58 by ymeziane          #+#    #+#             */
/*   Updated: 2024/02/18 20:35:02 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//  

static void	child(char *cmd, int index_cmd, int nb_cmds, int *pipe_fd)
{
	pid_t	pid;

	(void)cmd;
	pid = fork();
	if (pid == -1)
	{
		ft_putstr_fd("Error:\nFork failed\n", 2);
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		// configure_io();
		// execute_cmd(cmd);
	}
	if (index_cmd != 1)
		close(pipe_fd[(index_cmd - 1) * 2]);
	if (index_cmd != nb_cmds - 1)
		close(pipe_fd[index_cmd * 2 + 1]);
}

void	children(int nb_pipe, t_token **tokenlist, int *pipe_fd)
{
	int		status;
	pid_t	pid_child;
    int nb_cmds;
	t_token	*tmp;
	int		i;

    nb_cmds = nb_pipe + 1;
	tmp = *tokenlist;
	i = 0;
	while (tmp)
	{
		if (tmp->ttype == COMMAND)
			child(tmp->element, i++, nb_cmds, pipe_fd);
		tmp = tmp->next;
	}
	pid_child = wait(&status);
	while (pid_child > 0)
		pid_child = wait(&status);
}

