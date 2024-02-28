/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 16:05:17 by ymeziane          #+#    #+#             */
/*   Updated: 2024/02/28 17:22:18 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	**init_pipes(t_data **data)
{
	int **fds;
	int i;

	fds = malloc(sizeof(int *) * (*data)->nb_pipe);
	i = 0;
	printf("pipe = %d\n", (*data)->nb_pipe);
	while (i < (*data)->nb_pipe)
	{
		fds[i] = malloc(sizeof(int) * 2);
		pipe(fds[i]);
		i++;
	}
	return (fds);
}