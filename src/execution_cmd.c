/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_cmd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 10:59:54 by ymeziane          #+#    #+#             */
/*   Updated: 2024/02/22 17:27:50 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//Returns the path of the command if it is executable, NULL if it is not.
char	*get_path_cmd(char **paths, char *cmd)
{
	char	*tmp;
	char	*command;

	if (cmd[0] == '.' || cmd[0] == '/')
	{
		if (access(cmd, F_OK | X_OK) == 0)
			return (cmd);
		return (NULL);
	}
	if (!paths)
		return (NULL);
	while (*paths)
	{
		tmp = ft_strjoin(*paths, "/");
		command = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(command, F_OK | X_OK) == 0)
			return (command);
		free(command);
		paths++;
	}
	return (NULL);
}

static char	**fill_args(t_token *token)
{
	size_t	nb_args;
	t_token	*tmp;
	char	**args;
	size_t	i;

	tmp = token;
	nb_args = 0;
	while (tmp)
	{
		nb_args++;
		tmp = tmp->next;
	}
	args = malloc(sizeof(char *) * (nb_args + 1));
	tmp = token;
	i = 0;
	while (tmp && tmp->ttype != META_CHAR)
	{
		args[i] = tmp->element;
		i++;
		tmp = tmp->next;
	}
	args[i] = NULL;
	return (args);
}

//Executes the command in the token.
static void	execute_cmd(t_token *token, t_env_list **env)
{
	char	**args;
	char	**bin_paths;
	char	*path_cmd;
	char	**envp;

	args = fill_args(token);
	for(int i = 0; args[i]; i++)
		printf("args[%d] = %s\n", i, args[i]);
	bin_paths = find_bin_paths(env);
	path_cmd = get_path_cmd(bin_paths, token->element);
	free_double_array(bin_paths);
	envp = env_list_to_array(env);
	if (path_cmd)
		execve(path_cmd, args, envp);
	free_double_array(envp);
	free(args);
	free(path_cmd);
	exit(EXIT_FAILURE);
}

//Executes the tokens in the tokenlist.
void	process_tokens(t_token **tokenlist, t_data **data)
{
	if (!*tokenlist)
		return ;
	if ((*tokenlist)->ttype == COMMAND)
		execute_cmd(*tokenlist, (*data)->env);
	else if ((*tokenlist)->ttype == BUILTIN)
		execute_bultin(tokenlist, data, (*tokenlist)->element);
}
