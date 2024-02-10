/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 22:28:00 by maxborde          #+#    #+#             */
/*   Updated: 2024/02/10 17:27:16 by maxborde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//Sorts the linked list expvars alphabetically using a bubble sort algorithm.
void	sort_alphabetically(t_env_list **expvars, int size)
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
	flag = 0;
	newvariable = malloc(sizeof(char) * (ft_strlen(variable) + 3));
	if (!ft_strchr(variable, '='))
		return (variable);
	while(variable[i])	
	{
		newvariable[j] = variable[i];
		if (variable[i] == '=' && !flag)
		{
			newvariable[++j] = '"';
			flag = 1;
		}
		i++;
		j++;
	}
	newvariable[++i] = '"';
	newvariable[++i] = 0;
	return (newvariable);
}

//Appends the declare -x prefix to every variables and inserts the quotes around the value
//of the variable.
char	*append_declare_prefix_and_quotes(char *variable)
{
	char	*newvariable;
	char	*tmp;

	newvariable = insert_quotes(variable);
	tmp = variable;
	newvariable = ft_strjoin("declare -x ", newvariable);
	return (newvariable);
}

//Returns the variable listed printed when you call export with no arguments.
//It makes a copy of the dupenv, then sorts it alphabetically and adds the -x prefix.
t_env_list	**get_export_variables(t_env_list **env)
{
	t_env_list	**export_variables;
	t_env_list	*tmp;	
	char	*newvariable;
	
	export_variables = malloc(sizeof(t_env_list *));
	*export_variables = NULL;
	tmp = *env;
	while (tmp)
	{
		newvariable = append_declare_prefix_and_quotes(tmp->variable);
		lst_add_back(export_variables, lst_new(ft_strdup(newvariable)));
		tmp = tmp->next;
	}
	return (export_variables);
}

//This function will return the extracted variable name from arg (with the = if there is one).
//Will return NULL if this is not a valid variable name. It does so by calculating the size of 
//the var name (= included), then it does a duplication of it and at the same time it checks
//if the varname is valid.
char	*extract_var_name(char *arg)
{
	int	len;
	int	i;
	char	*varname;

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
	while (i < len)
	{
		if(!ft_isalpha(arg[i]) && !ft_isdigit(arg[i]) && arg[i] != '_' && arg[i] != '=')	
			return (free(varname), NULL);
		if (arg[i] == '=' && i < len - 1)
			return (free(varname), NULL);
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

	printf("arg = %s\n", arg);
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
	free(var_name);
	return(2);
}

//This function replaces the value of a variable in the env and in the export list.
//It does so by by extracting the varname, finding if there is a match in the env and export
//lists, and if so replacing the variable at the corresponding node by the arg (the full
//expression like HOME=/newhome for example.
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
	free(varname);
}

//Prints the error message in case of a unvalid identifier just like bash.
void	ft_put_unvalidvar_error(char *arg)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);

}

//This function will go trough all of the args passed to export so they can be added to
//the env list and the export list. To do so, we first check if the variable exists in the 
//lists so we can replace the value. If it doesn't exist, we add it to the two lists.
//We have to keep in mind that you can export variable without values and it will show
//in the export list but not in the env list. Also, if we export a variable that already
//exists, but we don't give it a value, it will not replace the one that already exists. 
//VAR UNDEFINED is when variable is not in env and export list.
//VAR DEFINED is when the variable is already in env and export list.
//VAR INVALID is when the variable name is not a valid one in bash.
void	add_variables_to_env(t_env_list **env, t_env_list **export_variables, char *arg)
{
	t_exportcases	cases;	

	if (arg)
	{
		cases = check_var(arg + 11, export_variables);
		if (cases == VAR_DEFINED)
		{
			if (ft_strchr(arg, '='))
				replace_variable_value(env, export_variables, arg);
		}
		else if (cases == VAR_UNDEFINED)
		{
			if (!ft_strchr(arg, '='))
			{
				lst_add_back(export_variables, lst_new(ft_strdup(arg)));
				lst_add_back(env, lst_new(ft_strdup(arg)));
			}
			else
			{
				printf("Hello\n");
				lst_add_back(export_variables, lst_new(ft_strdup(arg)));
				lst_add_back(env, lst_new(ft_strdup(arg)));
			}
		}
		else if (cases == VAR_INVALID)
			ft_put_unvalidvar_error(arg);
		printf("Hxllo\n");
	}
}

void	print_export(t_env_list **export_variables)
{
	t_env_list	*tmp;

	tmp = *export_variables;
	if (!tmp)
		return;
	while (tmp)
	{
		ft_putstr_fd(tmp->variable, 1);
		write(1, "\n", 1);
		tmp = tmp->next;
	}
}

void	export(char **args, t_env_list **env, t_env_list **exp_list)
{
	char	*newvariable;

	if (*(++args) == NULL)
	{
		sort_alphabetically(exp_list, lst_size(exp_list));
		print_export(exp_list);
	}
	while(*args)
	{
		newvariable = append_declare_prefix_and_quotes(*args); 
		add_variables_to_env(env, exp_list, newvariable);
		args++;
	}
	sort_alphabetically(exp_list, lst_size(exp_list));
}
