/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 10:59:37 by ymeziane          #+#    #+#             */
/*   Updated: 2024/02/29 15:58:03 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	count_args(char **args)
{
	size_t	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

char	**cut_args_at_pipe(char **args, char *token_to_be_exec)
{
	char	**newargs;
	int		i;
	int		j;
	size_t	nb_args;
	size_t	cmd_len;

	nb_args = count_args(args);
	i = nb_args - 1;
	j = 0;
	while (args[i])
	{
		if (ft_strcmp(args[i], token_to_be_exec) == 0)
			break ;
		i--;
	}
	cmd_len = nb_args - i;
	newargs = malloc(sizeof(char *) * (cmd_len + 1));
	while (j < (int)(cmd_len))
	{
		newargs[j] = ft_strdup(args[i]);
		j++;
		i++;
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
