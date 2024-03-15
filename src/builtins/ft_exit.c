/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 10:26:56 by ymeziane          #+#    #+#             */
/*   Updated: 2024/03/15 18:08:11 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool isspaces(char c)
{
	return (c == ' ' || (c >= '\t' && c <= '\r'));
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
	i = 0;
	if (!status || !status[0])
		return (1);
	if (!status[1])
		return (0);
	if(status[0] == '-' || status[0] == '+' || status[0] == '\'' || status[0] == '\"')
		i++;
	if(status[0] == '\'')
		single_quote = true;
	if(status[0] == '\'')
		double_quote = true;
	while(status[i] && isspaces(status[i]))
		i++;
	while (status[i])
	{
		if(isspaces(status[i]))
			break;
		if (!ft_isdigit(status[i]))
			return (0);
		i++;
	}
	while(status[i] && isspaces(status[i]))
		i++;
	if((single_quote && status[i - 1] != '\'') || (double_quote && status[i - 1] != '\"'))
		return (0);
	g_status = ft_atoll(status, &overflow);
	if (overflow)
		return (0);
	return (1);
}

// Exits the program with the given status.
// If the status is not a valid number, it will print the error message.
void	exit_bash(char *status, bool too_many_args, t_data **data, t_token **token, char **args)
{
	int	is_valid;


	if (!status || (*data)->nb_pipe > 0)
	{
		if(token)
			free_token_list(token);
		if (args)
			free_double_array(args);
		free_data_struct(*data);
		g_status = 0;
		exit(EXIT_SUCCESS);
	}
	if(too_many_args)
	{
		print_error("exit", status, "too many arguments");
		if(token)
			free_token_list(token);
		if (args)
			free_double_array(args);
		free_data_struct(*data);
		g_status = 1;
		exit(EXIT_FAILURE);
	}
	is_valid = is_valid_status(status);
	if (!is_valid || !status[0])
	{
		print_error("exit", status, "numeric argument required");
		if(token)
			free_token_list(token);
		if (args)
			free_double_array(args);
		free_data_struct(*data);
		g_status = 2;
		exit(2);
	}
	if(token)
		free_token_list(token);
	if (args)
		free_double_array(args);
	free_data_struct(*data);
	free(status);
	g_status = g_status % 256;
	exit(g_status);
}
