/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 15:23:42 by ymeziane          #+#    #+#             */
/*   Updated: 2024/03/02 23:26:26 by maxborde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Creates a new token, adds a duplicate of element and returns it.
static t_token	*create_new_token(char *element)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
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

// Will return 1 if the element is or has a variable ($VAR), 0 if
// it is not and -1 if we have unclosed quotes.
// "$VAR" is interpreted as a variable where '$VAR' is not.
static int	is_or_has_a_variable(char *element)
{
	int	dquotesflag;
	int	squotesflag;
	int	i;

	dquotesflag = 0;
	squotesflag = 0;
	i = 0;
	while (element[i])
	{
		if (element[i] == '$' && (squotesflag % 2 == 0)
			&& (isalpha(element[i + 1]) || isdigit(element[i + 1])
				|| element[i + 1] == '_'))
			return (1);
		if (element[i] == '\'' && dquotesflag % 2 == 0)
			squotesflag++;
		if (element[i] == '"' && squotesflag % 2 == 0)
			dquotesflag++;
		i++;
	}
	return (0);
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
	char	*newline;

	element = get_element(line);
	elementlen = ft_strlen(element);
	if (is_or_has_a_variable(element))
	{
		free(element);
		newline = replace_in_line(line, env);
		return (newline);
	}
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
	clean_up_tokens(tokenlist);
	return (tokenlist);
}
