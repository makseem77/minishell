/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 10:26:56 by ymeziane          #+#    #+#             */
/*   Updated: 2024/02/16 11:41:19 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Checks if the status is a valid number
// and sets the exit_status to the number.
int	is_valid_status(char *status, int *exit_status)
{
	int		i;
	bool	overflow;

	overflow = false;
	i = 0;
	if (!status || !status[0])
		return (1);
	if ((status[0] == '-' || status[0] == '+') && !status[1])
		return (0);
	while (status[i])
	{
		if (!ft_isdigit(status[i]))
			return (0);
		i++;
	}
	*exit_status = ft_atoll(status, &overflow);
	if (overflow)
		return (0);
	return (1);
}

// Exits the program with the given status.
// If the status is not a valid number, it will print the error message.
void	exit_bash(char *status, t_data **data, t_token **token)
{
	int	exit_status;
	int	is_valid;

	ft_putstr_fd("exit\n", 2);
	is_valid = is_valid_status(status, &exit_status);
	if (!status)
	{
		if(token)
			free_token_list(token);
		free_data_struct(*data);
		exit(0);
	}
	if (!is_valid || !status[0])
	{
		print_error("exit", status, "numeric argument required");
		if(token)
			free_token_list(token);
		free_data_struct(*data);
		exit(EXIT_FAILURE);
	}
	if(token)
		free_token_list(token);
	free_data_struct(*data);
	exit(exit_status % 256);
}
