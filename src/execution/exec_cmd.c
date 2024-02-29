/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 13:21:20 by ymeziane          #+#    #+#             */
/*   Updated: 2024/02/29 15:27:34 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**cut_arrays_into_expression(char **array, int index)
{
	int	i;	
	int	pipecount;
	char	**expression;

	i = 0;
	pipecount = 0;
	while(*array && index > 0)
	{
		if (ft_strcmp(*array, "|") == 0)
		{
			if (pipecount == index - 1)
			{
				array++;
				break;
			}
			else
				pipecount++;
		}
		array++;
	}
	while (array[i] && ft_strcmp(array[i], "|"))
		i++;
	expression = malloc(sizeof(char *) * (i + 1));	
	i = 0;
	while (array[i] && ft_strcmp(array[i], "|"))
	{
		expression[i] = ft_strdup(array[i]);
		i++;
	}
	expression[i] = 0;
	return (expression);
}

static void configure_io(int index, int **fds, t_data **data)
{
    if (index == 0)
        dup2(fds[index][1], STDOUT_FILENO);
    else if (index == (*data)->nb_pipe)
        dup2(fds[index - 1][0], STDIN_FILENO);
    else
    {
        dup2(fds[index - 1][0], STDIN_FILENO);
        dup2(fds[index][1], STDOUT_FILENO);
    }
}

void exec_cmd(t_data **data, int index, int **fds, char **args)
{   
	char	**expression;
    char    **bin_paths;
    char	*path_cmd;
    pid_t    pid;
    // int      status;

    bin_paths = find_bin_paths((*data)->env);
    if((*data)->nb_pipe == 0)
        expression = args;
    else
        expression = cut_arrays_into_expression(args, index);
    path_cmd = get_path_cmd(bin_paths, expression[0]);
    if(!path_cmd)
		print_error(expression[0], NULL, "command not found");
    pid = fork();
    if (pid == 0)
    {
        configure_io(index, fds, data);
        close_all_pipes(fds, (*data)->nb_pipe);
        if (execve(path_cmd, expression, env_list_to_array((*data)->env)) == -1)
            exit(1);
    }
    // else
    //     waitpid(pid, &status, 0);
}