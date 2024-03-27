/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 16:05:17 by ymeziane          #+#    #+#             */
/*   Updated: 2024/03/27 10:22:38 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	**init_pipes(t_data **data)
{
	int	**fds;
	int	i;

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

void	close_all_pipes(t_token **tokenlist, int **fds, int nb_pipe)
{
	t_token	*tmp;
	int		i;

	i = 0;
	tmp = *tokenlist;
	while (tmp)
	{
		if (tmp->fd_out > 1)
			close(tmp->fd_out);
		if (tmp->fd_in > 1)
			close(tmp->fd_in);
		tmp = tmp->next;
	}
	while (i < nb_pipe)
	{
		close(fds[i][0]);
		close(fds[i][1]);
		i++;
	}
}
