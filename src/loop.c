/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:43:34 by ymeziane          #+#    #+#             */
/*   Updated: 2024/03/15 15:25:57 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	state;

// The main loop of the program
// It reads the input from the user and processes it
// It will keep reading the input until the user exits the program
void	listening_loop(t_data **data)
{
	char	*line;
	t_token	**tokenlist;

	tokenlist = NULL;
	state = 0;
	handle_signals();
	while (true)
	{
		state = 0;
		line = readline("minishell> ");
		if (!line)
			exit_bash(EXIT_SUCCESS, false, data, NULL, NULL);
		if (*line)
			add_history(line);
		if (tokenlist)
			free_token_list(tokenlist);
		tokenlist = tokenize(line, (*data)->env, (*data)->exit_status);
		if (tokenlist)
		{
			if (!set_token_types(tokenlist, (*data)->env, &(*data)->nb_pipe, &(*data)->here_doc))
			{
				if (*tokenlist)
					execute_line(tokenlist, data, &(*data)->exit_status);
			}
		}
		free(line);
	}
}
