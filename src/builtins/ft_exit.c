/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 10:26:56 by ymeziane          #+#    #+#             */
/*   Updated: 2024/03/26 15:51:29 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	isspaces(char c)
{
	return (c == ' ' || (c >= '\t' && c <= '\r'));
}

void	handle_first_char_status(char *status, int *i, bool *single_quote,
		bool *double_quote)
{
	*i = 0;
	*single_quote = false;
	*double_quote = false;
	if (status[0] == '-' || status[0] == '+' || status[0] == '\''
		|| status[0] == '\"')
		*i += 1;
	if (status[0] == '\'')
		*single_quote = true;
	if (status[0] == '\'')
		*double_quote = true;
}

// Checks if the status is a valid number
// and sets the exit_status to the number.
int	is_valid_status(char *status)
{
	int		i;
	bool	overflow;
	bool	single_quote;
	bool	double_quote;

	overflow = false;
	handle_first_char_status(status, &i, &single_quote, &double_quote);
	while (status[i] && isspaces(status[i]))
		i++;
	while (status[i])
	{
		if (isspaces(status[i]))
			break ;
		if (!ft_isdigit(status[i]))
			return (0);
		i++;
	}
	while (status[i] && isspaces(status[i]))
		i++;
	if ((single_quote && status[i - 1] != '\'') || (double_quote && status[i
				- 1] != '\"'))
		return (0);
	g_status = ft_atoll(status, &overflow);
	return (!overflow);
}

void	free_and_exit(t_data **data, t_token **token, char **args, int status)
{
	g_status = status;
	if (token)
		free_token_list(token);
	if (args)
		free_double_array(args);
	if((*data)->nb_pipe > 0)
		free_fds_array((*data)->pipe_fds, (*data)->nb_pipe);
	free_data_struct(*data);
	g_status = g_status % 256;
	exit(g_status);
}

// Exits the program with the given status.
// If the status is not a valid number, it will print the error message.
void	exit_bash(char *status, t_data **data, t_token **token, char **args)
{
	int		is_valid;
	bool	too_many_args;

	if (!status || (*data)->nb_pipe > 0)
	{
		if (!status && (*data)->nb_pipe == 0)
			ft_putstr_fd("exit\n", 1);
		free_and_exit(data, token, args, EXIT_SUCCESS);
	}
	too_many_args = (args && args[2]);
	if (too_many_args)
	{
		ft_putstr_fd("exit\n", 1);
		print_error("exit", NULL, "too many arguments");
		return ;
	}
	is_valid = is_valid_status(status);
	if (!is_valid)
	{
		ft_putstr_fd("exit\n", 1);
		print_error("exit", status, "numeric argument required");
		free(status);
		free_and_exit(data, token, args, 2);
	}
	ft_putstr_fd("exit\n", 1);
	free_and_exit(data, token, args, g_status);
}
