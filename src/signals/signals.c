/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 15:47:33 by ymeziane          #+#    #+#             */
/*   Updated: 2024/04/02 14:05:51 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//Here we handle the sigquit signal like so:
//If the status is not -1 (meaning if we are not in the process of a bash command)
//and the status code has been set to 0, 1 or anything, we handle sigquit by writing
//a message and redisplaying a  prompt.
void	handle_sigquit(int sig)
{
	if (sig == SIGQUIT && g_status == -1)
	{
		ft_putstr_fd("Quit (core dumped)\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		g_status = 131;
	}
}

//This function goal is to get the right exit status after the end of a command.
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

//This function initializes the siginte signal to a custom function and the sigquit signal to be ignored (SIG_IGN).
void	init_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}
