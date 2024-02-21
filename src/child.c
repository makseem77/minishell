/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 17:50:58 by ymeziane          #+#    #+#             */
/*   Updated: 2024/02/21 13:01:53 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void child(char* cmd, char** argv)
{
    int pipefd[2], f;

    if (pipe(pipefd) < 0)
        perror("pipe creation failed");

    f = fork();
    if (f == 0) {
        if (dup2(pipefd[1], STDOUT_FILENO) < 0)
            perror("dup2 failed");

        close(pipefd[0]);

    }
    else
    {
        if (dup2(pipefd[0], STDIN_FILENO) < 0)
            perror("dup2 failed");

        close(pipefd[1]);

        if (execvp(cmd, argv) < 0)
            perror("execvp failed");
    }
}

size_t count_args(char **args)
{
    size_t i = 0;
    while (args[i])
        i++;
    return (i);
}

// Gère la création de tous les processus enfants pour les commandes
void children(t_token **tokenlist, t_data **data)
{
    int		i;
    char **args;
    size_t nb_args;

    args = tokens_to_array(tokenlist);
    nb_args = count_args(args);
    i = nb_args - 1;
    while (i >= 1)
    {
        if (ft_strcmp(args[i], "|") == 0)
        {
            args[i] = NULL;
            child(args[i + 1], &args[i + 1]);
            nb_args = i;
        }
        i--;
    }


    char ** env2 = env_list_to_array((*data)->env);
    char **bin_paths = find_bin_paths((*data)->env);
	char *path_cmd = get_path_cmd(bin_paths, args[0]);
    if(execve(path_cmd, args, env2) == -1)
    {
        perror("execve failed");
        exit(EXIT_FAILURE);
    }
}
