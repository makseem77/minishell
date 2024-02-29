/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 17:23:46 by ymeziane          #+#    #+#             */
/*   Updated: 2024/02/29 15:58:27 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	count_tokens(t_token **token)
{
	int	i;
	t_token	*tmp;

	i = 0;
	tmp = *token;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

//Returns the tokens in the tokenlist as an array of strings.
char	**tokens_to_array(t_token **token)
{
	size_t	nb_tokens;
	t_token	*tmp;
	char	**args;
	size_t	i;

	nb_tokens = count_tokens(token);
	args = (char **)malloc(sizeof(char *) * (nb_tokens + 1));
	tmp = *token;
	i = 0;
	while (tmp)
	{
		args[i] = ft_strdup(tmp->element);
		i++;
		tmp = tmp->next;
	}
	args[i] = NULL;
	return (args);
}

static void	handle_cd_exit(t_token **tokenlist, char **args, t_data **data)
{
	if (ft_strcmp(*args, "cd") == 0)
	{
		if (*(args + 1))
		{
			if (*(args + 2))
			{
				print_error("cd", NULL, "too many arguments");
				return ;
			}
			cd(*(args + 1), data);
		}
		else
			cd(NULL, data);
	}
	else if (ft_strcmp(*args, "exit") == 0)
	{
		if (*(args + 1))
			exit_bash(*(args + 1), data, tokenlist);
		else
			exit_bash(NULL, data, tokenlist);
	}
}

//Executes the builtin command in the token.
void	execute_bultin(t_token **tokenlist, t_data **data, char *cmd)
{
	char	**args;
	char	**tmp_tokens;
	t_token	*tmp_token;
	t_token	*token_to_be_exec;

	tmp_tokens = tokens_to_array(tokenlist);
	tmp_token = *tokenlist;
	while (tmp_token)
	{
		if(ft_strcmp(tmp_token->element, cmd) == 0)
			token_to_be_exec = tmp_token;
		tmp_token = tmp_token->next;
	}
	args = cut_args_at_pipe(tmp_tokens, token_to_be_exec->element);
	if (ft_strcmp(token_to_be_exec->element, "echo") == 0)
		echo(args);
	else if (ft_strcmp(token_to_be_exec->element, "env") == 0)
		env(args, (*data)->env);
	else if (ft_strcmp(token_to_be_exec->element, "export") == 0)
		export(args, (*data)->env, (*data)->exp_list);
	else if (ft_strcmp(token_to_be_exec->element, "pwd") == 0)
		pwd();
	else if (ft_strcmp(token_to_be_exec->element, "unset") == 0)
		unset(args, (*data)->env, (*data)->exp_list);
	handle_cd_exit(tokenlist, args, data);
	free_double_array(args);
	free(tmp_tokens);
}
