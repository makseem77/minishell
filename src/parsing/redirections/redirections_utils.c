#include "minishell.h"

int	create_or_append_helper(t_token *tmp, t_token *command_token, t_data **data)
{
	if (command_token->fd_out > 1)
		close(command_token->fd_out);
	command_token->fd_out = open(tmp->next->element,
			O_CREAT | O_RDWR | O_APPEND, 0644);
	if ((*data)->invalid_file)
	{
		if (command_token->fd_out > 1)
			close(command_token->fd_out);
		return (command_token->fd_out = -1, 1);
	}
	return (0);
}

int	create_or_truncate_helper(t_token *tmp, t_token *command_token, t_data **data)
{
	if (command_token->fd_out > 1)
		close(command_token->fd_out);
	command_token->fd_out = open(tmp->next->element,
			O_CREAT | O_RDWR | O_TRUNC, 0644);
	if ((*data)->invalid_file)
	{
		if (command_token->fd_out > 1)
			close(command_token->fd_out);
		return (command_token->fd_out = -1, 1);
	}
	return (0);
}

void	read_from_file_helper(t_token *tmp, t_token *command_token)
{
	if ((command_token && command_token->fd_in == -1))
	{
		if (command_token && command_token->fd_in == -1
			&& command_token->fd_out != -1)
		{
			print_error(NULL, tmp->next->element, strerror(errno));
			if (command_token->fd_out > 1)
				close(command_token->fd_out);
			command_token->fd_out = -1;
		}
	}
}
