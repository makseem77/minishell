/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 15:47:33 by ymeziane          #+#    #+#             */
/*   Updated: 2024/03/26 16:31:41 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static char *  limiter_stored;
static int fd_hd;

#include "minishell.h"

int	exited_status(int status)
{
	int	exit_status;

	if (WIFEXITED(status))
		exit_status = WEXITSTATUS(status);
	if (WIFSIGNALED(status))
	{
		exit_status = WTERMSIG(status);
		if (exit_status != 131)
			exit_status += 128;
	}
	return (exit_status);
}

static void	handle_signals(int signal)
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
			{
				close(fd_hd);
				free(limiter_stored);
				exit(130);
			}
			else
				rl_redisplay();
		}
		if (g_status == -1 || g_status == -2)
			ft_putstr_fd("\n", 1);
		g_status = 130;
	}
}

void	init_signals(void)
{
	signal(SIGINT, handle_signals);
	signal(SIGQUIT, handle_signals);
}

int	write_to_heredoc(int fd, char *limiter, bool command, t_data **data,
		t_token **tokenlist)
{
	char	*line;
	pid_t	pid;
	int		status;

	g_status = -1;
	limiter_stored = ft_strdup(limiter);
	fd_hd = fd;
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
