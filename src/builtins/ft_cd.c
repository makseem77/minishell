/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 13:03:07 by ymeziane          #+#    #+#             */
/*   Updated: 2024/02/13 11:37:41 by ymeziane         ###   ########.fr       */
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
	if (ft_strcmp(absolute_path, "-") == 0 && old_pwd[0])
	{
		absolute_path = old_pwd;
		ft_putstr_fd(absolute_path, 1);
		write(2, "\n", 1);
	}
	else if (ft_strcmp(absolute_path, "~") == 0)
		absolute_path = (*data)->home_dir;
	else if (ft_strcmp(absolute_path, "") == 0)
	{
		absolute_path = get_env("HOME", (*data)->env);
		if (!absolute_path)
			return (ft_putstr_fd("cd: HOME not set\n", 2), NULL);
	}
	return (absolute_path);
}

// cd will change the current directory.
// If the path is not valid, it will print the error message.
// If the path is valid, it will change the current directory.
void	cd(char *absolute_path, t_data **data)
{
	static char	old_pwd[PATH_MAX];
	char		current_dir[PATH_MAX];

	if (!absolute_path)
		return ;
	if (!getcwd(current_dir, sizeof(current_dir)))
	{
		ft_putstr_fd(strerror(errno), 2);
		write(2, "\n", 1);
		return ;
	}
	absolute_path = update_absolute_path(absolute_path, old_pwd, data);
	if (!absolute_path)
		return ;
	if (chdir(absolute_path) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		write(2, absolute_path, ft_strlen(absolute_path));
		ft_putstr_fd(": ", 2);
		write(2, strerror(errno), ft_strlen(strerror(errno)));
		write(2, "\n", 1);
	}
	else
	{
		ft_strlcpy(old_pwd, current_dir, PATH_MAX);
		(*data)->old_pwd = ft_strdup(old_pwd);
	}
}
