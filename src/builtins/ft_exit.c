/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 10:26:56 by ymeziane          #+#    #+#             */
/*   Updated: 2024/02/13 11:22:42 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//exit_bash will exit the shell.
//If no argument is given, it will exit with the status 0.
//If the argument is not a number, or has a sign, or is bigger than long long,
//it will print an error message and return.
//If the argument is a number, it will exit with the status % 256,
//because the status is an 8-bit number.
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
		return (print_error("exit", status, "numeric argument required"));
	while (status[i])
	{
		if (!ft_isdigit(status[i]))
			return (print_error("exit", status, "numeric argument required"));
		i++;
	}
	status_int = ft_atoll(status, &overflow);
	if (overflow)
		return (print_error("exit", status, "numeric argument required"));
	ft_putstr_fd("exit\n", 2);
	exit(status_int % 256);
}
