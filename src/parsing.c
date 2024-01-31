/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 15:23:35 by ymeziane          #+#    #+#             */
/*   Updated: 2024/01/31 20:15:53 by maxborde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	is_a_built_in(char *element)
{	
	if (ft_strcmp("cd", element) || ft_strcmp("ls", element) || 
			ft_strcmp("pwd", element) || ft_strcmp("exit", element) 
			|| ft_strcmp("export", element) || ft_strcmp("unset", element)
			|| ft_strcmp("env", element))
		return (1);
	return (0);
}

int	is_a_meta_char(char *element)
{
	if (ft_strcmp(">", element) || ft_strcmp(">>", element) || 
			ft_strcmp("<", element) || ft_strcmp("<<", element) 
			|| ft_strcmp("|", element) || ft_strcmp("$", element))
		return (1);
	return (0);

}
//Goes trough the token linked list and gives a tokentype to every node of the list.
void	set_token_types(t_token **tokenlist)
{	    
	t_token	*tmp;
	int	pos;

	tmp = *tokenlist;
	pos = 0;
	while (tmp)
	{
	}
}
