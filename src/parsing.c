/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 15:23:35 by ymeziane          #+#    #+#             */
/*   Updated: 2024/02/01 20:37:53 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	is_a_built_in(char *element)
{
	if (!ft_strcmp("echo", element) || !ft_strcmp("cd", element) ||
		!ft_strcmp("pwd", element) || !ft_strcmp("export", element)
			|| !ft_strcmp("unset", element) || !ft_strcmp("env", element)
			|| !ft_strcmp("exit", element))
		return (1);
	return (0);
}

int	is_a_meta_char(char *element)
{
	if (!ft_strcmp(">", element) || !ft_strcmp(">>", element) ||
		!ft_strcmp("<", element) || !ft_strcmp("<<", element) || !ft_strcmp("&",
				element) || !ft_strcmp("|", element) || !ft_strcmp("&&",
				element) || !ft_strcmp("||", element) || !ft_strcmp(",",
				element) || !ft_strcmp("(", element)
			|| !ft_strcmp(")", element))
		return (1);
	return (0);
}

//Returns 1 if it's element is actually an executable command,
//0 if it is not.
//Here, we go trough the splitted PATH variables, and join element
//to each of them, then check if they are actually executable.
//We also do a first check with open to make sure that we are not
//using access, X_OK on a directory because it would return 0 like
//an exe.
int	is_a_command(char *element, char **env)
{
	char	*executable;
	int	fd;
	char **bin_paths;

	bin_paths = find_bin_paths(env);
	executable = NULL;
	while (*bin_paths)
	{
		executable = ft_strjoin(*bin_paths, "/");
		executable = ft_strjoin(executable, element);
		// printf("Element = %s\n\n", executable);
		fd = open(executable, O_DIRECTORY);
		if (fd == -1) 
		{
			if (access(executable, X_OK) == 0)
				return (1);
		}
		else
			close(fd);
		bin_paths++;
	}
	return (0);
}

//Goes trough the token linked list and gives a tokentype to every node of the list.
void	set_token_types(t_token **tokenlist, char **env)
{
	t_token	*tmp;
	int		i;

	tmp = *tokenlist;
	while (tmp)
	{
		i = 0;
		if (is_a_built_in(tmp->element))
			tmp->ttype = BUILTIN;
		else if (is_a_meta_char(tmp->element))
			tmp->ttype = META_CHAR;
		else if(is_a_command(tmp->element, env))
			tmp->ttype = COMMAND;
		else
			tmp->ttype = STRING;
		tmp = tmp->next;
	}
}
