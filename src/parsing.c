/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 15:23:35 by ymeziane          #+#    #+#             */
/*   Updated: 2024/03/17 18:59:54 by ymeziane         ###   ########.fr       */
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
			|| tmp->ttype == HERE_DOC || tmp->ttype == DELIMITER)
		{
			if (prev)
				prev->next = tmp->next;
			else
				*tokenlist = tmp->next;
			free(tmp->element);
			free(tmp);
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
		if (!tmp->next)
			return(print_error(NULL, NULL, "pipe should be followed by a command"), 1);
		else
			(*nb_pipe)++;
	}
	else if (ft_strcmp(tmp->element, ";") == 0 || ft_strcmp(tmp->element, "\\") == 0)
		return(print_error(NULL, NULL, "special characters ';'' or '\\' are not authorized"), 1);
	else if((tmp->ttype == REDIRECTION || tmp->ttype == HERE_DOC) && !tmp->next)
		return(ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2), 1);
	return (0);
}

void types_assignement(t_token **tokenlist, t_env_list **env, bool *heredoc)
{
	t_token	*tmp;
	
	tmp = *tokenlist;
	while(tmp)
	{
		if(ft_strncmp(tmp->element, "<<", 2) == 0)
		{
			*heredoc = true;
			tmp->ttype = HERE_DOC;
			if(tmp->next)
				tmp->next->ttype = DELIMITER;
		}
		else if (ft_strncmp(tmp->element, ">>", 2) == 0 || ft_strcmp(tmp->element,
					">") == 0 || ft_strcmp(tmp->element, "<") == 0)
		{
			tmp->ttype = REDIRECTION;
			if(tmp->next)
				tmp->next->ttype = REDIRECTION_FILE;
		}
		else if (type(tmp->element, env) == BUILTIN)
			tmp->ttype = BUILTIN;
		else if (type(tmp->element, env) == META_CHAR)
			tmp->ttype = META_CHAR;
		else if (type(tmp->element, env) == COMMAND)
			tmp->ttype = COMMAND;
		tmp = tmp->next;
	}
}

int handle_redirections(t_token **tokenlist, int *nb_pipe)
{
	t_token	*tmp;
	t_token *last_cmd;
	t_token	*token_fd;

	tmp = *tokenlist;
	while(tmp)
	{
		if(error_syntax(tmp, nb_pipe))
			return (1);
		if(tmp->ttype == BUILTIN || tmp->ttype == COMMAND)
			last_cmd = tmp;
		if(last_cmd)
			token_fd = last_cmd;
		else if(tmp->next->next && (tmp->next->next->ttype == COMMAND || tmp->next->next->ttype == BUILTIN))
			token_fd = tmp->next->next;
		else
			token_fd = NULL;
		if(ft_strncmp(tmp->element, ">>", 2) == 0 || ft_strcmp(tmp->element, ">") == 0)
		{
			if(ft_strncmp(tmp->element, ">>", 2) == 0)
					token_fd->fd_out = open(tmp->next->element, O_CREAT | O_RDWR | O_APPEND, 0644);
			else
				token_fd->fd_out = open(tmp->next->element, O_CREAT | O_RDWR | O_TRUNC, 0644);
			if (token_fd->fd_out == -1)
				return(print_error(NULL, tmp->next->element, strerror(errno)), 1);
		}
		else if(ft_strncmp(tmp->element, "<<", 2) == 0 || ft_strcmp(tmp->element, "<") == 0)
		{
			if(ft_strncmp(tmp->element, "<<", 2) == 0)
			{
				if(token_fd)
					token_fd->fd_in = open("tmp", O_CREAT | O_RDWR | O_TRUNC, 0644);
				write_to_heredoc(token_fd->fd_in, tmp->next->element);
			}
			else
				token_fd->fd_in = open(tmp->next->element,  O_RDWR, 0644);
			if(token_fd->fd_in == -1)
				return(print_error(NULL, tmp->next->element, strerror(errno)), 1);
		}
		tmp = tmp->next;
	}
	return (0);
}
// Goes trough the token linked list  
// and gives a tokentype to every node of the list.
int	set_token_types(t_token **tokenlist, t_env_list **env, int *nb_pipe, bool *heredoc)
{
	types_assignement(tokenlist, env, heredoc);
	if(handle_redirections(tokenlist, nb_pipe) == 1)
		return (1);
	clean_up_redirection(tokenlist);
	return (0);
}
