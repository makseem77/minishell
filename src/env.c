/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 15:23:28 by ymeziane          #+#    #+#             */
/*   Updated: 2024/01/31 15:35:40 by ymeziane         ###   ########.fr       */
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

//Returns 1 if it's element is actually an executable command,
//0 if it is not.
//Here, we go trough the splitted PATH variables, and join element
//to each of them, then check if they are actually executable.
//We also do a first check with open to make sure that we are not
//using access, X_OK on a directory because it would return 0 like
//an exe.
int	is_a_command(char *element, char **bin_paths)
{
	char	*executable;
	int	fd;

	executable = NULL;
	while (*bin_paths)
	{
		executable = ft_strjoin(*bin_paths, "/");
		executable = ft_strjoin(executable, element);
		printf("Element = %s\n\n", executable);
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
