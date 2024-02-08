/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxborde <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 00:26:30 by maxborde          #+#    #+#             */
/*   Updated: 2024/02/08 17:13:08 by maxborde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//Sorts the linked list expvars alphabetically using a bubble sort algorithm.
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
	if (!flag)
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

//This function will return the extracted variable name from arg (with the = if there is one).
//Will return NULL if this is not a valid variable name.
char	*extract_var(char *arg)
{
	int	equalflag;
	int	len;
	int	i;
	char	*varname;

	equalflag = 0;
	len = 0;
	i = 0;
	if (isdigit(*arg))
		return (0);
	while(arg[len])
	{
		len++;
		if (arg[len] == '=')
		{
			len++;
			break;
		}
	}
	varname = malloc(len + 1);
	while (i < len + equalflag)
	{
		if(!ft_isalpha(arg[i]) && !ft_isdigit(arg[i]) && arg[i] != '_' && arg[i] != '=')	
			return (NULL);
		if (arg[i] == '=' && i < len - 1)
			return (NULL);
		varname[i] = arg[i];	
		i++;
	}
	varname[i] = 0;
	return (varname);	
}

//This function goal is to check the var and return ints for each different cases.
//We will start by duplicating arg until we find a "=" or a NULL to extract the variable
//name. We check if the variable name is valid during the duplication. 
//We will then check if it exists in the lists. If it doesn't, we just add it to both list with
//its value. If it does, we change the variable at the node where the old variable was located to
//the new variable.
int	check_var(char *arg, t_env_list **export_variables)
{
	char	*var_name;
	t_env_list	*tmp;

	var_name = extract_var(arg);
	tmp = *export_variables;
	if (!var_name)
		return (0);
	printf("VAR_NAME = %s\n\n\n\n", var_name);
	while (tmp)
	{
		if (ft_strncmp(var_name, tmp->variable, ft_strlen(var_name)) == 0)
			return (1);
		tmp = tmp->next;
	}
	return(2);
}

//This function will go trough all of the args passed to export so they can be added to
//the env list and the export list. To do so, we first check if the variable exists in the 
//lists so we can replace the value. If it doesn't exist, we add it to the two lists.
//We have to keep in mind that you can export variable without values and it will show
//in the export list but not in the env list. Also, if we export a variable that already
//exists, but we don't give it a value, it will not replace the one that already exists. 
//CASE 1 is when the variable is already in env and export list.
//CASE 2 is when variable is not in env list.
//CASE 3 is when variable has no value (will only show in export).
void	add_variable_to_env(t_env_list **env, t_env_list **export_variables, char **args)
{
	int	cases;	

	while (*args)
	{
		cases = check_var(*args, export_variables);
		if (cases == 1)
		{
			if (ft_strchr(*args, '='));
			{
				//Nothing to do, variable with no name.
			}
			else
			{
				//replace the var with the new variable.
			}
			printf("MATCH !\n");
		}
		else if (cases == 2)
		{
			printf("NO MATCH !\n");	
			lst_add_back(export_variables, lst_new(ft_strdup(*args)));
			lst_add_back(env, lst_new(ft_strdup(*args)));
		}
		args++;
	}
}

void	export(char **args, t_env_list **env)
{
	t_env_list	**export_variables;

	export_variables = get_export_variables(env);
	/*if (!args)
		print_export_variables;
	else */
	add_variable_to_env(env, export_variables, args);
	sorting_alphabetically(export_variables, lst_size(export_variables));
	append_declare_prefix_and_quotes(export_variables);



	/*printf("EXPORT: \n\n\n\n");
	t_env_list	*tmp;
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
	}*/
	//export with just a variable name just adds the variable to the export list but with no value. (variables with no value are not showed in the env, but in the export list).
	//export with a variablename=value will add it to the export list and the env.
	//export can work with multiple arguments.
}
