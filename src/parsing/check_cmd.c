/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 10:43:55 by ymeziane          #+#    #+#             */
/*   Updated: 2024/03/22 11:36:14 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static int	check_in_paths(char *element, t_env_list **env)
{
	char	**bin_paths;
	char	*executable;
	char	*tmp;
	int		i;

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

int	is_a_command(char *element, t_env_list **env)
{
	if (is_executable(element, NULL, false))
		return (1);
	if (errno == EACCES)
		return (0);
	if (check_in_paths(element, env))
		return (1);
	return (0);
}
