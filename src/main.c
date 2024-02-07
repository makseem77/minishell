/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 15:26:32 by ymeziane          #+#    #+#             */
/*   Updated: 2024/02/07 14:28:27 by maxborde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"


int	main(int argc, char *argv[], char *envp[])
{
	t_env_list	**envdup;
	
	if(argc > 1 || ft_strcmp(argv[0], "./minishell"))
		return(ft_putstr_fd("Usage: ./minishell\n", 2), 0);
	argc += 1;
	*argv += 1;
	envdup = dup_env(envp);
	//export(NULL, envdup);
	listening_loop(envdup);
	return (0);
}
