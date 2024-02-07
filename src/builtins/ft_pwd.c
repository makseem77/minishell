/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 11:29:19 by maxborde          #+#    #+#             */
/*   Updated: 2024/02/07 17:41:56 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//pwd will print the working directory.
//It takes no argument: pwd and pwd "somerandomstuff" will act the same.
void	pwd(void)
{
	char	buffer[PATH_MAX + 1];

	if (getcwd(buffer, PATH_MAX + 1) == NULL)
	{
		ft_putstr_fd("Cannot get current working directory path\n", 2);
		write(2, strerror(errno), strlen(strerror(errno)));
        write(2, "\n", 1);
		if (errno == ERANGE)
			ft_putstr_fd("Buffer size is too small.\n", 2);
		return ;
	}
	printf("Current working directory: %s\n", buffer);
}
