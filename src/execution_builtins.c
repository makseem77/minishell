/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_builtins.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 17:23:46 by ymeziane          #+#    #+#             */
/*   Updated: 2024/02/14 18:18:33 by maxborde         ###   ########.fr       */
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

static void	handle_cd_exit(t_token **token, t_data **data)
{
	if (ft_strcmp((*token)->element, "cd") == 0)
	{
		if ((*token)->next)
		{
			if ((*token)->next->next)
			{
				print_error("cd", NULL, "too many arguments");
				return ;
			}
			cd((*token)->next->element, data);
		}
		else
			cd(NULL, data);
	}
	else if (ft_strcmp((*token)->element, "exit") == 0)
	{
		if ((*token)->next)
			exit_bash((*token)->next->element);
		else
			exit_bash(NULL);
	}
}

//Executes the builtin command in the token.
void	execute_bultin(t_token **token, t_data **data)
{
	char	**args;

	args = tokens_to_array(token);
	if (ft_strcmp((*token)->element, "echo") == 0)
		echo(&(*token)->next);
	else if (ft_strcmp((*token)->element, "env") == 0)
		env(args, (*data)->env);
	else if (ft_strcmp((*token)->element, "export") == 0)
		export(args, (*data)->env, (*data)->exp_list);
	else if (ft_strcmp((*token)->element, "pwd") == 0)
		pwd();
	else if (ft_strcmp((*token)->element, "unset") == 0)
		unset(args, (*data)->env, (*data)->exp_list);
	handle_cd_exit(token, data);
	free(args);
}
