/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_array.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/18 15:54:37 by ymeziane          #+#    #+#             */
/*   Updated: 2024/03/18 16:01:11 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_double_array(char **darray)
{
	int	i;

	i = 0;
	if (darray)
	{
		while (darray[i])
		{
			free(darray[i]);
			i++;
		}
		free(darray);
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
