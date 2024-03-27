/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_redirects.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 10:49:32 by ymeziane          #+#    #+#             */
/*   Updated: 2024/03/27 16:26:44 by ymeziane         ###   ########.fr       */
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

void	clean_up_redirection(t_token **tokenlist)
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

static void	create_and_set_fd(t_token *tmp, t_token *command_token,
		t_data **data, t_token **tokenlist)
{
	if (ft_strncmp(tmp->element, ">>", 2) == 0 || ft_strcmp(tmp->element,
			">") == 0)
	{
		if (ft_strncmp(tmp->element, ">>", 2) == 0)
			create_or_append(tmp, command_token);
		else
			create_or_truncate(tmp, command_token);
	}
	else if (ft_strncmp(tmp->element, "<<", 2) == 0 || ft_strcmp(tmp->element,
			"<") == 0)
	{
		if (ft_strncmp(tmp->element, "<<", 2) == 0)
			create_and_read_from_heredoc(tmp, command_token, data, tokenlist);
		else
			read_from_file(tmp, command_token);
	}
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

int	handle_redirections(t_token **tokenlist, int *nb_pipe, t_data **data)
{
	t_token	*tmp;
	t_token	*command_token;
	int		expr_index;

	tmp = *tokenlist;
	expr_index = 0;
	while (tmp)
	{
		if (error_syntax(tmp, nb_pipe))
		{
			(*data)->nb_pipe = 0;
			return (1);
		}
		command_token = get_cmd_token(tokenlist, expr_index);
		if (ft_strcmp(tmp->element, "|") == 0)
			expr_index++;
		create_and_set_fd(tmp, command_token, data, tokenlist);
		tmp = tmp->next;
	}
	return (0);
}
