/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 11:32:51 by maxborde          #+#    #+#             */
/*   Updated: 2024/02/12 15:55:38 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int compute_bytes_to_cmp(char *variableinlist, char *variablename)
{
    char    *varnameinlist;
    int     bytestocmp;

    varnameinlist = extract_var_name(variableinlist);
    if (ft_strlen(varnameinlist) > ft_strlen(variablename))
    {
        bytestocmp = ft_strlen(varnameinlist);
        free(varnameinlist);
        return (bytestocmp);
    }
    else
    {
        bytestocmp = ft_strlen(variablename);
        free(varnameinlist);
        return (bytestocmp);
    }
}

static  bool arg_in_list(char *arg, t_env_list **lst, int offset)
{
    t_env_list *tmp;
	char	*var_name;
	int	bytestocmp;

    tmp = *lst;
    var_name = extract_var_name(arg);
    while (tmp)
    {
        bytestocmp = compute_bytes_to_cmp(tmp->variable + offset, var_name);	
        if (ft_strncmp(tmp->variable + offset, var_name, bytestocmp) == 0)
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
        if(arg_in_list(*args, env, 0))
            lst_del_one(env, *args, 0);
        if(arg_in_list(*args, exp_list, ft_strlen("declare -x ")))
            lst_del_one(exp_list, *args, ft_strlen("declare -x "));
        args++;
    }
}
