/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 15:23:28 by ymeziane          #+#    #+#             */
/*   Updated: 2024/02/09 17:44:10 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

//Returns a duplicate of envp.
//Goes trough the 2d array envp once to get its size for allocating
//memory, then goes trough it a second time to duplicate each env variables.
t_env_list	**dup_env(char **env)
{
	t_env_list	**env_list;

	env_list = malloc(sizeof(t_env_list *));
	if(!env_list)
		return (NULL);
	*env_list = NULL;
	while (*env)
	{
		lst_add_back(env_list, lst_new(ft_strdup(*env)));
		env++;
	}
	return (env_list);
}

//Returns all the PATHs variables, splitted in a 2d array, without
//the PATH= prefix.
char	**find_bin_paths(t_env_list **env)
{
	char		**bin_paths;
	t_env_list	*tmp;

	tmp = *env;
	bin_paths = NULL;
	while (tmp && ft_strncmp("PATH", tmp->variable, 4))
		tmp = tmp->next;
	bin_paths = ft_split(tmp->variable + 5, ':');
	return (bin_paths);
}


//Returns the value of the variable in the env list.
char	*get_env(char *variable, t_env_list **env)
{
	t_env_list *tmp;

	tmp = *env;
	while (tmp)
	{
		if (ft_strncmp(tmp->variable, variable, ft_strlen(variable)) == 0)
			return (ft_strchr(tmp->variable, '=') + 1);
		tmp = tmp->next;
	}
	return (NULL);
}