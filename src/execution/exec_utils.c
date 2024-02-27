/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 10:59:37 by ymeziane          #+#    #+#             */
/*   Updated: 2024/02/27 16:26:34 by maxborde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	count_args(char **args)
{
	size_t	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

char	**cut_args_at_pipe(char **args)
{
	char	**newargs;
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (args[i])
	{
		if (ft_strcmp(args[i], "|") == 0)
			break;
		i++;
	}
	newargs = malloc(sizeof(char *) * (i + 1));
	while (j < i)
	{
		newargs[j] = ft_strdup(args[j]);
		j++;
	}	
	newargs[j] = 0;
	return (newargs);
}

char	*get_path_cmd(char **paths, char *cmd)
{
	char	*tmp;
	char	*command;

	if (cmd[0] == '.' || cmd[0] == '/')
	{
		if (access(cmd, F_OK | X_OK) == 0)
			return (cmd);
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
