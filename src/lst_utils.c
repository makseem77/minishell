/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 13:53:56 by maxborde          #+#    #+#             */
/*   Updated: 2024/02/14 16:31:23 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Returns the last element of the list
t_env_list	*lst_last(t_env_list *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
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
