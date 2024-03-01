/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 11:32:51 by maxborde          #+#    #+#             */
/*   Updated: 2024/03/01 16:29:01 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

// This function will extract the number of bytes to compare
// from the variable in the list and the variable name.
// We will compare the two strings byte by byte,
// and we will return the number of bytes to compare.
// For example,
// if the variable in the list is "VAR=123" and the variable name is "VAR",
// we will return 3, here the length of the variable name.
// However,
// if the variable in the list is "VAR" and the variable name is "VAR=123",
// we will return 3, here the length of the variable in the list.
// If the variable in the list is longer than the variable name,
// we will return the length of the variable in the list.
// If the variable name is longer than the variable in the list,
// we will return the length of the variable name.
// If the two strings are the same length,
// we will return the length of the variable name.
int	compute_bytes_to_cmp(char *variableinlist, char *variablename)
{
	char	*varnameinlist;
	int		bytestocmp;

	if (!variablename)
		return (0);
	varnameinlist = extract_var_name(variableinlist);
	if (ft_strlen(varnameinlist) > ft_strlen(variablename))
	{
		bytestocmp = ft_strlen(varnameinlist);
		free(varnameinlist);
		return (bytestocmp);
	}
	else
	{
		bytestocmp = ft_strlen(variablename);
		free(varnameinlist);
		return (bytestocmp);
	}
}

// This function will check if the variable is in the list.
static bool	arg_in_list(char *arg, t_env_list **lst, int offset)
{
	t_env_list	*tmp;
	char		*var_name;
	int			bytestocmp;

	tmp = *lst;
	var_name = extract_var_name(arg);
	while (tmp)
	{
		bytestocmp = compute_bytes_to_cmp(tmp->variable + offset, var_name);
		if (ft_strncmp(tmp->variable + offset, var_name, bytestocmp) == 0)
		{
			free(var_name);
			return (true);
		}
		tmp = tmp->next;
	}
	free(var_name);
	return (false);
}

// This function will check if the variable is valid.
// A variable is valid if it starts with a letter or an underscore,
// and if it contains only letters, numbers and underscores.
static bool	is_valid_variable(char *variable)
{
	int	i;

	i = 0;
	if (!ft_isalpha(variable[0]) || variable[0] == '_')
		return (false);
	while (variable[i])
	{
		if (!ft_isalnum(variable[i]) && variable[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

// This function will go trough all of the args passed to unset
// so they can be removed from the env list and the export list.
// We first check if the variable is a valid one, and if it's not,
// we print an error.
// If the variable is valid, we check if it exists in the lists.
// If it does, we remove it from the lists.
void	unset(char **args, t_env_list **env, t_env_list **exp_list)
{
	while (*args)
	{
		if (!(is_valid_variable(*args)))
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(*args, 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
		}
		if (arg_in_list(*args, env, 0))
			lst_del_one(env, *args, 0);
		if (arg_in_list(*args, exp_list, ft_strlen("declare -x ")))
			lst_del_one(exp_list, *args, ft_strlen("declare -x "));
		args++;
	}
}
