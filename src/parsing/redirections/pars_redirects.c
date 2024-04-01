/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_redirects.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 10:49:32 by ymeziane          #+#    #+#             */
/*   Updated: 2024/03/28 10:10:25 by ymeziane         ###   ########.fr       */
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

void	clean_up_redirections_quotes(t_token **tokenlist)
{
	t_token *tmp;
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

void	delete_redirections_tokens(t_token **tokenlist)
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

static int	create_and_set_fd(t_token *tmp, t_token *command_token,
		t_data **data, t_token **tokenlist)
{
	if (ft_strncmp(tmp->element, ">>", 2) == 0 || ft_strcmp(tmp->element,
			">") == 0)
	{
		if (ft_strncmp(tmp->element, ">>", 2) == 0)
			return (create_or_append(tmp, command_token, data));
		else
			return (create_or_truncate(tmp, command_token, data));
	}
	else if (ft_strncmp(tmp->element, "<<", 2) == 0 || ft_strcmp(tmp->element,
			"<") == 0)
	{
		if (ft_strncmp(tmp->element, "<<", 2) == 0)
			create_and_read_from_heredoc(tmp, command_token, data, tokenlist);
		else
			read_from_file(tmp, command_token);
	}
	return (1);
}

static t_token	*get_cmd_token(t_token **tokenlist, int expr_index)
{
	t_token	*tmp;
	int		i;

	tmp = *tokenlist;
	i = 0;
	while (tmp)
	{
		if ((tmp->ttype == COMMAND || tmp->ttype == BUILTIN) && i == expr_index)
			return (tmp);
		if (ft_strcmp(tmp->element, "|") == 0)
			i++;
		tmp = tmp->next;
	}
	return (NULL);
}

int	handle_redirections(t_token **tokenlist, t_data **data)
{
	t_token	*tmp;
	t_token	*command_token;
	int		expr_index;

	tmp = *tokenlist;
	expr_index = 0;
	(*data)->invalid_file = false;
	tmp = *tokenlist;
	while (tmp)
	{
		command_token = get_cmd_token(tokenlist, expr_index);
		if (ft_strcmp(tmp->element, "|") == 0)
			expr_index++;
		if (!create_and_set_fd(tmp, command_token, data, tokenlist))
			(*data)->invalid_file = true;
		tmp = tmp->next;
	}
	return (0);
}
