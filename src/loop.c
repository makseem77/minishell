/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:43:34 by ymeziane          #+#    #+#             */
/*   Updated: 2024/02/13 22:52:17 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// The main loop of the program
// It reads the input from the user and processes it
// It will keep reading the input until the user exits the program
void	listening_loop(t_data **data)
{
	char	*line;
	t_token	**tokenlist;

	tokenlist = NULL;
	handle_signals(tokenlist);
	while (1)
	{
		line = readline("minishell> ");
		if(!line)
		{
			free_data_struct(*data);
			if(tokenlist)
				free_token_list(tokenlist);
			exit_bash(0);
		}
		if (!line)
			return ;
		if (*line)
			add_history(line);
		if(tokenlist)
			free_token_list(tokenlist);
		tokenlist = tokenize(line, (*data)->env);
		set_token_types(tokenlist, (*data)->env);
		process_tokens(tokenlist, data);
	}
}
