/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 22:28:00 by maxborde          #+#    #+#             */
/*   Updated: 2024/02/14 11:18:21 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Returns the variable listed printed when you call export with no arguments.
// It makes a copy of the dupenv, then sorts it alphabetically and adds the
//-x prefix.
t_env_list	**get_export_variables(t_env_list **env)
{
	t_env_list	**export_variables;
	t_env_list	*tmp;
	char		*newvariable;

	export_variables = malloc(sizeof(t_env_list *));
	*export_variables = NULL;
	tmp = *env;
	while (tmp)
	{
		if (ft_strncmp(tmp->variable, "_", 1) != 0)
		{
			newvariable = append_declare_prefix_and_quotes(tmp->variable);
			lst_add_back(export_variables, lst_new(ft_strdup(newvariable)));
			free(newvariable);
		}
		tmp = tmp->next;
	}
	return (export_variables);
}

// This function replaces the value of a variable in the env
// and in the export list.
// It does so by by extracting the varname,
// finding if there is a match in the env and export lists,
// and if so replacing the variable at the corresponding node by the arg
// (the full expression like HOME=/newhome for example).
static void	replace_variable_value(t_env_list **list, char *arg, int offset)
{
	t_env_list	*tmp;
	char		*varname;
	char		*varnameinlist;
	int			bytestocmp;

	tmp = *list;
	varname = extract_var_name(arg + offset);
	while (tmp)
	{
		varnameinlist = extract_var_name(tmp->variable + offset);
		bytestocmp = compute_bytes_to_cmp(varnameinlist, varname);
		if (ft_strncmp(varname, varnameinlist, bytestocmp) == 0)
		{
			free(tmp->variable);
			tmp->variable = arg;
		}
		tmp = tmp->next;
		free(varnameinlist);
	}
	free(varname);
}

// This function will go trough all of the args passed to export
// so they can be added to the env list and the export list.
// To do so, we first check if the variable exists in the lists
// so we can replace the value.
// If it doesn't exist, we add it to the two lists.
// We have to keep in mind that you can export variable without values
// and it will show in the export list but not in the env list.
// Also, if we export a variable that already exists,
// but we don't give it a value,
// it will not replace the one that already exists.
// VAR UNDEFINED is when variable is not in env and export list.
// VAR DEFINED is when the variable is already in env and export list.
// VAR INVALID is when the variable name is not a valid one in bash.
static void	add_variables_to_export(t_env_list **export_variables, char *arg)
{
	t_exportcases	cases;

	if (arg)
	{
		cases = check_var(arg, export_variables, 11);
		if (cases == VAR_DEFINED)
		{
			if (ft_strchr(arg, '='))
				replace_variable_value(export_variables, ft_strdup(arg), 11);
		}
		else if (cases == VAR_UNDEFINED)
		{
			if (!ft_strchr(arg, '='))
				lst_add_back(export_variables, lst_new(ft_strdup(arg)));
			else
				lst_add_back(export_variables, lst_new(ft_strdup(arg)));
		}
	}
}

// This function will add the variables to the env list.
// It will check if the variable is already in the list,
// and if so, it will replace the value.
// If the variable is not in the list, it will add it to the list.
// If the variable is not a valid one, it will print an error.
static void	add_variables_to_env(t_env_list **env, char *arg)
{
	t_exportcases	cases;

	if (arg)
	{
		cases = check_var(arg, env, 0);
		if (cases == VAR_DEFINED)
		{
			if (ft_strchr(arg, '='))
				replace_variable_value(env, ft_strdup(arg), 0);
		}
		else if (cases == VAR_UNDEFINED)
		{
			if (ft_strchr(arg, '='))
				lst_add_back(env, lst_new(ft_strdup(arg)));
		}
		else if (cases == VAR_INVALID)
		{
			if (arg[0] == '\0')
				print_error("export", "`'", "not a valid identifier");
			else
				print_error("export", arg, "not a valid identifier");
		}
	}
}

// If export is called on its own,
// this function will print the export list in the order of the alphabet.
// It will also print the declare -x prefix and the quotes.
// else, it will add the variables to the export list and the env list.
void	export(char **args, t_env_list **env, t_env_list **exp_list)
{
	char	*newvariable;

	if (*(++args) == NULL)
	{
		sort_alphabetically(exp_list, lst_size(exp_list));
		print_export(exp_list);
	}
	while (*args)
	{
		if (ft_strncmp(*args, "_", 1) != 0)
		{
			newvariable = append_declare_prefix_and_quotes(*args);
			add_variables_to_export(exp_list, newvariable);
			free(newvariable);
			add_variables_to_env(env, *args);
		}
		args++;
	}
	sort_alphabetically(exp_list, lst_size(exp_list));
}
