/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 15:23:35 by ymeziane          #+#    #+#             */
/*   Updated: 2024/03/14 16:52:10 by ymeziane         ###   ########.fr       */
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

// Goes trough the token linked list
// and gives a tokentype to every node of the list.
int	set_token_types(t_token **tokenlist, t_env_list **env, int *nb_pipe, bool *heredoc)
{
	t_token	*tmp;
	t_token	*last_cmd;

	last_cmd = NULL;
	tmp = *tokenlist;
	while (tmp)
	{
		if (ft_strcmp(tmp->element, "|") == 0)
		{
			if (!tmp->next)
			{
				ft_putstr_fd("minishell: pipe should be followed by a command\n",
								2);
				return (1);
			}
			else
				(*nb_pipe)++;
		}
		else if (ft_strcmp(tmp->element, ";") == 0 || ft_strcmp(tmp->element,
					"\\") == 0)
			return (ft_putstr_fd("minishell: special characters ');' or '\\' are not authorized\n",
									2),
					1);
		else if (ft_strcmp(tmp->element, ">") == 0 || ft_strcmp(tmp->element,
					">>") == 0 || ft_strcmp(tmp->element, "<") == 0 || ft_strcmp(tmp->element, "<<") == 0)
		{
			tmp->ttype = REDIRECTION;
			t_token *symbol_token = tmp;
			tmp = tmp->next;
			if (tmp)
			{
				tmp->ttype = REDIRECTION_FILE;
				if(ft_strcmp(symbol_token->element, ">>") == 0)
				{
					if(last_cmd)
						last_cmd->fd_out = open(tmp->element, O_CREAT | O_RDWR | O_APPEND, 0644);
					else if(symbol_token->next->next && (type(symbol_token->next->next->element, env) == COMMAND || type(symbol_token->next->next->element, env) == BUILTIN))
						symbol_token->next->next->fd_out = open(tmp->element, O_CREAT | O_RDWR | O_APPEND, 0644);
				}
				else if(ft_strcmp(symbol_token->element, ">") == 0)
				{
					if(last_cmd)
						last_cmd->fd_out = open(tmp->element, O_CREAT | O_RDWR | O_TRUNC, 0644);
					else if(symbol_token->next->next && (type(symbol_token->next->next->element, env) == COMMAND || type(symbol_token->next->next->element, env) == BUILTIN))
						symbol_token->next->next->fd_out = open(tmp->element, O_CREAT | O_RDWR | O_TRUNC, 0644);
				}
				else if (ft_strcmp(symbol_token->element, "<<") == 0)
				{
					*heredoc = true;
					if(last_cmd)
					{
						last_cmd->fd_in = open("tmp", O_CREAT | O_RDWR | O_TRUNC, 0644);
						write_to_heredoc(last_cmd->fd_in, tmp->element);
					}
					else if(symbol_token->next->next && (type(symbol_token->next->next->element, env) == COMMAND || type(symbol_token->next->next->element, env) == BUILTIN))
					{
						symbol_token->next->next->fd_in = open("tmp", O_CREAT | O_RDWR | O_TRUNC, 0644);
						write_to_heredoc(symbol_token->next->next->fd_in, tmp->element);
					}
					else
						write_to_heredoc(open("tmp", O_CREAT | O_RDWR | O_TRUNC, 0644), tmp->element);
					symbol_token->ttype = HERE_DOC;
					tmp->ttype = DELIMITER;
				}
				else if(ft_strcmp(symbol_token->element, "<") == 0)
				{
					if(last_cmd)
					{
						last_cmd->fd_in = open(tmp->element, O_RDWR, 0644);
						if (last_cmd->fd_in == -1)
							return(print_error(tmp->element, NULL, strerror(errno)), 1);
					}
					else if(symbol_token->next->next && (type(symbol_token->next->next->element, env) == COMMAND || type(symbol_token->next->next->element, env) == BUILTIN))
						symbol_token->next->next->fd_in = open(tmp->element, O_RDWR, 0644);
				}
			}
			else
				return(ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", 2), 1);
		}
		else if (type(tmp->element, env) == BUILTIN)
		{
			tmp->ttype = BUILTIN;
			last_cmd = tmp;
		}
		else if (type(tmp->element, env) == META_CHAR)
			tmp->ttype = META_CHAR;
		else if (type(tmp->element, env) == COMMAND)
		{
			tmp->ttype = COMMAND;
			last_cmd = tmp;
		}
		tmp = tmp->next;
	}
	clean_up_redirection(tokenlist);
	return (0);
}
