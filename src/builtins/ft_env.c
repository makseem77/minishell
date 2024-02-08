/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 20:24:16 by ymeziane          #+#    #+#             */
/*   Updated: 2024/02/08 20:32:48 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void   env(t_env_list **env)
{
    t_env_list *tmp;

    tmp = *env;
    while (tmp)
    {
        ft_putstr_fd(tmp->variable, 1);
        write(1, "\n", 1);
        tmp = tmp->next;
    }
}