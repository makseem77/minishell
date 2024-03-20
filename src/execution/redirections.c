/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 04:28:22 by maxborde          #+#    #+#             */
/*   Updated: 2024/03/19 14:14:04 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_output_fd(t_token **tokenlist, int index)
{
	t_token	*tmp;
	int	i;

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
	int	i;

	tmp = *tokenlist;
	i = 0;
	while (tmp)
	{
		if (tmp->ttype == COMMAND || tmp->ttype == BUILTIN)
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

void    configure_io(t_token **tokenlist, int index, int **fds, int nb_pipe)
{
	int	fd_out;
	int	fd_in;

	fd_out = get_output_fd(tokenlist, index);
	fd_in = get_input_fd(tokenlist, index);

	// printf("fd_out = %d\n", fd_out);
	if (index == 0 && nb_pipe > 0)
	{
		dup2(fds[0][1], STDOUT_FILENO);
		if (fd_in)
			dup2(fd_in, STDIN_FILENO);
		if (fd_out != -1)
			dup2(fd_out, STDOUT_FILENO);
	}
	else if (index > 0 && index < nb_pipe)
	{
		dup2(fds[index - 1][0], STDIN_FILENO);
		dup2(fds[index][1], STDOUT_FILENO);
		if (fd_in)
			dup2(fd_in, STDIN_FILENO);
		if (fd_out != -1)
			dup2(fd_out, STDOUT_FILENO);
	}
	else if (index == nb_pipe && nb_pipe > 0)
	{
		dup2(fds[nb_pipe - 1][0], STDIN_FILENO);
		if (fd_in)
			dup2(fd_in, STDIN_FILENO);
		if (fd_out != -1)
			dup2(fd_out, STDOUT_FILENO);
	}	
	else if (index == 0 && nb_pipe == 0)
	{
		dup2(fd_out, STDOUT_FILENO);
		if (fd_in != STDIN_FILENO)
			dup2(fd_in, STDIN_FILENO);
		if (fd_out != STDOUT_FILENO)
		{
			dup2(fd_out, STDOUT_FILENO);
		}
	}
	close_all_pipes(fds, nb_pipe);
}

int	write_to_heredoc(int fd, char *limiter)
{
	char	*line;
	pid_t	pid;

	pid = fork();
	g_status = -3;
	if (pid == 0)
	{
		g_status = -2;
		while (true)
		{
			line = readline("> ");
			if (!line)
			{	
				ft_putstr_fd("\n", 1);
				exit(1);
			}
			if (ft_strcmp(line, limiter) == 0)
			{
				free(line);
				break ;
			}
			else
			{
				ft_putendl_fd(line, fd);
				free(line);
			}
		}
		//NEED FREE
		close(fd);
		exit(0);
	}
	waitpid(pid, &g_status, 0);
	close(fd);
	return (open(".tmp", O_RDWR, 0644));
}
