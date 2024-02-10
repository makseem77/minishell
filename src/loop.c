/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:43:34 by ymeziane          #+#    #+#             */
/*   Updated: 2024/02/10 14:26:47 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	listening_loop(t_data **data) //don't forget to free the line
{
	char	*line;
	t_token **tokenlist;
	t_token *tmp;
	
	while(1)
	{
		line = readline("minishell> ");
        if (!line)
			return;
		if (*line)
			add_history(line);
		tokenlist = tokenize(line, (*data)->env);
		set_token_types(tokenlist, (*data)->env);
		process_tokens(tokenlist, data);
		free(line);
		tmp = *tokenlist;
		while (tmp)
		{
			// printf("Element = %s\n", tmp->element);
			// printf("Type = %d\n", tmp->ttype);
			tmp = tmp->next;
		}
	}
}
