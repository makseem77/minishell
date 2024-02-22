/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_builtins.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 17:23:46 by ymeziane          #+#    #+#             */
/*   Updated: 2024/02/22 16:41:54 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//Returns the tokens in the tokenlist as an array of strings.
char	**tokens_to_array(t_token **token)
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

static void	handle_cd_exit(t_token **tokenlist, t_data **data)
{
	if (ft_strcmp((*tokenlist)->element, "cd") == 0)
	{
		if ((*tokenlist)->next)
		{
			if ((*tokenlist)->next->next)
			{
				print_error("cd", NULL, "too many arguments");
				return ;
			}
			cd((*tokenlist)->next->element, data);
		}
		else
			cd(NULL, data);
	}
	else if (ft_strcmp((*tokenlist)->element, "exit") == 0)
	{
		if ((*tokenlist)->next)
			exit_bash((*tokenlist)->next->element, data, tokenlist);
		else
			exit_bash(NULL, data, tokenlist);
	}
}

//Executes the builtin command in the token.
void	execute_bultin(t_token **tokenlist, t_data **data, char *cmd)
{
	char	**args;
	t_token	*tmp;
	int		i;

	tmp = *tokenlist;
	i = 0;
	while (tmp && ft_strcmp(tmp->element, cmd) != 0)
	{
		i++;
		tmp = tmp->next;
	}
	args = tokens_to_array(tokenlist);
	if (ft_strcmp(tmp->element, "echo") == 0)
		echo(&(tmp)->next);
	else if (ft_strcmp(tmp->element, "env") == 0)
		env(args, (*data)->env);
	else if (ft_strcmp(tmp->element, "export") == 0)
		export(args, (*data)->env, (*data)->exp_list);
	else if (ft_strcmp(tmp->element, "pwd") == 0)
		pwd();
	else if (ft_strcmp(tmp->element, "unset") == 0)
		unset(args, (*data)->env, (*data)->exp_list);
	handle_cd_exit(tokenlist, data);
	free(args);
}
