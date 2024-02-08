/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 20:29:50 by ymeziane          #+#    #+#             */
/*   Updated: 2024/02/08 20:52:47 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void print_escaped_chars(char *str)
{
    while (*str)
    {
        if (*str == '\\' && *(str + 1))
        {
            str++;
            if (*str == 'n')
                write(1, "\n", 1);
            else if (*str == 't')
                write(1, "\t", 1);
        }
        else
            write(1, str, 1);
        str++;
    }
}

void    echo(t_token **tokenlist)
{
    t_token *tmp;
    int     i;
    bool     interpret_escape_sequences = false;

    tmp = *tokenlist;
    i = 0;
    if (tmp && ft_strcmp(tmp->element, "-e") == 0)
    {
        interpret_escape_sequences = true;
        tmp = tmp->next;
    }
    while (tmp)
    {
        if (i > 0)
            write(1, " ", 1);
        if (interpret_escape_sequences)
            print_escaped_chars(tmp->element);
        else
            write(1, tmp->element, ft_strlen(tmp->element));
        tmp = tmp->next;
        i++;
    }
    write(1, "\n", 1);
}
