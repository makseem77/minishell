/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:43:34 by ymeziane          #+#    #+#             */
/*   Updated: 2024/03/18 15:50:07 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		g_status;

// The main loop of the program
// It reads the input from the user and processes it
// It will keep reading the input until the user exits the program
void	listening_loop(t_data **data)
{
	char	*line;
	t_token	**tokenlist;

	tokenlist = NULL;
	handle_signals();
	g_status = 1;
	while (true)
	{
		line = readline("minishell> ");
		if (!line)
			exit_bash(EXIT_SUCCESS, data, NULL, NULL);
		if (*line)
			add_history(line);
		tokenlist = tokenize(line, (*data)->env);
		if (tokenlist)
		{
			if (!set_token_types(tokenlist, (*data)->env, &(*data)->nb_pipe,
					&(*data)->here_doc))
				if (*tokenlist)
					execute_line(tokenlist, data);
			free_token_list(tokenlist);
		}
		free(line);
	}
}
