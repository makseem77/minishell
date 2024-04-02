/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_redirects.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 12:37:53 by ymeziane          #+#    #+#             */
/*   Updated: 2024/04/02 12:48:52 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_to_be_deleted(t_token *tmp)
{
	if (ft_strcmp(tmp->element, "") == 0 && tmp->ttype != EMPTY)
		return (true);
	return ((tmp->ttype == REDIRECTION || tmp->ttype == REDIRECTION_FILE
			|| tmp->ttype == HERE_DOC || tmp->ttype == DELIMITER));
}

bool	is_a_quoted_redir_operator(char *element)
{
	return ((ft_strncmp(element, "\'<\'", 3) == 0) || (ft_strncmp(element,
				"\'<<\'", 4) == 0) || (ft_strncmp(element, "\'>\'", 3) == 0)
		|| (ft_strncmp(element, "\'>>\'", 4) == 0) || (ft_strncmp(element,
				"\"<\"", 3) == 0) || (ft_strncmp(element, "\"<<\"", 4) == 0)
		|| (ft_strncmp(element, "\">\"", 3) == 0) || (ft_strncmp(element,
				"\">>\"", 4) == 0));
}

static void	clean_up_redirections_quotes(t_token **tokenlist)
{
	t_token	*tmp;
	char	*holder;

	tmp = *tokenlist;
	while (tmp)
	{
		if (is_a_quoted_redir_operator(tmp->element))
		{
			holder = tmp->element;
			tmp->element = ft_strtrim(tmp->element, "\'\"");
			free(holder);
		}
		tmp = tmp->next;
	}
}

static void	delete_redirections_tokens(t_token **tokenlist)
{
	t_token	*tmp;
	t_token	*prev;

	tmp = *tokenlist;
	prev = NULL;
	while (tmp)
	{
		if (is_to_be_deleted(tmp))
		{
			if (prev)
				prev->next = tmp->next;
			else
				*tokenlist = tmp->next;
			free_node(tmp);
			if (prev)
				tmp = prev->next;
			else
				tmp = *tokenlist;
		}
		else
		{
			prev = tmp;
			tmp = tmp->next;
		}
	}
}

void	clean_up_redirection(t_token **tokenlist)
{
	delete_redirections_tokens(tokenlist);
	clean_up_redirections_quotes(tokenlist);
}
