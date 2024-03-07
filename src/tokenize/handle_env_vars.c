/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_env_vars.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 18:43:05 by maxborde          #+#    #+#             */
/*   Updated: 2024/03/07 04:16:45 by maxborde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_var_value(char *var_name, t_env_list **env)
{
	t_env_list	*tmp;
	char	*var_value;
	int			bytestocmp;

	tmp = *env;
	while (tmp)
	{
		bytestocmp = compute_bytes_to_cmp(tmp->variable, var_name + 1);
		if (ft_strncmp(var_name + 1, tmp->variable, bytestocmp) == 0)
			break ;
		tmp = tmp->next;
	}
	if (tmp)
		var_value = ft_strdup(tmp->variable);
	else
		var_value = NULL;
	return (var_value);
}

static char	*extract_var_name_from_ele(char *var_in_element)
{
	char	*var_name;
	int	var_len;

	var_len = 1;
	while (ft_isdigit(var_in_element[var_len]) || ft_isalpha(var_in_element[var_len]) 
			|| var_in_element[var_len] == '_')
		var_len++;
	var_name = ft_strndup(var_in_element, var_len);
	return (var_name);
}

static int	compute_value_len(char *element_at_var, t_env_list **env, int *i)
{
	char	*var_name;
	char	*var_value;
	int	value_len;

	var_name = extract_var_name_from_ele(element_at_var);
	var_value = get_var_value(var_name, env) + ft_strlen(var_name);
	value_len = 0;
	if (var_value - ft_strlen(var_name))
	{
		*i += ft_strlen(var_name);
		value_len += ft_strlen(var_value);
	}
	else
	{
		value_len++;
		*i += 1;
	}
	free(var_value - ft_strlen(var_name));
	free(var_name);
	return (value_len);
}


int	compute_new_element_len(char *element, t_env_list **env)
{
	int	new_element_len;
	int	i;

	new_element_len = 0;
	i = 0;
	while (element[i])
	{
		if (element[i] == '$')		
			new_element_len += compute_value_len(&element[i], env, &i);
		else
		{
			i++;
			new_element_len++;
		}
	}
	return (new_element_len);
}

void	convert_var_into_value(char *element_at_var, char *new_element, t_env_list **env, int *i, int *j)
{
	char	*var_name;
	char	*var_value;

	var_name = extract_var_name_from_ele(element_at_var);
	var_value = get_var_value(var_name, env) + ft_strlen(var_name);
	if (var_value - ft_strlen(var_name))
	{
		ft_strlcpy(new_element, var_value, ft_strlen(var_value) + 1);
		*j += ft_strlen(var_value);
	}
	else
		*new_element = 0;
	*i += ft_strlen(var_name);
	free(var_value - ft_strlen(var_name));
	free(var_name);
}

