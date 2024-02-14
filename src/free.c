/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 22:17:47 by maxborde          #+#    #+#             */
/*   Updated: 2024/02/13 11:27:07 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_variable_lists(t_env_list **export_list, t_env_list **env_list)
{
	t_env_list	*tmp;
	t_env_list	*tmp2;

	tmp = *export_list;
	while (tmp)
	{
		tmp2 = tmp;
		if (tmp->variable)
			free(tmp->variable);
		tmp = tmp->next;
		free(tmp2);
	}
	free(export_list);
	tmp = *env_list;
	while (tmp)
	{
		tmp2 = tmp;
		if (tmp->variable)
			free(tmp->variable);
		tmp = tmp->next;
		free(tmp2);
	}
	free(env_list);
}

void	free_token_list(t_token **token)
{
	t_token	*tmp;
	t_token	*tmp2;

	tmp = *token;
	while (tmp)
	{
		tmp2 = tmp;
		if (tmp->element)
			free(tmp->element);
		tmp = tmp->next;
		free(tmp2);
	}
	free(token);
}

void	free_data_struct(t_data *data)
{
	free(data->old_pwd);
	free(data->home_dir);
	free_variable_lists(data->exp_list, data->env);
	free(data);
}

void	free_double_array(char **darray)
{
	int	i;

	i = 0;
	while (darray[i])
	{
		free(darray[i]);
		i++;
	}
	free(darray);
}