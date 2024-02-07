/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxborde <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 00:26:30 by maxborde          #+#    #+#             */
/*   Updated: 2024/02/07 12:47:42 by maxborde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//Sorts the double array expvars alphabetically using a bubble sort algorithm.
void	sorting_alphabetically(char **expvars, int size)
{
	int	i;
	int	j;
	char	*tmp;

	i = 0;
	while (i < size)
	{
		j = 0;
		while (j < size - i - 1)
		{
			if (ft_strcmp(expvars[j], expvars[j + 1]) > 0)
			{
				tmp = expvars[j];
				expvars[j] = expvars[j + 1];
				expvars[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

//Appends the declare -x prefix to every variables.
void 	append_declare_prefix(char **expvars)
{
	while (*expvars)
	{
		*expvars = ft_strjoin("declare -x ", *expvars);
		expvars++;
	}
}

//Returns the variable listed printed when you call export with no arguments.
//It makes a copy of the dupenv, then sorts it alphabetically and adds the -x prefix.
char	**get_export_variables(char **env)
{
	char	**export_variables;	
	int	size;
	
	size = 0;
	while(env[size])
		size++;
	export_variables = malloc(sizeof(char *) * (size + 1));
	size = 0;
	while (env[size])
	{
		export_variables[size] = ft_strdup(env[size]);
		size++;
	}
	export_variables[size] = 0;
	sorting_alphabetically(export_variables, size);
	append_declare_prefix(export_variables);
	return (export_variables);
}

void	export(char **args, char **env)
{
	//export without any arguments list all the env variables in alphabetical order.
	char	**exp_vars;

	exp_vars = get_export_variables(env);
	args = 0;
	printf("PRINT export\n\n\n");
	while (*exp_vars)
	{
		printf("%s\n", *exp_vars);
		exp_vars++;
	}
	//export with just a variable name just adds the variable to the export list but with no value. (variables with no value are not showed in the env, but in the export list).
	//export with a variablename=value will add it to the export list and the env.
	//export can work with multiple arguments.
}
