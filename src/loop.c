/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:43:34 by ymeziane          #+#    #+#             */
/*   Updated: 2024/03/25 18:53:42 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//This variable is used to store the exit code of the last command executed 
//in the  shell.
int		g_status = 0;

// The main loop of the program
// It reads the input from the user and processes it
// It will keep reading the input until the user exits the program
void	listening_loop(t_data **data)
{
	char	*line;
	t_token	**tokenlist;

	tokenlist = NULL;
	while (true)
	{
		init_signals();
		line = readline("minishell> ");
		if (!line)
			exit_bash(EXIT_SUCCESS, data, NULL, NULL);
		if (*line)
			add_history(line);
		tokenlist = tokenize(line, data);
		if (tokenlist)
		{
			if (set_token_types(tokenlist, data))
				if (*tokenlist)
					execute_line(tokenlist, data);
			free_token_list(tokenlist);
		}
		if ((*data)->here_doc)
			unlink(".tmp");
		(*data)->here_doc = false;
		free(line);
	}
}
