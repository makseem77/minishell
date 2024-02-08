/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 11:32:51 by maxborde          #+#    #+#             */
/*   Updated: 2024/02/08 14:39:24 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool arg_in_env(char *arg, t_env_list **env)
{
    t_env_list *tmp;

    tmp = *env;
    while (tmp)
    {
        if (ft_strncmp(tmp->variable, arg, ft_strlen(arg)) == 0)
            return (true);
        tmp = tmp->next;
    }
    return (false);
}

void	unset(char **args, t_env_list **env)
{
    while(*args)
    {
        if(arg_in_env(*args, env))
            lst_del_one(env, *args);
        args++;
    }
}
