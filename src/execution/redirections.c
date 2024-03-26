/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 04:28:22 by maxborde          #+#    #+#             */
/*   Updated: 2024/03/26 11:55:14 by ymeziane         ###   ########.fr       */
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
		if (tmp->ttype == COMMAND || tmp->ttype == BUILTIN)
		{
			if (i == index)
				return (tmp->fd_in);
			else
				i++;
		}
		tmp = tmp->next;
	}
	return (-1);
}

int	configure_io(t_token **tokenlist, int index, t_data **data)
{
	int	fd_out;
	int	fd_in;

	fd_out = get_output_fd(tokenlist, index);
	fd_in = get_input_fd(tokenlist, index);
	// if (fd_in == -1 || fd_out == -1)
	// 	return (0);
	if (index == 0 && (*data)->nb_pipe > 0)
	{
		dup2((*data)->pipe_fds[0][1], STDOUT_FILENO);
		if (fd_in != -1)
			dup2(fd_in, STDIN_FILENO);
		if (fd_out != -1)
			dup2(fd_out, STDOUT_FILENO);
	}
	else if (index > 0 && index < (*data)->nb_pipe)
	{
		dup2((*data)->pipe_fds[index - 1][0], STDIN_FILENO);
		dup2((*data)->pipe_fds[index][1], STDOUT_FILENO);
		if (fd_in != -1)
			dup2(fd_in, STDIN_FILENO);
		if (fd_out != -1)
			dup2(fd_out, STDOUT_FILENO);
	}
	else if (index == (*data)->nb_pipe && (*data)->nb_pipe > 0)
	{
		dup2((*data)->pipe_fds[(*data)->nb_pipe - 1][0], STDIN_FILENO);
		if (fd_in != -1)
			dup2(fd_in, STDIN_FILENO);
		if (fd_out != -1)
			dup2(fd_out, STDOUT_FILENO);
	}
	else if (index == 0 && (*data)->nb_pipe == 0)
	{
		dup2(fd_out, STDOUT_FILENO);
		if (fd_in != STDIN_FILENO && fd_in != -1)
			dup2(fd_in, STDIN_FILENO);
		if (fd_out != STDOUT_FILENO && fd_in != -1)
			dup2(fd_out, STDOUT_FILENO);
	}
	close_all_pipes(tokenlist, (*data)->pipe_fds, (*data)->nb_pipe);
	return (1);
}

int	write_to_heredoc(int fd, char *limiter, bool command, t_data **data,
		t_token **tokenlist)
{
	char	*line;
	pid_t	pid;
	int		status;
	static char *  limiter_stored;

	g_status = -1;
	limiter_stored = ft_strdup(limiter);
	pid = fork();
	if (pid)
		signal(SIGINT, SIG_IGN);
	if (pid == 0)
	{
		g_status = -2;
		free_data_struct(*data);
		free_token_list(tokenlist);
		while (true)
		{
			line = readline("> ");
			if (!line || ft_strcmp(line, limiter_stored) == 0)
			{
				if(line)
					free(line);
				free(limiter_stored);
				close(fd);
				exit(0);
			}
			else
			{
				ft_putendl_fd(line, fd);
				free(line);
			}
		}
	}
	free(limiter_stored);
	close(fd);
	waitpid(pid, &status, 0);
	if(status)
		g_status = exited_status(status);
	else if(g_status != 130)
		g_status = 0;
	if (command)
			return (open(".tmp", O_RDWR, 0644));
	else
		return (0);
}
