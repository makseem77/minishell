/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 15:47:33 by ymeziane          #+#    #+#             */
/*   Updated: 2024/03/26 12:10:39 by ymeziane         ###   ########.fr       */
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
		if (g_status == -1)
			while (++i < end)
				;
		else
		{
			ft_putstr_fd("\n", 1);
			rl_on_new_line();
			rl_replace_line("", 0);
			if (g_status == -2)
				execve("/bin/bash", (char *[]){"/bin/bash", "-c", "exit", "130",
					NULL}, NULL);
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
