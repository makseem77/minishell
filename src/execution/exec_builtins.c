/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 17:23:46 by ymeziane          #+#    #+#             */
/*   Updated: 2024/02/26 02:49:41 by maxborde         ###   ########.fr       */
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
	char	**temp;
	t_token	*tmp;
	int		i;

	printf("IN BUILTIN:\n PID = %ld\nPPID = %ld\n\n\n\n", (long)getpid(), (long)getppid());
	tmp = *tokenlist;
	i = 0;
	while (tmp && ft_strcmp(tmp->element, cmd) != 0)
	{
		i++;
		tmp = tmp->next;
	}
	temp = tokens_to_array(&tmp);
	args = cut_args_at_pipe(temp);
	if (ft_strcmp(tmp->element, "echo") == 0)
		echo(args);
	else if (ft_strcmp(tmp->element, "env") == 0)
		env(args, (*data)->env);
	else if (ft_strcmp(tmp->element, "export") == 0)
		export(args, (*data)->env, (*data)->exp_list);
	else if (ft_strcmp(tmp->element, "pwd") == 0)
		pwd();
	else if (ft_strcmp(tmp->element, "unset") == 0)
		unset(args, (*data)->env, (*data)->exp_list);
	handle_cd_exit(tokenlist, args, data);
	free_double_array(args);
	free(temp);
}
