/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 04:28:22 by maxborde          #+#    #+#             */
/*   Updated: 2024/03/21 11:41:46 by ymeziane         ###   ########.fr       */
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
	return (0);
}

int	configure_io(t_token **tokenlist, int index, int **fds, int nb_pipe)
{
	int	fd_out;
	int	fd_in;

	fd_out = get_output_fd(tokenlist, index);
	fd_in = get_input_fd(tokenlist, index);
	if (fd_in == -1 || fd_out == -1)
		return (0);
	if (index == 0 && nb_pipe > 0)
	{
		dup2(fds[0][1], STDOUT_FILENO);
		if (fd_in != -1)
			dup2(fd_in, STDIN_FILENO);
		if (fd_out != -1)
			dup2(fd_out, STDOUT_FILENO);
	}
	else if (index > 0 && index < nb_pipe)
	{
		dup2(fds[index - 1][0], STDIN_FILENO);
		dup2(fds[index][1], STDOUT_FILENO);
		if (fd_in != -1)
			dup2(fd_in, STDIN_FILENO);
		if (fd_out != -1)
			dup2(fd_out, STDOUT_FILENO);
	}
	else if (index == nb_pipe && nb_pipe > 0)
	{
		dup2(fds[nb_pipe - 1][0], STDIN_FILENO);
		if (fd_in != -1)
			dup2(fd_in, STDIN_FILENO);
		if (fd_out != -1)
			dup2(fd_out, STDOUT_FILENO);
	}
	else if (index == 0 && nb_pipe == 0)
	{
		dup2(fd_out, STDOUT_FILENO);
		if (fd_in != STDIN_FILENO && fd_in != -1)
			dup2(fd_in, STDIN_FILENO);
		if (fd_out != STDOUT_FILENO && fd_in != -1)
			dup2(fd_out, STDOUT_FILENO);
	}
	close_all_pipes(tokenlist, fds, nb_pipe);
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
	if (pid == 0)
	{
		g_status = -2;
		free_data_struct(*data);
		free_token_list(tokenlist);
		while (true)
		{
			line = readline("> ");
			if (!line)
			{
				close(fd);
				free(limiter_stored);
				exit(0);
			}
			if (ft_strcmp(line, limiter_stored) == 0)
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
		close(fd);
	}
	free(limiter_stored);
	close(fd);
	waitpid(pid, &status, 0);
	if(status)
		g_status = exited_status(status);
	else
		g_status = 0;
	if (command)
			return (open(".tmp", O_RDWR, 0644));
	else
		return (0);
}
