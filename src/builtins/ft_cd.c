/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 13:03:07 by ymeziane          #+#    #+#             */
/*   Updated: 2024/03/20 13:24:03 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// If the path is "-",
// it will change the current directory to the previous directory.
// If the path is "~",
// it will change the current directory to the home directory.
// If the path is "",
// it will change the current directory to the home directory,
// if the HOME variable is set, otherwise it will print the error message.
static char	*update_absolute_path(char *absolute_path, char *old_pwd,
		t_data **data)
{
	if (!absolute_path)
	{
		absolute_path = get_env("HOME", (*data)->env);
		if (!absolute_path)
			return (print_error("cd", NULL, "HOME not set"), NULL);
	}
	if (ft_strcmp(absolute_path, "-") == 0)
	{
		absolute_path = old_pwd;
		ft_putstr_fd(absolute_path, 1);
		ft_putstr_fd("\n", 1);
	}
	else if (ft_strcmp(absolute_path, "~") == 0)
		absolute_path = (*data)->home_dir;
	else if (ft_strcmp(absolute_path, "") == 0)
		return (NULL);
	return (absolute_path);
}

// cd will change the current directory.
// If the path is not valid, it will print the error message.
// If the path is valid, it will change the current directory.
void	cd(char *absolute_path, t_data **data)
{
	char	old_pwd[PATH_MAX];
	char	current_dir[PATH_MAX];

	if (!getcwd(current_dir, sizeof(current_dir)))
	{
		print_error("cd", NULL, strerror(errno));
		g_status = 1;
		return ;
	}
	absolute_path = update_absolute_path(absolute_path, (*data)->old_pwd, data);
	if (!absolute_path)
		absolute_path = current_dir;
	if (chdir(absolute_path) != 0)
	{
		print_error("cd", absolute_path, strerror(errno));
		g_status = 1;
		return ;
	}
	else
	{
		ft_strlcpy(old_pwd, current_dir, PATH_MAX);
		free((*data)->old_pwd);
		(*data)->old_pwd = ft_strdup(old_pwd);
	}
}
