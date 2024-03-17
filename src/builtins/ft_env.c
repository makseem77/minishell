/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 20:24:16 by ymeziane          #+#    #+#             */
/*   Updated: 2024/03/15 18:04:00 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//Prints the env list.
void	env(char **args, t_env_list **env)
{
	t_env_list	*tmp;

	tmp = *env;
	if (*(args + 1) != NULL)
	{
		print_error("env", *(args + 1), "Too many arguments");
		state = 1;
		return ;
	}
	while (tmp)
	{
		ft_putstr_fd(tmp->variable, 1);
		ft_putstr_fd("\n", 1);
		tmp = tmp->next;
	}
	state = 0;
}
