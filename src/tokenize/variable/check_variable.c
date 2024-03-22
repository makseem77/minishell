/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_variable.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 12:01:24 by ymeziane          #+#    #+#             */
/*   Updated: 2024/03/22 12:36:03 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	compute_value_len(char *element_at_var, t_env_list **env, int *i)
{
	char	*var_name;
	char	*var_value;
	int		value_len;

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

static int	compute_new_element_len(char *element, t_env_list **env)
{
	int		new_element_len;
	char	*tmp;
	int		i;

	new_element_len = 0;
	i = 0;
	while (element[i])
	{
		if (ft_strncmp(&element[i], "$?", 2) == 0)
		{
			tmp = ft_itoa(g_status);
			new_element_len += ft_strlen(tmp);
			free(tmp);
			i += 2;
		}
		else if (element[i] == '$')
			new_element_len += compute_value_len(&element[i], env, &i);
		else
		{
			i++;
			new_element_len++;
		}
	}
	return (new_element_len);
}

char	*convert_element(char *element, t_env_list **env)
{
	char	*new_element;
	int		index[2];

	new_element = malloc(sizeof(char) * (compute_new_element_len(element, env)
				+ 1));
	index[0] = 0;
	index[1] = 0;
	while (element[index[0]])
	{
		if (ft_strncmp(&element[index[0]], "$?", 2) == 0)
			convert_exit_status_into_value(&new_element[index[1]], index);
		else if (element[index[0]] == '$' && (isalpha(element[index[0] + 1])
				|| isdigit(element[index[0] + 1])
				|| element[index[0] + 1] == '_'))
			convert_var_into_value(&element[index[0]], &new_element[index[1]],
				env, index);
		else
			new_element[index[1]++] = element[index[0]++];
	}
	new_element[index[1]] = 0;
	free(element);
	return (new_element);
}

// Will return 1 if the element is or has a variable ($VAR), 0 if
// it is not and -1 if we have unclosed quotes.
// "$VAR" is interpreted as a variable where '$VAR' is not.
int	is_or_has_a_variable(char *element)
{
	int	dquotesflag;
	int	squotesflag;
	int	i;

	dquotesflag = 0;
	squotesflag = 0;
	i = 0;
	while (element[i])
	{
		if (element[i] == '$' && (squotesflag % 2 == 0)
			&& element[i + 1] == '?')
			return (1);
		if (element[i] == '$' && (squotesflag % 2 == 0)
			&& (isalpha(element[i + 1]) || isdigit(element[i + 1])
				|| element[i + 1] == '_'))
			return (1);
		if (element[i] == '\'' && dquotesflag % 2 == 0)
			squotesflag++;
		if (element[i] == '"' && squotesflag % 2 == 0)
			dquotesflag++;
		i++;
	}
	return (0);
}
