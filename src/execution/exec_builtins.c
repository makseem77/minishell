/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 17:23:46 by ymeziane          #+#    #+#             */
/*   Updated: 2024/03/28 09:47:37 by ymeziane         ###   ########.fr       */
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

static void	handle_cd(t_data **data, char **expression)
{
	char	*expression_next;

	expression_next = NULL;

	if (ft_strcmp(*expression, "cd") == 0)
	{
		if (*(expression + 1))
			expression_next = ft_strdup(*(expression + 1));
		if (expression_next)
		{
			if (*(expression + 2))
			{
				print_error("cd", NULL, "too many arguments");
				free(expression_next);
				return ;
			}
			cd(*(expression + 1), data);
		}
		else
			cd(NULL, data);
	}
	free(expression_next);
}

static void	handle_exit(t_token **tokenlist, t_data **data, char **expression,
		char **args)
{
	char	*expression_next;
	bool	too_many_args;

	expression_next = NULL;
	if (ft_strcmp(*expression, "exit") == 0)
	{
		if (*(expression + 1))
			expression_next = ft_strdup(*(expression + 1));
		too_many_args = (args && args[2]);
		if (!too_many_args)
			free_double_array(args);
		if (expression_next)
			exit_bash(expression_next, data, tokenlist, too_many_args);
		else
			exit_bash(NULL, data, tokenlist, too_many_args);
	}
}

// Executes the builtin command in the token.
void	exec_builtins(t_token **tokenlist, t_data **data, char **expression,
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
	handle_cd(data, expression);
	handle_exit(tokenlist, data, expression, args);
	if ((*data)->nb_pipe > 0)
	{
		free_after_execution(tokenlist, data, args, expression);
		exit(g_status);
	}
}
