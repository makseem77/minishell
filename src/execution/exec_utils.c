/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 10:59:37 by ymeziane          #+#    #+#             */
/*   Updated: 2024/04/02 12:28:52 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**duplicate_expression(char **array, int i)
{
	char	**expression;

	expression = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (array[i] && ft_strcmp(array[i], "|"))
	{
		if ((ft_strcmp(array[i], "\"|\"") == 0)
			|| ft_strcmp(array[i], "\'|\'") == 0)
			expression[i] = ft_strdup("|");
		else
			expression[i] = ft_strdup(array[i]);
		i++;
	}
	expression[i] = 0;
	return (expression);
}

char	**cut_arrays_into_expression(char **array, int index)
{
	int		i;
	int		pipecount;

	i = 0;
	pipecount = 0;
	while (*array && index > 0)
	{
		if (ft_strcmp(*array, "|") == 0)
		{
			pipecount++;
			if (pipecount == index)
			{
				array++;
				break ;
			}
		}
		array++;
	}
	while (array[i] && ft_strcmp(array[i], "|"))
		i++;
	return (duplicate_expression(array, i));
}

char	*get_path_cmd(char **paths, char *cmd)
{
	char	*tmp;
	char	*command;

	if (!cmd)
		return (NULL);
	if (cmd[0] == '.' || cmd[0] == '/')
	{
		if (access(cmd, F_OK | X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	if (!paths)
		return (NULL);
	while (*paths)
	{
		tmp = ft_strjoin(*paths, "/");
		command = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(command, F_OK | X_OK) == 0)
			return (command);
		free(command);
		paths++;
	}
	return (NULL);
}

static size_t	count_tokens(t_token **token)
{
	int		i;
	t_token	*tmp;

	i = 0;
	tmp = *token;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

// Returns the tokens in the tokenlist as an array of strings.
char	**tokens_to_array(t_token **token)
{
	size_t	nb_tokens;
	t_token	*tmp;
	char	**args;
	size_t	i;

	nb_tokens = count_tokens(token);
	args = (char **)malloc(sizeof(char *) * (nb_tokens + 1));
	tmp = *token;
	i = 0;
	while (tmp)
	{
		args[i] = ft_strdup(tmp->element);
		i++;
		tmp = tmp->next;
	}
	args[i] = NULL;
	return (args);
}
