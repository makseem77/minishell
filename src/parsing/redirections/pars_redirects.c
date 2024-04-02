/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_redirects.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 10:49:32 by ymeziane          #+#    #+#             */
/*   Updated: 2024/04/02 12:49:39 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	create_and_set_fd(t_token *tmp, t_token *command_token,
		t_data **data, t_token **tokenlist)
{
	if (ft_strcmp(tmp->element, ">>") == 0 || ft_strcmp(tmp->element,
			">") == 0)
	{
		if (ft_strncmp(tmp->element, ">>", 2) == 0)
			return (create_or_append(tmp, command_token, data));
		else
			return (create_or_truncate(tmp, command_token, data));
	}
	else if (ft_strcmp(tmp->element, "<<") == 0 || ft_strcmp(tmp->element,
			"<") == 0)
	{
		if (ft_strncmp(tmp->element, "<<", 2) == 0)
			create_and_read_from_heredoc(tmp, command_token, data, tokenlist);
		else
			return (read_from_file(tmp, command_token, data));
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
	bool	flag;

	tmp = *tokenlist;
	expr_index = 0;
	flag = true;
	tmp = *tokenlist;
	while (tmp)
	{
		command_token = get_cmd_token(tokenlist, expr_index);
		if (ft_strcmp(tmp->element, "|") == 0)
		{
			expr_index++;
			flag = true;
		}
		if (flag && !create_and_set_fd(tmp, command_token, data, tokenlist))
		{
			flag = false;
			(*data)->invalid_file = true;
		}
		tmp = tmp->next;
	}
	return (0);
}
