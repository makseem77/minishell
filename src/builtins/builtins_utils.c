/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 17:51:04 by ymeziane          #+#    #+#             */
/*   Updated: 2024/02/14 15:43:36 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// This function goal is to check the var
// and return ints for each different cases.
// We will start by duplicating arg until we find a "="
// or a NULL to extract the variable name.
// We check if the variable name is valid during the duplication.
// We will then check if it exists in the lists. If it doesn't,
// we just add it to both list with its value.
// If it does, we change the variable at the node
// where the old variable was located to the new variable.
int	check_var(char *arg, t_env_list **export_variables, int offset)
{
	char		*var_name;
	char		*varnameinlist;
	t_env_list	*tmp;
	int			bytestocmp;

	var_name = extract_var_name(arg + offset);
	tmp = *export_variables;
	if (!var_name)
		return (0);
	while (tmp)
	{
		varnameinlist = extract_var_name(tmp->variable + offset);
		if (ft_strlen(varnameinlist) > ft_strlen(var_name))
			bytestocmp = ft_strlen(varnameinlist);
		else
			bytestocmp = ft_strlen(var_name);
		if (ft_strncmp(var_name, varnameinlist, bytestocmp) == 0)
		{
			free(varnameinlist);
			return (free(var_name), 1);
		}
		free(varnameinlist);
		tmp = tmp->next;
	}
	return (free(var_name), 2);
}

// This function will return the extracted variable name from arg
// (with the = if there is one).
// Will return NULL if this is not a valid variable name.
// It does so by calculating the size of the var name (= excluded),
// then it does a duplication of it and at the same time it checks
// if the varname is valid.
char	*extract_var_name(char *arg)
{
	int		len;
	int		i;
	char	*varname;

	len = 0;
	i = 0;
	if (!arg || isdigit(*arg) || *arg == '=')
		return (NULL);
	while (arg[len] && arg[len] != '=')
		len++;
	varname = malloc(len + 1);
	while (i < len && arg[i] != '=')
	{
		if (!ft_isalpha(arg[i]) && !ft_isdigit(arg[i]) && arg[i] != '_')
			return (free(varname), NULL);
		varname[i] = arg[i];
		i++;
	}
	varname[i] = 0;
	return (varname);
}

// Appends the declare
//-x prefix to every variables and inserts the quotes around the value
// of the variable.
char	*append_declare_prefix_and_quotes(char *variable)
{
	char	*tmp;
	char	*newvariable;

	tmp = insert_quotes(variable);
	newvariable = ft_strjoin("declare -x ", tmp);
	free(tmp);
	return (newvariable);
}

// This function takes the variable and returns a duplicate with double quotes
// inserted around the value of the variable. If it's a variable with no value
// we just return the variable. Else we allocate memory for 2 quotes and  the
// NULL byte and insert quotes after the = and at the end of the value.
char	*insert_quotes(char *variable)
{
	char	*newvariable;
	int		i;
	int		j;
	int		flag;

	i = 0;
	j = 0;
	flag = 0;
	if (!ft_strchr(variable, '='))
		return (ft_strdup(variable));
	newvariable = malloc(sizeof(char) * (ft_strlen(variable) + 3));
	while (variable[i])
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

// Sorts the linked list expvars alphabetically using a bubble sort algorithm.
void	sort_alphabetically(t_env_list **expvars, int size)
{
	t_env_list	*tmp;
	t_env_list	*head;
	char		*temp;
	int			i;
	int			j;

	i = -1;
	tmp = *expvars;
	head = *expvars;
	while (++i < size)
	{
		j = -1;
		tmp = head;
		while (++j < size - i - 1)
		{
			if (ft_strcmp(tmp->variable, tmp->next->variable) > 0)
			{
				temp = tmp->variable;
				tmp->variable = tmp->next->variable;
				tmp->next->variable = temp;
			}
			tmp = tmp->next;
		}
	}
}
