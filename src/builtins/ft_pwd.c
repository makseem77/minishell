/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 11:29:19 by maxborde          #+#    #+#             */
/*   Updated: 2024/03/18 15:17:09 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// get_current_dir will return the current directory.
// If it fails, it will print the error message and return NULL.
char	*get_current_dir(void)
{
	char	current_dir[PATH_MAX];

	if (getcwd(current_dir, PATH_MAX) == NULL)
	{
		perror("getcwd");
		ft_putstr_fd("\n", 1);
		return (NULL);
	}
	return (ft_strdup(current_dir));
}

//pwd will print the working directory.
//It takes no argument: pwd and pwd "somerandomstuff" will act the same.
void	pwd(void)
{
	char	*current_dir;

	current_dir = get_current_dir();
	ft_putstr_fd(current_dir, 1);
	ft_putstr_fd("\n", 1);
	free(current_dir);
	g_status = 0;
	return ;
}
