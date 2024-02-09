/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 00:26:30 by maxborde          #+#    #+#             */
/*   Updated: 2024/02/09 17:46:20 by ymeziane         ###   ########.fr       */
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

//This function takes the variable and returns a duplicate with double quotes
//inserted around the value of the variable. If it's a variable with no value 
//we just return the variable. Else we allocate memory for 2 quotes and  the 
//NULL byte and insert quotes after the = and at the end of the value.
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
	if (!ft_strchr(variable, '='))
		return (variable);
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
		if (variable[i])
		{
			newvariable[i + j] = variable[i];
			i++;
		}
	}
	if (!flag)
	{
		newvariable[i + j++] = '"';
	}
	newvariable[i + j] = 0;
	printf("NVariable = %s\n", newvariable);
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
	if(!export_variables)
		return (NULL);
	*export_variables = NULL;
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
char	*extract_var_name(char *arg)
{
	int	equalflag;
	int	len;
	int	i;
	char	*varname;

	equalflag = 0;
	len = 0;
	i = 0;
	if (isdigit(*arg) || *arg == '=')
		return (NULL);
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

	var_name = extract_var_name(arg);
	tmp = *export_variables;
	if (!var_name)
		return (0);
	while (tmp)
	{
		if (ft_strncmp(var_name, tmp->variable, ft_strlen(var_name)) == 0)
			return (1);
		tmp = tmp->next;
	}
	return(2);
}

void	replace_variable_value(t_env_list **env, t_env_list **export_variables, char *arg)
{
	t_env_list	*tmp;
	char	*varname;

	tmp = *env;
	varname = extract_var_name(arg);
	while (tmp)
	{
		if (ft_strncmp(varname, tmp->variable, ft_strlen(varname)) == 0)
			tmp->variable = arg;
		tmp = tmp->next;
	}
	tmp = *export_variables;
	while (tmp)
	{
		if (ft_strncmp(varname, tmp->variable, ft_strlen(varname)) == 0)
			tmp->variable = arg;
		tmp = tmp->next;
	}
}
//This function will go trough all of the args passed to export so they can be added to
//the env list and the export list. To do so, we first check if the variable exists in the 
//lists so we can replace the value. If it doesn't exist, we add it to the two lists.
//We have to keep in mind that you can export variable without values and it will show
//in the export list but not in the env list. Also, if we export a variable that already
//exists, but we don't give it a value, it will not replace the one that already exists. 
//CASE UNDEFINED is when variable is not in env and export list.
//CASE DEFINED is when the variable is already in env and export list.
void	add_variable_to_env(t_env_list **env, t_env_list **export_variables, char **args)
{
	t_exportcases	cases;	

	while (*args)
	{
		cases = check_var(*args, export_variables);
		printf("ARG = %s\n", *args);
		if (cases == VAR_UNDEFINED)
		{
			if (ft_strchr(*args, '='))
				replace_variable_value(env, export_variables, *args);
		}
		else if (cases == VAR_DEFINED)
		{
			if (!ft_strchr(*args, '='))
				lst_add_back(export_variables, lst_new(ft_strdup(*args)));
			else
			{
				lst_add_back(export_variables, lst_new(ft_strdup(*args)));
				lst_add_back(env, lst_new(ft_strdup(*args)));
			}
		}
		args++;
	}
}

void	export(char **args, t_env_list **env)
{
	t_env_list	**export_variables;
	export_variables = get_export_variables(env);
	add_variable_to_env(env, export_variables, args);
	sorting_alphabetically(export_variables, lst_size(export_variables));
	append_declare_prefix_and_quotes(export_variables);



	printf("EXPORT: \n\n\n\n");
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
	}
	//export with just a variable name just adds the variable to the export list but with no value. (variables with no value are not showed in the env, but in the export list).
	//export with a variablename=value will add it to the export list and the env.
	//export can work with multiple arguments.
}
