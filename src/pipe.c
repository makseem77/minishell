/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 16:20:27 by ymeziane          #+#    #+#             */
/*   Updated: 2024/02/17 20:31:33 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int init_pipe(int nb_pipe, int **pipefd)
{
    int nb_pipefd;
    int i;

    nb_pipefd = nb_pipe * 2;
    *pipefd = malloc(sizeof(int) * nb_pipefd);
    if (!*pipefd)
        return (ft_putstr_fd("Error:\nAllocation failed\n", 2), 0);
    i = 0;
    while (i < nb_pipe)
    {
       if (pipe(*pipefd + i * 2) == -1)
		{
			ft_putstr_fd("Error:\nPipe creation failed\n", 2);
			while (i > 0)
			{
				i--;
				close(*pipefd[i * 2]);
				close(*pipefd[i * 2 + 1]);
			}
			free(pipefd);
			return (0);
		}
		i++;
    }
    return (1);
}