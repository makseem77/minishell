/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 15:23:28 by ymeziane          #+#    #+#             */
/*   Updated: 2024/02/01 17:21:33 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

//Returns a duplicate of envp. 
//Goes trough the 2d array envp once to get its size for allocating
//memory, then goes trough it a second time to duplicate each env variables.
char	**dup_env(char **env)
{
	char	**envdup;
	int	size;

	size = 0;
	while (env[size])
		size++;
	envdup = malloc(sizeof(char *) * (size + 1));
	size = 0;
	while (env[size])
	{
		envdup[size] = ft_strdup(env[size]);
		size++;
	}
	envdup[size] = 0;
	return (envdup);
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

