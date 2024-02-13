/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 15:47:33 by ymeziane          #+#    #+#             */
/*   Updated: 2024/02/13 17:59:31 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sigint_handler(int sig)
{
	(void)sig;
    ft_putstr_fd("\n", 1);
	rl_on_new_line(); // Prépare readline pour une nouvelle ligne
	rl_replace_line("", 0); // Efface la ligne courante
	rl_redisplay(); // Réaffiche l'invite
}

void	handle_signals(t_token **token)
{
	struct sigaction sa_sigint;
	struct sigaction sa_sigquit;
    (void)token;

	// Configuration pour SIGINT
	sa_sigint.sa_handler = sigint_handler;
	sigemptyset(&sa_sigint.sa_mask);
	sa_sigint.sa_flags = 0;
	if (sigaction(SIGINT, &sa_sigint, NULL) == -1)
	{
		perror("Erreur lors de la configuration de SIGINT");
		exit(EXIT_FAILURE);
	}

	// Configuration pour SIGQUIT
	sa_sigquit.sa_handler = SIG_IGN; // Ignore le signal
	sigemptyset(&sa_sigquit.sa_mask);
	sa_sigquit.sa_flags = 0;
	if (sigaction(SIGQUIT, &sa_sigquit, NULL) == -1)
	{
		perror("Erreur lors de la configuration de SIGQUIT");
		exit(EXIT_FAILURE);
	}
}