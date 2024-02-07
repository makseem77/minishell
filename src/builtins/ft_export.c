/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxborde <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 00:26:30 by maxborde          #+#    #+#             */
/*   Updated: 2024/02/08 00:08:55 by maxborde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//Sorts the double array expvars alphabetically using a bubble sort algorithm.
void	sorting_alphabetically(t_env_list **expvars, int size)
{
	t_env_list	*tmp;
	t_env_list	*head;
	char	*temp;
	int	i;
	int	j;

	i = 0;
	tmp = *expvars;
	head = *expvars;
	while (i < size)
	{
		j = 0;
		tmp = head;
		while (j < size - i - 1)
		{
			if (ft_strcmp(tmp->variable, tmp->next->variable) > 0)
			{
				temp = tmp->variable;
				tmp->variable = tmp->next->variable;
				tmp->next->variable = temp;
			}
			j++;
			tmp = tmp->next;
		}
		i++;
	}
}

char	*insert_quotes(char *variable)
{
	char	*newvariable;
	int	i;
	int	j;
	int	flag;

	i = 0;
	j = 0;
	flag = 1;
	newvariable = malloc(sizeof(char) * (ft_strlen(variable) + 3));
	while(variable[i])	
	{
		if (variable[i] == '=' && flag)
		{
			newvariable[i] = variable[i];
			j++;
			newvariable[i + j] = '"';
			i++;
			flag = 0;
		}
		newvariable[i + j] = variable[i];
		i++;
	}
	newvariable[i + j++] = '"';
	newvariable[i + j] = 0;
	return (newvariable);
}

//Appends the declare -x prefix to every variables.
void 	append_declare_prefix_and_quotes(t_env_list **expvars)
{
	t_env_list	*tmp;

	tmp = *expvars;
	while (tmp)
	{
		
		tmp->variable = insert_quotes(tmp->variable);
		tmp->variable = ft_strjoin("declare -x ", tmp->variable);
		tmp = tmp->next;
	}
}

//Returns the variable listed printed when you call export with no arguments.
//It makes a copy of the dupenv, then sorts it alphabetically and adds the -x prefix.
t_env_list	**get_export_variables(t_env_list **env)
{
	t_env_list	**export_variables;
	t_env_list	*tmp;	
	
	export_variables = malloc(sizeof(t_env_list *));
	tmp = *env;
	while (tmp)
	{
		lst_add_back(export_variables, lst_new(ft_strdup(tmp->variable)));
		tmp = tmp->next;
	}
	return (export_variables);
}

int	is_valid_var(char *arg)
{
	int	equalflag;

	equalflag = 0;
	if (isdigit(*arg))
		return (0);
	printf("ARG = %s\n", arg);
	while (*arg)
	{
		printf("GOOD\n");
		if(*arg == '=')
			equalflag = 1;
		if(!equalflag)	
		{
			if((isalpha(*arg) == 0 && *arg != '_'))	
				return (0);
		}
		arg++;
	}
	return(1);
	//when passing an argument to export, the variable name has those rules:
	//Can't start with a number. (but can contain one).
	//So should start with a letter or a _.
	//Anything can go after the =. But you have to remove quotes and double quotes around the expression.
}

void	add_variable_to_env(t_env_list **env, t_env_list **export_variables, char **args)
{
	while (*args)
	{
		if (is_valid_var(*args))
		{
			lst_add_back(export_variables, lst_new(ft_strdup(*args)));
			lst_add_back(env, lst_new(ft_strdup(*args)));
		}
		args++;
	}
}

void	export(char **args, t_env_list **env)
{
	t_env_list	**export_variables;
	t_env_list	*tmp;

	export_variables = get_export_variables(env);
	/*if (!args)
		print_export_variables;
	else */
	add_variable_to_env(env, export_variables, args);
	sorting_alphabetically(export_variables, lst_size(export_variables));
	append_declare_prefix_and_quotes(export_variables);



	printf("EXPORT: \n\n\n\n");
	tmp = *export_variables;
	while (tmp)
	{
		printf("%s\n", tmp->variable);
		tmp = tmp->next;
	}
	printf("ENV: \n\n\n\n");
	tmp = *env;
	while (tmp)
	{
		printf("%s\n", tmp->variable);
		tmp = tmp->next;
	}
	//export with just a variable name just adds the variable to the export list but with no value. (variables with no value are not showed in the env, but in the export list).
	//export with a variablename=value will add it to the export list and the env.
	//export can work with multiple arguments.
}
