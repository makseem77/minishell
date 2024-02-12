/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 13:53:56 by maxborde          #+#    #+#             */
/*   Updated: 2024/02/12 11:05:36 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env_list	*lst_new(char	*variable)
{
	t_env_list	*new;

	new = (t_env_list *)malloc(sizeof(t_env_list));
	if (!new)
		return (NULL);
	new->variable = variable;
	new->next = NULL;
	return (new);
}

t_env_list	*lst_last(t_env_list *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

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

void	lst_del_one(t_env_list **lst, char *variable, bool export)
{
	t_env_list	*tmp;
	t_env_list	*prev;

	if (!lst || !*lst)
		return ;
	tmp = *lst;
	if(!export)
	{
		if (ft_strcmp(tmp->variable, variable) == 0)
		{
			*lst = tmp->next;
			free(tmp->variable);
			free(tmp);
			return ;
		}
		while (tmp && ft_strncmp(tmp->variable, variable, ft_strlen(variable)) != 0)
		{
			prev = tmp;
			tmp = tmp->next;
		}
	}
	else
	{
		if (ft_strcmp(tmp->variable + ft_strlen("declare -x "), variable) == 0)
		{
			*lst = tmp->next;
			free(tmp->variable);
			free(tmp);
			return ;
		}
		while (tmp && ft_strncmp(tmp->variable + ft_strlen("declare -x "), variable, ft_strlen(variable)) != 0)
		{
			prev = tmp;
			tmp = tmp->next;
		}
	}
	if (!tmp)
		return ;
	prev->next = tmp->next;
	free(tmp->variable);
	free(tmp);
}

int	lst_size(t_env_list **lst)
{
	t_env_list	*tmp;
	int	i;

	i = 0;
	tmp = *lst;	
	while (tmp)
	{
		tmp = tmp->next;
		i++;
	}
	return (i);
}
