/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 17:56:02 by ymeziane          #+#    #+#             */
/*   Updated: 2024/02/12 17:59:16 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include    "minishell.h"

//Prints the error message in case of a unvalid identifier just like bash.
void	ft_put_unvalidvar_error(char *arg)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}

void	print_exit_error(char *status)
{
	ft_putstr_fd("exit\n", 2);
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(status, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
	exit(255);
}

void	print_export(t_env_list **export_variables)
{
	t_env_list	*tmp;

	tmp = *export_variables;
	if (!tmp)
		return ;
	while (tmp)
	{
		ft_putstr_fd(tmp->variable, 1);
		write(1, "\n", 1);
		tmp = tmp->next;
	}
}

