/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 15:26:32 by ymeziane          #+#    #+#             */
/*   Updated: 2024/01/31 20:10:57 by maxborde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"


int	main(int argc, char *argv[], char *envp[])
{
	char	**envdup;

	printf("Value = %s\n", getenv("LANGUAGE"));
	argc += 1;
	*argv += 1;
	envdup = dup_env(envp);
	listening_loop();
	return (0);
}
