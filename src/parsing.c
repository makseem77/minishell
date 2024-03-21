/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 15:23:35 by ymeziane          #+#    #+#             */
/*   Updated: 2024/03/20 17:08:10 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	is_executable(char *executable, char **bin_paths, bool should_free)
{
	int	fd;

	fd = open(executable, O_DIRECTORY);
	if (fd == -1)
	{
		if (access(executable, X_OK) == 0)
		{
			if (should_free)
			{
				free_double_array(bin_paths);
				free(executable);
			}
			return (1);
		}
	}
	else
		close(fd);
	return (0);
}

static int	is_a_command(char *element, t_env_list **env)
{
	char	**bin_paths;
	char	*executable;
	char	*tmp;
	int		i;

	if (is_executable(element, NULL, false))
		return (1);
	bin_paths = find_bin_paths(env);
	if (!bin_paths)
		return (0);
	i = 0;
	while (bin_paths[i])
	{
		executable = ft_strjoin(bin_paths[i], "/");
		tmp = executable;
		executable = ft_strjoin(executable, element);
		free(tmp);
		if (is_executable(executable, bin_paths, true))
			return (1);
		free(executable);
		i++;
	}
	free_double_array(bin_paths);
	return (0);
}

int	type(char *element, t_env_list **env)
{
	if (!element)
		return (-1);
	if(!ft_strcmp("", element))
		return (EMPTY);
	if (!ft_strcmp("echo", element) || !ft_strcmp("cd", element)
		|| !ft_strcmp("pwd", element) || !ft_strcmp("export", element)
		|| !ft_strcmp("unset", element) || !ft_strcmp("env", element)
		|| !ft_strcmp("exit", element))
		return (BUILTIN);
	if (!ft_strcmp(">", element) || !ft_strcmp(">>", element) || !ft_strcmp("<",
			element) || !ft_strcmp("<<", element) || !ft_strcmp("&", element)
		|| !ft_strcmp("|", element) || !ft_strcmp("&&", element)
		|| !ft_strcmp("||", element) || !ft_strcmp(",", element)
		|| !ft_strcmp("(", element) || !ft_strcmp(")", element))
		return (META_CHAR);
	if (is_a_command(element, env))
		return (COMMAND);
	return (-1);
}

void	clean_up_redirection(t_token **tokenlist)
{
	t_token	*tmp;
	t_token	*prev;

	tmp = *tokenlist;
	prev = NULL;
	while (tmp)
	{
		if (tmp->ttype == REDIRECTION || tmp->ttype == REDIRECTION_FILE
			|| tmp->ttype == HERE_DOC || tmp->ttype == DELIMITER || tmp->ttype == EMPTY)
		{
			if (prev)
				prev->next = tmp->next;
			else
				*tokenlist = tmp->next;
			free_node(tmp);
			if(prev)
				tmp = prev->next;
			else	
				tmp = *tokenlist;
		}
		else
		{
			prev = tmp;
			tmp = tmp->next;
		}
	}
}


int error_syntax(t_token *tmp, int *nb_pipe)
{
	if (ft_strcmp(tmp->element, "|") == 0)
	{
		if (!tmp->next || ft_strcmp(tmp->next->element, "|") == 0)
			return(print_error(NULL, NULL, "pipe should be followed by a command"), 1);
		else
			(*nb_pipe)++;
	}
	else if (ft_strcmp(tmp->element, ";") == 0)
		return(print_error(NULL, NULL, "special character ';'' is not authorized"), 1);
	else if((tmp->ttype == REDIRECTION || tmp->ttype == HERE_DOC) && !tmp->next)
		return(ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2), 1);
	return (0);
}

t_token	*set_heredoc_type(t_token *tmp, bool *heredoc)
{
	*heredoc = true;
	tmp->ttype = HERE_DOC;
	if(tmp->next)
	{
		tmp = tmp->next;
		if (ft_strcmp(tmp->element, ">") != 0)
			tmp->ttype = DELIMITER;
		else
			return (NULL);
	}
	return (tmp);
}

t_token	*set_redirections_type(t_token *tmp)
{
	tmp->ttype = REDIRECTION;
	if(tmp->next)
	{
		tmp = tmp->next;
		if (ft_strncmp(tmp->element, ">", 1)  || ft_strncmp(tmp->element, "<", 1) != 0)
			tmp->ttype = REDIRECTION_FILE;
		else
			return (NULL);
	}
	return (tmp);
}

void types_assignement(t_token **tokenlist, t_env_list **env, bool *heredoc)
{
	t_token	*tmp;
	
	tmp = *tokenlist;
	while(tmp)
	{
		if(ft_strncmp(tmp->element, "<<", 2) == 0)
			tmp = set_heredoc_type(tmp, heredoc);
		else if (ft_strncmp(tmp->element, ">>", 2) == 0 || ft_strcmp(tmp->element,
					">") == 0 || ft_strcmp(tmp->element, "<") == 0)
			tmp = set_redirections_type(tmp);
		else if (type(tmp->element, env) == BUILTIN)
			tmp->ttype = BUILTIN;
		else if (type(tmp->element, env) == META_CHAR)
			tmp->ttype = META_CHAR;
		else if (type(tmp->element, env) == COMMAND)
			tmp->ttype = COMMAND;
		else if(type(tmp->element, env) == EMPTY)
			tmp->ttype = EMPTY;
		tmp = tmp->next;
	}
}

t_token	*get_cmd_token(t_token **tokenlist, int expr_index)
{
	t_token	*tmp;
	int	i;

	tmp = *tokenlist;
	i = 0;
	while (tmp)
	{
		if ((tmp->ttype == COMMAND || tmp->ttype == BUILTIN) && i == expr_index)
			return (tmp);
		if (ft_strcmp(tmp->element, "|") == 0)
			i++;
		tmp = tmp->next;
	}
	return (NULL);
}

int	create_or_append(t_token *tmp, t_token *command_token)
{
	int	fd;
	fd = -1;
	if (command_token)	
	{
		if (command_token->fd_out > 1)
			close(command_token->fd_out);
		command_token->fd_out = open(tmp->next->element, O_CREAT | O_RDWR | O_APPEND, 0644);
	}
	else
		fd = open(tmp->next->element, O_CREAT | O_RDWR | O_APPEND, 0644);
	if (fd != -1)
		close(fd);
	if (command_token)
	{
		if (command_token->fd_out == -1)
			print_error(NULL, tmp->next->element, strerror(errno));
	}
	return (-1);
}

int	create_or_truncate(t_token *tmp, t_token *command_token)
{
	int	fd;
	fd = -1;
	if (command_token)	
	{
		if (command_token->fd_out > 1)
			close(command_token->fd_out);
		command_token->fd_out = open(tmp->next->element, O_CREAT | O_RDWR | O_TRUNC, 0644);
	}
	else
		fd = open(tmp->next->element, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd != -1)
		close(fd);
	if (command_token)
	{
		if (command_token->fd_out == -1)
			print_error(NULL, tmp->next->element, strerror(errno));
	}
	return (-1);
}

void	create_and_read_from_heredoc(t_token *tmp, t_token *command_token, t_data **data, t_token **tokenlist)
{
	int	fd;

	fd = 0;
	if(!command_token)
		fd = write_to_heredoc(open(".tmp", O_CREAT | O_RDWR | O_TRUNC, 0644), tmp->next->element, false, data, tokenlist);
	else
	{
		if (command_token->fd_in > 1)
			close(command_token->fd_in);
		command_token->fd_in = open(".tmp", O_CREAT | O_RDWR | O_TRUNC, 0644);
		fd = write_to_heredoc(command_token->fd_in, tmp->next->element, true, data, tokenlist);
	}
	if((command_token && command_token->fd_in == -1) || fd == -1)
	{
		if (command_token && command_token->fd_in == -1)
		{
			command_token->fd_out = open("/dev/null", O_WRONLY);
		}
		print_error(NULL, tmp->next->element, strerror(errno));
	}
}

void	read_from_file(t_token *tmp, t_token *command_token)
{
	int	fd;

	fd = 0;
	if (command_token)
	{
		if (command_token->fd_in > 1)
			close(command_token->fd_in);
		command_token->fd_in = open(tmp->next->element,  O_RDWR, 0644);
	}
	else
		fd = open(tmp->next->element, O_RDWR, 0644);
	if((command_token && command_token->fd_in == -1) || fd == -1)
	{
		if (command_token && command_token->fd_in == -1)
			command_token->fd_out = open("/dev/null", O_WRONLY);
		print_error(NULL, tmp->next->element, strerror(errno));
	}
}

int	create_and_set_fd(t_token *tmp, t_token *command_token, t_data **data, t_token **tokenlist)
{
	if(ft_strncmp(tmp->element, ">>", 2) == 0 || ft_strcmp(tmp->element, ">") == 0)
	{
		if(ft_strncmp(tmp->element, ">>", 2) == 0)
			create_or_append(tmp, command_token);
		else
			create_or_truncate(tmp, command_token);
	}
	else if(ft_strncmp(tmp->element, "<<", 2) == 0 || ft_strcmp(tmp->element, "<") == 0)
	{
		if(ft_strncmp(tmp->element, "<<", 2) == 0)
			create_and_read_from_heredoc(tmp, command_token, data, tokenlist);
		else
			read_from_file(tmp, command_token);
	}
	return (-1);
}

int handle_redirections(t_token **tokenlist, int *nb_pipe, t_data **data)
{
	t_token	*tmp;
	t_token	*command_token;
	int	expr_index;

	tmp = *tokenlist;
	expr_index = 0;
	while(tmp)
	{
		if(error_syntax(tmp, nb_pipe))
			return (1);
		command_token = get_cmd_token(tokenlist, expr_index);
		if (ft_strcmp(tmp->element, "|") == 0)
			expr_index++;
		create_and_set_fd(tmp, command_token, data, tokenlist);
		tmp = tmp->next;
	}
	return (0);
}

// void	print_token_list(t_token **tokenlist)
// {
// 	t_token	*tmp;
//  	tmp = *tokenlist;
//
//  	while (tmp)
//  	{
// 		printf("Element = %s\n", tmp->element);
//  		printf("Type = %d\n", tmp->ttype);
//  		tmp = tmp->next;
//  	}
//  }

// Goes trough the token linked list
// and gives a tokentype to every node of the list.
int	set_token_types(t_token **tokenlist, t_data **data)
{
	types_assignement(tokenlist, (*data)->env, &(*data)->here_doc);
	if(handle_redirections(tokenlist, &(*data)->nb_pipe, data) == 1)
		return (1);
	// print_token_list(tokenlist);
	clean_up_redirection(tokenlist);
	return (0);
}
