/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_type.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 10:40:45 by ymeziane          #+#    #+#             */
/*   Updated: 2024/03/27 09:45:11 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	type(char *element, t_env_list **env)
{
	if (!element)
		return (-1);
	if (!ft_strcmp("echo", element) || !ft_strcmp("cd", element)
		|| !ft_strcmp("pwd", element) || !ft_strcmp("export", element)
		|| !ft_strcmp("unset", element) || !ft_strcmp("env", element)
		|| !ft_strcmp("exit", element))
		return (BUILTIN);
	if (!ft_strcmp("&", element)
		|| !ft_strcmp("|", element) || !ft_strcmp("&&", element)
		|| !ft_strcmp("||", element) || !ft_strcmp(",", element)
		|| !ft_strcmp("(", element) || !ft_strcmp(")", element))
		return (META_CHAR);
	if (is_a_command(element, env))
		return (COMMAND);
	return (-1);
}

static t_token	*set_heredoc_type(t_token *tmp, bool *heredoc)
{
	*heredoc = true;
	tmp->ttype = HERE_DOC;
	if (tmp->next)
	{
		tmp = tmp->next;
		if (ft_strcmp(tmp->element, ">") != 0)
			tmp->ttype = DELIMITER;
		else
			return (NULL);
	}
	return (tmp);
}

static t_token	*set_redirections_type(t_token *tmp)
{
	tmp->ttype = REDIRECTION;
	if (tmp->next)
	{
		tmp = tmp->next;
		if (ft_strncmp(tmp->element, ">", 1) || ft_strncmp(tmp->element, "<",
				1) != 0)
			tmp->ttype = REDIRECTION_FILE;
		else
			return (NULL);
	}
	return (tmp);
}

static void	types_assignement(t_token **tokenlist, t_env_list **env,
		bool *heredoc)
{
	t_token	*tmp;

	tmp = *tokenlist;
	while (tmp)
	{
		if (ft_strncmp(tmp->element, "<<", 2) == 0)
			tmp = set_heredoc_type(tmp, heredoc);
		else if (ft_strncmp(tmp->element, ">>", 2) == 0
			|| ft_strcmp(tmp->element, ">") == 0 || ft_strcmp(tmp->element,
				"<") == 0)
			tmp = set_redirections_type(tmp);
		else if (type(tmp->element, env) == BUILTIN)
			tmp->ttype = BUILTIN;
		else if (type(tmp->element, env) == META_CHAR)
			tmp->ttype = META_CHAR;
		else if (type(tmp->element, env) == COMMAND)
			tmp->ttype = COMMAND;
		if (tmp)
			tmp = tmp->next;
	}
}

// Goes trough the token linked list
// and gives a tokentype to every node of the list.
int	set_token_types(t_token **tokenlist, t_data **data)
{
	if (check_tokens_syntax(tokenlist, data) == -1)
		return (0);
	types_assignement(tokenlist, (*data)->env, &(*data)->here_doc);
	if (clean_up_tokens(tokenlist) == -1)
		return (0);
	if (handle_redirections(tokenlist, data) == 1)
		return (0);
	clean_up_redirection(tokenlist);
	return (1);
}
