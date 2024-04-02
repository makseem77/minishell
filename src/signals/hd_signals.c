/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hd_signals.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 12:57:05 by ymeziane          #+#    #+#             */
/*   Updated: 2024/04/02 13:43:56 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_heredoc_handler	g_heredoc_struct;

static void	free_in_hd(t_heredoc_handler g_heredoc_struct)
{
	free(g_heredoc_struct.g_limiter_stored);
	if (g_heredoc_struct.g_fd_hd != -1)
		close(g_heredoc_struct.g_fd_hd);
}

void	handle_sigint(int signal)
{
	int	i;
	int	end;

	i = 0;
	end = 70000;
	if (signal == SIGINT)
	{
		if (g_status == -1)
			while (++i < end)
				;
		else
		{
			ft_putstr_fd("\n", 1);
			rl_on_new_line();
			rl_replace_line("", 0);
			if (g_status == -2)
				return (free_in_hd(g_heredoc_struct), exit(130));
			else
				rl_redisplay();
		}
		if (g_status == -1 || g_status == -2)
			ft_putstr_fd("\n", 1);
		g_status = 130;
	}
}

void	set_up_heredoc(int fd, char *limiter)
{
	g_heredoc_struct.g_limiter_stored = ft_strdup(limiter);
	g_heredoc_struct.g_fd_hd = fd;
	g_status = -1;
}

static void	heredoc_loop(t_data **data, t_token **tokenlist)
{
	char	*line;

	free_data_struct(*data);
	close_all_pipes(tokenlist, NULL, 0);
	free_token_list(tokenlist);
	while (true)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, g_heredoc_struct.g_limiter_stored) == 0)
		{
			if (line)
				free(line);
			else
				ft_putstr_fd("\n", 1);
			free_in_hd(g_heredoc_struct);
			exit(0);
		}
		else
		{
			ft_putendl_fd(line, g_heredoc_struct.g_fd_hd);
			free(line);
		}
	}
}

int	write_to_heredoc(bool command, t_data **data, t_token **tokenlist)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid)
		signal(SIGINT, SIG_IGN);
	if (pid == 0)
	{
		g_status = -2;
		heredoc_loop(data, tokenlist);
	}
	free_in_hd(g_heredoc_struct);
	waitpid(pid, &status, 0);
	if (status)
		g_status = exited_status(status);
	else if (g_status != 130)
		g_status = 0;
	if (command)
		return (open(".tmp", O_RDWR, 0644));
	else
		return (0);
}
