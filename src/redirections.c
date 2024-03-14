/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxborde <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 04:28:22 by maxborde          #+#    #+#             */
/*   Updated: 2024/03/07 05:00:51 by maxborde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	write_to_heredoc(int fd, char *limiter)
{
	char	*line;

	while (true)
	{
		line = readline("> ");
		if (!line)
			exit(1);
		if (ft_strcmp(line, limiter) == 0)
		{
			free(line);
			break ;
		}
		else
		{
			ft_putendl_fd(line, fd);
			free(line);
		}
	}
	close(fd);
	open("tmp", O_RDWR, 0644);
}
