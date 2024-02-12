/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 11:32:51 by maxborde          #+#    #+#             */
/*   Updated: 2024/02/12 10:53:22 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool arg_in_env(char *arg, t_env_list **env, bool export)
{
    t_env_list *tmp;

    tmp = *env;
    while (tmp)
    {
        if(export)
        {
            if (ft_strncmp(tmp->variable + ft_strlen("declare -x "), arg, ft_strlen(arg)) == 0)
                return (true);
        }
        else if(ft_strncmp(tmp->variable, arg, ft_strlen(arg)) == 0)
            return (true);
        tmp = tmp->next;
    }
    return (false);
}

bool    is_valid_variable(char *variable)
{
    int i;

    i = 0;
    if (!ft_isalpha(variable[0]) || variable[0] == '_')
        return (false);
    while (variable[i])
    {
        if (!ft_isalnum(variable[i]) && variable[i] != '_')
            return (false);
        i++;
    }
    return (true);
}

void	unset(char **args, t_env_list **env, t_env_list **exp_list)
{
    while(*args)
    {
        if(!(is_valid_variable(*args)))
        {
            ft_putstr_fd("minishell: unset: `", 2);
            ft_putstr_fd(*args, 2);
            ft_putstr_fd("': not a valid identifier\n", 2);
        }
        if(arg_in_env(*args, env, false))
            lst_del_one(env, *args, false);
        if(arg_in_env(*args, exp_list, true))
            lst_del_one(exp_list, *args, true);
        args++;
    }
}
