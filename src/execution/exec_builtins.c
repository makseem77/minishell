/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 17:23:46 by ymeziane          #+#    #+#             */
/*   Updated: 2024/03/26 15:57:44 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	count_tokens(t_token **token)
{
	int		i;
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

// Returns the tokens in the tokenlist as an array of strings.
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

static void	handle_cd_exit(t_token **tokenlist, t_data **data,
		char **expression, char **args)
{
	char *expression_next;

	expression_next = NULL;
	if(*(expression + 1))
		expression_next = ft_strdup(*(expression + 1));
	if (ft_strcmp(*expression, "cd") == 0)
	{
		free(expression_next);
		if (*(expression + 1))
		{
			if (*(expression + 2))
			{
				print_error("cd", NULL, "too many arguments");
				return ;
			}
			cd(*(expression + 1), data);
		}
		else
			cd(NULL, data);
	}
	else if (ft_strcmp(*expression, "exit") == 0)
	{
		free_double_array(expression);
		if (expression_next)
			exit_bash(expression_next, data, tokenlist, args);
		else
			exit_bash(NULL, data, tokenlist, args);
	}
}

// Executes the builtin command in the token.
void	execute_bultin(t_token **tokenlist, t_data **data, char **expression,
		char **args)
{
	g_status = 0;
	if (ft_strcmp(expression[0], "echo") == 0)
		echo(expression);
	else if (ft_strcmp(expression[0], "env") == 0)
		env(expression, (*data)->env);
	else if (ft_strcmp(expression[0], "export") == 0)
		export(expression, (*data)->env, (*data)->exp_list);
	else if (ft_strcmp(expression[0], "pwd") == 0)
		pwd();
	else if (ft_strcmp(expression[0], "unset") == 0)
		unset(expression, (*data)->env, (*data)->exp_list);
	handle_cd_exit(tokenlist, data, expression, args);
}
