/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 13:53:56 by maxborde          #+#    #+#             */
/*   Updated: 2024/02/13 10:35:41 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

// Returns the last element of the list
t_env_list	*lst_last(t_env_list *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
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

// Deletes an element from the list
void	lst_del_one(t_env_list **lst, char *variable, int offset)
{
	t_env_list	*tmp;
	t_env_list	*prev;
	char		*var_name;
	int			bytestocmp;

	if (!lst || !*lst)
		return ;
	var_name = extract_var_name(variable);
	tmp = *lst;
	bytestocmp = compute_bytes_to_cmp(tmp->variable + offset, var_name);
	if (ft_strncmp(tmp->variable + offset, variable, bytestocmp) == 0)
	{
		*lst = tmp->next;
		free(tmp->variable);
		free(tmp);
		return ;
	}
	while (tmp)
	{
		bytestocmp = compute_bytes_to_cmp(tmp->variable + offset, var_name);
		if (ft_strncmp(tmp->variable + offset, variable, bytestocmp) == 0)
			break ;
		prev = tmp;
		tmp = tmp->next;
	}
	if (!tmp)
		return ;
	prev->next = tmp->next;
	free(tmp->variable);
	free(tmp);
}

// Returns list size
int	lst_size(t_env_list **lst)
{
	t_env_list	*tmp;
	int			i;

	i = 0;
	tmp = *lst;
	while (tmp)
	{
		tmp = tmp->next;
		i++;
	}
	return (i);
}
