/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 15:47:33 by ymeziane          #+#    #+#             */
/*   Updated: 2024/03/21 10:37:02 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		//printf("g_status = %d\n", g_status);
		if (g_status == -1)
		{
			while (++i < end);
			ft_putstr_fd("\n", 1);
		}
		else if(g_status == -2)
		{
			//printf("CTRL C IN HERE DOC PROCESS -> EXIT HERE DOC PROCESS");
			ft_putstr_fd("\n", 1);
			exit(130);
		}
		else
		{
			ft_putstr_fd("\n", 1);
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
		}
		g_status = 130;
	}
	// printf("END OF HANDLE SIGNALS\n");
}

void	init_signals(void)
{
	signal(SIGINT, handle_signals);
	signal(SIGQUIT, handle_signals);
}
