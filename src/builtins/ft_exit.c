/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 10:26:56 by ymeziane          #+#    #+#             */
/*   Updated: 2024/02/12 17:57:20 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_bash(char *status)
{
	int		status_int;
	int		i;
	int		is_sign_present;
	bool	overflow;

	overflow = false;
	i = 0;
	is_sign_present = 0;
	if (status == NULL || status[0] == '\0')
	{
		ft_putstr_fd("exit\n", 2);
		exit(0);
	}
	if (status[i] == '-' || status[i] == '+')
	{
		is_sign_present = 1;
		i++;
	}
	if (is_sign_present && !status[i])
		return (print_exit_error(status));
	while (status[i])
	{
		if (!ft_isdigit(status[i]))
			return (print_exit_error(status));
		i++;
	}
	status_int = ft_atoll(status, &overflow);
	if (overflow)
		return (print_exit_error(status));
	ft_putstr_fd("exit\n", 2);
	exit(status_int % 256);
}
