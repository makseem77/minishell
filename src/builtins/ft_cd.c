/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/05 13:03:07 by ymeziane          #+#    #+#             */
/*   Updated: 2024/02/07 17:42:38 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void ft_cd(char *absolute_path)
{
    if(!absolute_path)
        return ;
    if(chdir(absolute_path) != 0)
    {
        ft_putstr_fd("cd: ", 2);
        write(2, absolute_path, ft_strlen(absolute_path));
        ft_putstr_fd(": ", 2);
        write(2, strerror(errno), ft_strlen(strerror(errno)));
        write(2, "\n", 1);
    }
}