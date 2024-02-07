/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 15:23:28 by ymeziane          #+#    #+#             */
/*   Updated: 2024/02/07 14:12:22 by maxborde         ###   ########.fr       */
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
	while (*env)
	{	
		lst_add_back(env_list, lst_new(ft_strdup(*env)));
		env++;
	}
	return (env_list);
}

//Returns all the PATHs variables, splitted in a 2d array, without
//the PATH= prefix.
char	**find_bin_paths(char **env)
{
	char	**bin_paths;

	bin_paths = NULL;
	while (ft_strncmp("PATH", *env, 4))
		env++;
	bin_paths = ft_split(*env + 5, ':');
	return (bin_paths);
}

