/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 10:59:37 by ymeziane          #+#    #+#             */
/*   Updated: 2024/03/26 12:26:51 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_expression_length(char **array, int index)
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
				break ;
		}
		array++;
	}
	while (array[i] && ft_strcmp(array[i], "|"))
		i++;
	return (i);
}

char	**cut_arrays_into_expression(char **array, int index)
{
	int		i;
	int		expression_length;
	char	**expression;

	expression_length = get_expression_length(array, index);
	expression = malloc(sizeof(char *) * (expression_length + 1));
	i = 0;
	while (array[i] && ft_strcmp(array[i], "|"))
	{
		expression[i] = ft_strdup(array[i]);
		i++;
	}
	expression[i] = 0;
	return (expression);
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
