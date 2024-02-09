/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxborde <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 22:29:14 by maxborde          #+#    #+#             */
/*   Updated: 2024/02/10 00:26:33 by maxborde         ###   ########.fr       */
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
	char	*args[] = {"LANGUAGE", "PAGER", "HOME", NULL};
	char	*argE[] = {"5agvwvr=42", "B=", "C=", "D[=", "[", "==", NULL};
	
	if(argc > 1 || ft_strcmp(argv[0], "./minishell"))
		return(ft_putstr_fd("Usage: ./minishell\n", 2), 0);
	if(!set_data(&data, envp))
		return (ft_putstr_fd("Error: malloc failed\n", 2), 1);
	printf("\n\n\n\n\n");
	unset(args, data->env);
	//unset needs to send invalid identifier message on "==" and "HOME=" to be like bash.
	//env(data->env);
	printf("\n\n\n\n\n");
	export(argE, data->env);
	listening_loop(data->env);
	return (0);
}


//New case to handle: when running env -i ./minishell, we have a seg fault caused by set_data that checks for $HOME. 
