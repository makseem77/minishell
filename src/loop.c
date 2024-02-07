/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:43:34 by ymeziane          #+#    #+#             */
/*   Updated: 2024/02/07 14:12:47 by maxborde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	listening_loop(t_env_list **env) //don't forget to free the line
{
	char	*line;
	t_token **tokenlist;
	t_token *tmp;
	(void)env;
	
	while(1)
	{
		line = readline("minishell> ");
        if (!line)
			return;
		if (*line)
			add_history(line);
		tokenlist = tokenize(line, env);
		//set_token_types(tokenlist, env);
		// execute_line(tokenlist, env);
		free(line);
		tmp = *tokenlist;
		while (tmp)
		{
			printf("Element = %s\n", tmp->element);
			printf("Type = %d\n", tmp->ttype);
			tmp = tmp->next;
		}
	}
}
