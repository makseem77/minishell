/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 22:29:14 by maxborde          #+#    #+#             */
/*   Updated: 2024/03/26 15:45:17 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//Sets the data structure.
//Returns true if the data structure is set, false otherwise.
//The data structure is set by allocating memory for it and setting its fields.
//The old_pwd field is set to the current directory.
//The env field is set to the environment variables.
//The exp_list field is set to the export variables.
//The home_dir field is set to the value of the HOME environment variable.
static bool	set_data(t_data **data, char **envp)
{
	char	*home;

	*data = (t_data *)malloc(sizeof(t_data));
	if (!*data)
		return (false);
	(*data)->old_pwd = get_current_dir();
	(*data)->env = dup_env(envp);
	(*data)->exp_list = get_export_variables((*data)->env);
	(*data)->bin_paths = find_bin_paths((*data)->env);
	home = get_env("HOME", (*data)->env);
	if (home)
		(*data)->home_dir = ft_strdup(home);
	(*data)->nb_pipe = 0;
	(*data)->pipe_fds = NULL;
	(*data)->here_doc = false;
	(*data)->path_cmd = NULL;
	return (true);
}

int	main(int argc, char **args, char **envp)
{
	t_data	*data;

	(void)args;
	data = NULL;
	if (argc > 1)
		return (ft_putstr_fd("Usage: ./minishell\n", 2), 0);
	if (!set_data(&data, envp))
		return (ft_putstr_fd("Error: malloc failed\n", 2), 1);
	listening_loop(&data);
	return (0);
}
