/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 15:26:32 by ymeziane          #+#    #+#             */
/*   Updated: 2024/02/08 17:30:13 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

bool	set_data(t_data **data, char **envp)
{
	*data = (t_data *)malloc(sizeof(t_data));
	if (!*data)
		return (false);
	(*data)->old_pwd = get_current_dir();
	(*data)->env = dup_env(envp);
	(*data)->home_dir = ft_strdup(get_env("HOME", (*data)->env));
	return (true);
}

int	main(int argc, char **argv, char **envp)
{
	t_data	*data = NULL;
	
	if(argc > 1 || ft_strcmp(argv[0], "./minishell"))
		return(ft_putstr_fd("Usage: ./minishell\n", 2), 0);
	argc += 1;
	*argv += 1;
	envdup = dup_env(envp);
	/*pwd();
	ft_cd("~");
	printf("after cd\n");
	pwd();*/
	char *args[]= {"X=hello", "D=hola", NULL};
	export(args, envdup);
	listening_loop(envdup);
	return (0);
}
