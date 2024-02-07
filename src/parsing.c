/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 15:23:35 by ymeziane          #+#    #+#             */
/*   Updated: 2024/02/07 13:19:14 by maxborde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	is_a_built_in(char *element)
{
	if (!ft_strcmp("echo", element) || !ft_strcmp("cd", element) ||
		!ft_strcmp("pwd", element) || !ft_strcmp("export", element)
			|| !ft_strcmp("unset", element) || !ft_strcmp("env", element)
			|| !ft_strcmp("exit", element))
		return (1);
	return (0);
}

int	is_a_meta_char(char *element)
{
	if (!ft_strcmp(">", element) || !ft_strcmp(">>", element) ||
		!ft_strcmp("<", element) || !ft_strcmp("<<", element) || !ft_strcmp("&",
				element) || !ft_strcmp("|", element) || !ft_strcmp("&&",
				element) || !ft_strcmp("||", element) || !ft_strcmp(",",
				element) || !ft_strcmp("(", element)
			|| !ft_strcmp(")", element))
		return (1);
	return (0);
}

//Returns 1 if it's element is actually an executable command,
//0 if it is not.
//Here, we go trough the splitted PATH variables, and join element
//to each of them, then check if they are actually executable.
//We also do a first check with open to make sure that we are not
//using access, X_OK on a directory because it would return 0 like
//an exe.
int	is_a_command(char *element, char **env)
{
	char	*executable;
	int	fd;
	char **bin_paths;

	bin_paths = find_bin_paths(env);
	executable = NULL;
	while (*bin_paths)
	{
		executable = ft_strjoin(*bin_paths, "/");
		executable = ft_strjoin(executable, element);
		fd = open(executable, O_DIRECTORY);
		if (fd == -1) 
		{
			if (access(executable, X_OK) == 0)
				return (1);
		}
		else
			close(fd);
		bin_paths++;
	}
	return (0);
}

//Will return 1 if the element is or has a variable ($VAR), 0 if 
//it is not and -1 if we have unclosed quotes. 
//"$VAR" is interpreted as a variable where '$VAR' is not.
int has_a_variable(char *element)
{
	int	dquotesflag;
	int	squotesflag;
	int	i;

	dquotesflag = 0;
	squotesflag = 0;
	i = 0;
	printf("This is the ele= %s\n", element);
	while (element[i])
	{
		if (element[i] == '$' && (squotesflag % 2 == 0) && 
				(isalpha(element[i + 1]) || isdigit(element[i + 1]) || element[i + 1] == '_'))
			return(1);
		if(element[i] == '\'' && dquotesflag % 2 == 0)
			squotesflag++;
		if(element[i] == '"' && squotesflag % 2 == 0)
			dquotesflag++;
		i++;
	}
	return (0);
}

//Goes trough the token linked list and gives a tokentype to every node of the list.
void	set_token_types(t_token **tokenlist, char **env	)
{
	t_token	*tmp;

	tmp = *tokenlist;
	while (tmp)
	{
		if (is_a_built_in(tmp->element))
			tmp->ttype = BUILTIN;
		else if (is_a_meta_char(tmp->element))
			tmp->ttype = META_CHAR;
		else if(is_a_command(tmp->element, env))
			tmp->ttype = COMMAND;
		else
		{
			printf("%s: command not found\n", tmp->element);
			exit(EXIT_FAILURE);
		}
		tmp = tmp->next;
	}
}
