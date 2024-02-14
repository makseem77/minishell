/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 15:47:33 by ymeziane          #+#    #+#             */
/*   Updated: 2024/02/14 11:02:53 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sigint_handler(int sig)
{
	(void)sig;
	ft_putstr_fd("\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handle_signals(t_token **token)
{
	struct sigaction	sa_sigint;
	struct sigaction	sa_sigquit;

	(void)token;
	sa_sigint.sa_handler = sigint_handler;
	sigemptyset(&sa_sigint.sa_mask);
	sa_sigint.sa_flags = 0;
	if (sigaction(SIGINT, &sa_sigint, NULL) == -1)
	{
		perror("Erreur lors de la configuration de SIGINT");
		exit(EXIT_FAILURE);
	}
	sa_sigquit.sa_handler = SIG_IGN;
	sigemptyset(&sa_sigquit.sa_mask);
	sa_sigquit.sa_flags = 0;
	if (sigaction(SIGQUIT, &sa_sigquit, NULL) == -1)
	{
		perror("Erreur lors de la configuration de SIGQUIT");
		exit(EXIT_FAILURE);
	}
}
