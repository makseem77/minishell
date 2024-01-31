/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 15:23:35 by ymeziane          #+#    #+#             */
/*   Updated: 2024/01/31 18:05:28 by maxborde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	is_a_built_in(char *element)
{	
	if (ft_strcmp("cd", element) || ft_strcmp("ls", element) || 
			ft_strcmp("pwd", element) || ft_strcmp("exit", element) 
			|| ft_strcmp("export", element) || ft_strcmp("unset")
			|| ft_strcmp("env", element));
		return (1);
	return (0);
}

int	is_a_meta_char(char *element)
{
	if (ft_strcmp(">", element) || ft_strcmp(">>", element) || 
			ft_strcmp("<", element) || ft_strcmp("<<", element) 
			|| ft_strcmp("|", element) || ft_strcmp("$"));
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
		if (pos == 0)
		{
			if (is_a_built_in(tmp->element))
				tmp->ttype = BUILTIN;
			else if (is_a_command(tmp->element))
				tmp->ttype = COMMAND;
			else if (is_a_variable(tmp->element))
				tmp->ttype = VARIABLE;
		}
		else
			tmp->ttype = COMMAND_ARGS;
		tmp = tmp->next;
		pos++;
	}
}
