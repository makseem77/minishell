/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 10:59:54 by ymeziane          #+#    #+#             */
/*   Updated: 2024/02/14 15:18:48 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//Returns the tokens in the tokenlist as an array of strings.
static char	**tokens_to_array(t_token **token)
{
	size_t	nb_args;
	t_token	*tmp;
	char	**args;
	size_t	i;

	tmp = *token;
	nb_args = 0;
	while (tmp)
	{
		nb_args++;
		tmp = tmp->next;
	}
	args = malloc(sizeof(char *) * (nb_args + 1));
	tmp = *token;
	i = 0;
	while (tmp)
	{
		args[i] = tmp->element;
		i++;
		tmp = tmp->next;
	}
	args[i] = NULL;
	return (args);
}

//Executes the builtin command in the token.
static void	execute_bultin(t_token **token, t_data **data)
{
	char	**args;

	args = tokens_to_array(token);
	if (ft_strcmp((*token)->element, "cd") == 0)
	{
		if ((*token)->next)
			cd((*token)->next->element, data);
		else
			cd("", data);
	}
	else if (ft_strcmp((*token)->element, "echo") == 0)
		echo(&(*token)->next);
	else if (ft_strcmp((*token)->element, "env") == 0)
		env(args, (*data)->env);
	else if (ft_strcmp((*token)->element, "exit") == 0)
	{
		if ((*token)->next)
			exit_bash((*token)->next->element);
		else
			exit_bash(NULL);
	}
	else if (ft_strcmp((*token)->element, "export") == 0)
		export(args, (*data)->env, (*data)->exp_list);
	else if (ft_strcmp((*token)->element, "pwd") == 0)
		pwd();
	else if (ft_strcmp((*token)->element, "unset") == 0)
		unset(args, (*data)->env, (*data)->exp_list);
	free(args);
}

//Returns the path of the command if it is executable, NULL if it is not.
static char	*get_path_cmd(char **paths, char *cmd)
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

//Executes the command in the token.
static void	execute_cmd(t_token *token, t_env_list **env)
{
	size_t	nb_args;
	t_token	*tmp;
	char	**args;
	size_t	i;
	char	**envp;
	char	**bin_paths;
	char	*path_cmd;
	pid_t	pid;

	envp = env_list_to_array(env);
	tmp = token;
	nb_args = 0;
	path_cmd = NULL;
	while (tmp)
	{
		nb_args++;
		tmp = tmp->next;
	}
	args = malloc(sizeof(char *) * (nb_args + 1));
	tmp = token;
	i = 0;
	while (tmp)
	{
		args[i] = tmp->element;
		i++;
		tmp = tmp->next;
	}
	args[i] = NULL;
	bin_paths = find_bin_paths(env);
	path_cmd = get_path_cmd(bin_paths, token->element);
	free_double_array(bin_paths);
	pid = fork();
	if (pid == 0)
	{
		if (path_cmd)
			execve(path_cmd, args, envp);
		exit(1);
	}
	else
	{
		free_double_array(envp);
		free(args);
		free(path_cmd);
		waitpid(pid, NULL, 0);
	}
}

//Executes the tokens in the tokenlist.
void	process_tokens(t_token **tokenlist, t_data **data)
{
	if (!*tokenlist)
		return ;
	if ((*tokenlist)->ttype == COMMAND)
		execute_cmd(*tokenlist, (*data)->env);
	else if ((*tokenlist)->ttype == BUILTIN)
		execute_bultin(tokenlist, data);
}
