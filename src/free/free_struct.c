/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_struct.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 15:56:41 by ymeziane          #+#    #+#             */
/*   Updated: 2024/03/18 16:01:49 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_variable_lists(t_env_list **export_list, t_env_list **env_list)
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

void	free_node(t_token *token)
{
	free(token->element);
	free(token);
}

void	free_lst_content(t_env_list *lst, char *var_name)
{
	free(lst->variable);
	free(lst);
	free(var_name);
}

void	free_data_struct(t_data *data)
{
	free(data->old_pwd);
	free(data->home_dir);
	free_double_array(data->bin_paths);
	free_variable_lists(data->exp_list, data->env);
	free(data->path_cmd);
	free(data);
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
