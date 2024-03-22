/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_to_value.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 18:43:05 by maxborde          #+#    #+#             */
/*   Updated: 2024/03/22 16:46:04 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_var_value(char *var_name, t_env_list **env)
{
	t_env_list	*tmp;
	char		*var_value;
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

char	*extract_var_name_from_ele(char *var_in_element)
{
	char	*var_name;
	int		var_len;

	var_len = 1;
	while (ft_isdigit(var_in_element[var_len])
		|| ft_isalpha(var_in_element[var_len])
		|| var_in_element[var_len] == '_')
		var_len++;
	var_name = ft_strndup(var_in_element, var_len);
	return (var_name);
}

void	convert_var_into_value(char *element_at_var, char *new_element,
		t_env_list **env, int index[2])
{
	char	*var_name;
	char	*var_value;

	var_name = extract_var_name_from_ele(element_at_var);
	var_value = get_var_value(var_name, env) + ft_strlen(var_name);
	if (var_value - ft_strlen(var_name))
	{
		ft_strlcpy(new_element, var_value, ft_strlen(var_value) + 1);
		index[1] += ft_strlen(var_value);
	}
	else
		*new_element = 0;
	index[0] += ft_strlen(var_name);
	free(var_value - ft_strlen(var_name));
	free(var_name);
}

void	convert_exit_status_into_value(char *new_element, int index[2])
{
	int		exit_status_len;
	char	*exit_status_str;

	exit_status_str = ft_itoa(g_status);
	exit_status_len = ft_strlen(exit_status_str);
	ft_strlcpy(new_element, exit_status_str, exit_status_len + 1);
	index[0] += 2;
	index[1] += exit_status_len;
	free(exit_status_str);
}
