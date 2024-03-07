/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_env_vars.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 18:43:05 by maxborde          #+#    #+#             */
/*   Updated: 2024/03/07 01:26:53 by maxborde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_var_value(char *var_name, t_env_list **env)
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
