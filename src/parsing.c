/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 15:23:35 by ymeziane          #+#    #+#             */
/*   Updated: 2024/02/14 15:10:00 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	is_executable(char *executable, char **bin_paths, bool should_free)
{
	int	fd;

	fd = open(executable, O_DIRECTORY);
	if (fd == -1)
	{
		if (access(executable, X_OK) == 0)
		{
			if (should_free)
			{
				free_double_array(bin_paths);
				free(executable);
			}
			return (1);
		}
	}
	else
		close(fd);
	return (0);
}

static int	is_a_command(char *element, t_env_list **env)
{
	char	**bin_paths;
	char	*executable;
	char	*tmp;
	int		i;

	if (is_executable(element, NULL, false))
		return (1);
	bin_paths = find_bin_paths(env);
	if (!bin_paths)
		return (0);
	i = 0;
	while (bin_paths[i])
	{
		executable = ft_strjoin(bin_paths[i], "/");
		tmp = executable;
		executable = ft_strjoin(executable, element);
		free(tmp);
		if (is_executable(executable, bin_paths, true))
			return (1);
		free(executable);
		i++;
	}
	free_double_array(bin_paths);
	return (0);
}

static int	type(char *element, t_env_list **env)
{
	if (!ft_strcmp("echo", element) || !ft_strcmp("cd", element)
		|| !ft_strcmp("pwd", element) || !ft_strcmp("export", element)
		|| !ft_strcmp("unset", element) || !ft_strcmp("env", element)
		|| !ft_strcmp("exit", element))
		return (BUILTIN);
	if (!ft_strcmp(">", element) || !ft_strcmp(">>", element) || !ft_strcmp("<",
			element) || !ft_strcmp("<<", element) || !ft_strcmp("&", element)
		|| !ft_strcmp("|", element) || !ft_strcmp("&&", element)
		|| !ft_strcmp("||", element) || !ft_strcmp(",", element)
		|| !ft_strcmp("(", element) || !ft_strcmp(")", element))
		return (META_CHAR);
	if (is_a_command(element, env))
		return (COMMAND);
	return (-1);
}

// Goes trough the token linked list
// and gives a tokentype to every node of the list.
void	set_token_types(t_token **tokenlist, t_env_list **env)
{
	t_token	*tmp;
	bool	first;

	tmp = *tokenlist;
	first = true;
	while (tmp)
	{
		if (type(tmp->element, env) == BUILTIN)
			tmp->ttype = BUILTIN;
		else if (type(tmp->element, env) == META_CHAR)
			tmp->ttype = META_CHAR;
		else if (type(tmp->element, env) == COMMAND)
			tmp->ttype = COMMAND;
		else if (first)
		{
			ft_putstr_fd(tmp->element, 2);
			ft_putstr_fd(": command not found\n", 2);
			return ;
		}
		first = false;
		tmp = tmp->next;
	}
}
