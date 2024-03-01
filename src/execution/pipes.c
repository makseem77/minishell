/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 16:05:17 by ymeziane          #+#    #+#             */
/*   Updated: 2024/03/01 22:39:55 by maxborde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	**init_pipes(t_data **data)
{
	int **fds;
	int i;

	if ((*data)->nb_pipe)
		fds = malloc(sizeof(int *) * (*data)->nb_pipe);
	else
		return (NULL);
	i = 0;
	while (i < (*data)->nb_pipe)
	{
		fds[i] = malloc(sizeof(int) * 2);
		pipe(fds[i]);
		i++;
	}
	return (fds);
}

void	close_all_pipes(int **fds, int nb_pipe)
{
	int	i = 0;

	while(i < nb_pipe)
	{
		close(fds[i][0]);
		close(fds[i][1]);
		i++;
	}
}

void	free_fds_array(int **fds, int nb_pipe)
{
	int	i;

	i = 0;
	while (i < nb_pipe)
	{
		free(fds[i]);
		i++;
	}	
	free(fds);
}
