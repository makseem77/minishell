/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_tokenlist.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 15:23:42 by ymeziane          #+#    #+#             */
/*   Updated: 2024/03/22 12:27:05 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Creates a new token, adds a duplicate of element and returns it.
t_token	*create_new_token(char *element)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->fd_out = STDOUT_FILENO;
	token->fd_in = STDIN_FILENO;
	token->ttype = -1;
	token->element = ft_strdup(element);
	if (!token->element)
	{
		free(token);
		return (NULL);
	}
	token->next = NULL;
	return (token);
}

// Returns the len of the element it encounters in the line.
// Elements in line are separated by whitespaces, but we have to check
// that the whitespaces are not inside quotes.
static size_t	compute_len(char *line)
{
	size_t	len;
	int		flag_double_quotes;
	int		flag_single_quotes;

	len = 0;
	flag_double_quotes = 0;
	flag_single_quotes = 0;
	if (ft_strncmp(line, ">>", 2) == 0 || ft_strncmp(line, "<<", 2) == 0)
		return (2);
	else if (*line == '|' || *line == '>' || *line == '<')
		return (1);
	while (line[len])
	{
		if ((line[len] == ' ' || (line[len] >= '\t' && line[len] <= '\r')
				|| line[len] == '|' || line[len] == '>' || line[len] == '<')
			&& flag_double_quotes % 2 == 0 && flag_single_quotes % 2 == 0)
			break ;
		if (line[len] == '"' && flag_single_quotes % 2 == 0)
			flag_double_quotes++;
		if (line[len] == '\'' && flag_double_quotes % 2 == 0)
			flag_single_quotes++;
		len++;
	}
	return (len);
}

// Takes the pointer to line and extracts
// and returns the first element it encounters.
// We first calculate the lenght of the element and then duplicate it.
// If the element starts with quote we go until the next quote.
static char	*get_element(char *line)
{
	char	*element;
	int		i;
	int		j;

	i = 0;
	j = 0;
	i = compute_len(line);
	element = malloc(i + 1);
	if (!element)
		return (NULL);
	while (j < i)
	{
		element[j] = line[j];
		j++;
	}
	element[j] = '\0';
	return (element);
}

// Creates and adds a new token to the end of tokenlist.
// Returns the pointer to line incremented by the len of the element
// to the end of the element.
// If the element is a var ($VAR) it will replace the variable by its value.
// If the element has a variable in it,
// we replace it by it's value so we can tokenize it.
static char	*add_token(char *line, t_token **tokenlist, t_env_list **env)
{
	t_token	*tmp;
	char	*element;
	int		elementlen;

	element = get_element(line);
	elementlen = ft_strlen(element);
	if (is_or_has_a_variable(element))
		element = convert_element(element, env);
	if (!(*tokenlist))
	{
		*tokenlist = create_new_token(element);
		free(element);
		return (line + elementlen);
	}
	tmp = *tokenlist;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = create_new_token(element);
	free(element);
	return (line + elementlen);
}

// Returns a pointer to the first element of the linked list
// of all the tokens in the bash expression.
// You go trough the line, skip whitespaces,
// and adds a token every time it encounters an element of a bash expression.
// The add token while increment the line pointer to the end of the element
// so you can then continue trough the rest of the line.
t_token	**tokenize(char *line, t_env_list **env)
{
	t_token	**tokenlist;

	tokenlist = malloc(sizeof(t_token *));
	*tokenlist = NULL;
	while (*line)
	{
		if ((*line < '\t' || *line > '\r') && *line != ' ')
			line = add_token(line, tokenlist, env);
		else
			line++;
	}
	if (clean_up_tokens(tokenlist) == -1)
	{
		free_token_list(tokenlist);
		return (NULL);
	}
	else
		return (tokenlist);
}
