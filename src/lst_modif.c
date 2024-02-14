/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_modif.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 16:30:44 by ymeziane          #+#    #+#             */
/*   Updated: 2024/02/14 17:14:58 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Deletes an element from the list
int	lst_del_one(t_env_list **lst, char *variable, int offset)
{
	t_env_list	*tmp;
	t_env_list	*prev;
	char		*var_name;
	int			bytestocmp;

	if (!lst || !*lst)
		return (1);
	var_name = extract_var_name(variable);
	tmp = *lst;
	bytestocmp = compute_bytes_to_cmp(tmp->variable + offset, var_name);
	if (ft_strncmp(tmp->variable + offset, variable, bytestocmp) == 0)
	{
		*lst = tmp->next;
		return (free_lst_content(tmp, var_name), 0);
	}
	while (tmp)
	{
		bytestocmp = compute_bytes_to_cmp(tmp->variable + offset, var_name);
		if (ft_strncmp(tmp->variable + offset, variable, bytestocmp) == 0)
			break ;
		prev = tmp;
		tmp = tmp->next;
	}
	prev->next = tmp->next;
	return (free_lst_content(tmp, var_name), 0);
}

// Adds a new element to the end of the list
void	lst_add_back(t_env_list **lst, t_env_list *new)
{
	t_env_list	*last_el;

	if (!lst || !new)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	last_el = lst_last(*lst);
	last_el->next = new;
}

// Creates a new node and returns it
t_env_list	*lst_new(char *variable)
{
	t_env_list	*new;

	new = (t_env_list *)malloc(sizeof(t_env_list));
	if (!new)
		return (NULL);
	new->variable = variable;
	new->next = NULL;
	return (new);
}
