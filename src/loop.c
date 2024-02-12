/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 16:43:34 by ymeziane          #+#    #+#             */
/*   Updated: 2024/02/12 23:44:23 by maxborde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	listening_loop(t_data **data)
{
	char	*line;
	t_token	**tokenlist;
	//t_token	*tmp;

	while (1)
	{
		line = readline("minishell> ");
		if (!line)
			return ;
		if (*line)
			add_history(line);
		tokenlist = tokenize(line, (*data)->env);
		set_token_types(tokenlist, (*data)->env);
		process_tokens(tokenlist, data);
		/*tmp = *tokenlist;
		while (tmp)
			tmp = tmp->next;*/
	}
	free(line);
	free_token_list(tokenlist);
	free_data_struct(*data);
}
