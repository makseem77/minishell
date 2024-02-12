/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 22:29:14 by maxborde          #+#    #+#             */
/*   Updated: 2024/02/12 17:38:43 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	set_data(t_data **data, char **envp)
{
	char	*home;

	*data = (t_data *)malloc(sizeof(t_data));
	if (!*data)
		return (false);
	(*data)->old_pwd = get_current_dir();
	(*data)->env = dup_env(envp);
	(*data)->exp_list = get_export_variables((*data)->env);
	home = get_env("HOME", (*data)->env);
	if (home)
		(*data)->home_dir = ft_strdup(home);
	return (true);
}

int	main(int argc, char **argv, char **envp)
{
	t_data	*data;

	data = NULL;
	if (argc > 1 || ft_strcmp(argv[0], "./minishell"))
		return (ft_putstr_fd("Usage: ./minishell\n", 2), 0);
	if (!set_data(&data, envp))
		return (ft_putstr_fd("Error: malloc failed\n", 2), 1);
	listening_loop(&data);
	return (0);
}
