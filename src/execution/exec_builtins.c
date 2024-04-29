/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 12:29:08 by ymeziane          #+#    #+#             */
/*   Updated: 2024/04/02 12:29:19 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	bool	expression_next;

	if (ft_strcmp(*expression, "exit") == 0)
	{
		expression_next = (*(expression + 1));
		if (expression_next)
			exit_bash(args, data, tokenlist, expression);
		else
			exit_bash(args, data, tokenlist, expression);
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

static bool	exit_in_single_buitlin(t_token **tokenlist, t_data **data,
		char **args, char **expression)
{
	if (ft_strcmp(*expression, "exit") == 0)
	{
		if (configure_io(tokenlist, 0, data))
			exec_builtins(tokenlist, data, expression, args);
		return (true);
	}
	else
		g_status = 1;
	return (false);
}

bool	check_and_exec_single_builtin(t_token **tokenlist, t_data **data,
		char **args)
{
	char	**expression;
	int		saved_stdout;
	int		saved_stdin;

	expression = args;
	if (type(expression[0], (*data)->env) == BUILTIN && (*data)->nb_pipe == 0)
	{
		if (exit_in_single_buitlin(tokenlist, data, args, expression))
			return (true);
		saved_stdout = dup(STDOUT_FILENO);
		saved_stdin = dup(STDIN_FILENO);
		if (configure_io(tokenlist, 0, data))
		{
			exec_builtins(tokenlist, data, expression, args);
			dup2(saved_stdout, STDOUT_FILENO);
			dup2(saved_stdin, STDIN_FILENO);
		}
		else
			g_status = 1;
		close(saved_stdout);
		close(saved_stdin);
		return (true);
	}
	return (false);
}
