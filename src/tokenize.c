/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 15:23:42 by ymeziane          #+#    #+#             */
/*   Updated: 2024/02/10 00:13:17 by maxborde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

//Creates a new token, adds a duplicate of element and returns it.
t_token	*create_new_token(char *element)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->element = ft_strdup(element);
	token->next = NULL;
	return (token);
}

//Returns the len of the element, but without quoting the double quotes and single quotes.
//We only count the dquotes or squotes that are inside quotes.
size_t compute_len(char *element)
{
	size_t len;
	int flag_double_quotes;
	int flag_single_quotes;

	len = 0;
	flag_double_quotes = 0;
	flag_single_quotes = 0;
	while (element[len])
	{
		if(element[len] == ' ' && flag_double_quotes % 2 == 0 && flag_single_quotes % 2 == 0)
			break;
		if (element[len] == '"' && flag_single_quotes % 2 == 0)
			flag_double_quotes++;
		if (element[len] == '\'' && flag_double_quotes % 2 == 0)
			flag_single_quotes++;
		len++;
	}
	return (len);
}

//Takes the pointer to line and extracts and returns the first element  it encounters.
//We first calculate the lenght of the element and then duplicate it. If the element starts with 
//quote we go until the next quote.
char	*get_element(char *line)
{
	char	*element;
	int	i;
	int	j;

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

//Creates and adds a new token to the end of tokenlist. Returns the pointer to line incremented by the len of the element. to the end of the element. If the element is a var ($VAR) it will replace the variable by its value.
//If the element has a variable in it, we replace it by it's value so we can tokenize it. 
char	*add_token(char	*line, t_token **tokenlist, t_env_list **env) 
{
	t_token	*tmp;
	char	*element;
	int	elementlen;

	element = get_element(line);
	elementlen = ft_strlen(element);
	if (has_a_variable(element))
	{
		printf("HAS A VAR\n");
		free(element);
		return (replace_in_line(line, env));
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

//Returns the number of quotes we need to delete on the element.
//We don't count the squotes or dquotes that are inside quotes.
size_t count_del_quotes(char *element)
{
	size_t single_quote;
	size_t double_quote;

	single_quote = 0;
	double_quote = 0;
	while (*element)
	{
		if(*element == '\'' && double_quote % 2 == 0)
			single_quote++;
		if(*element == '"' && single_quote % 2 == 0)
			double_quote++;
		element++;
	}
	if(single_quote % 2 != 0 || double_quote % 2 != 0)
	{
		printf("Error: Unmatched quotes\n");
		exit(EXIT_FAILURE);
	}
	return (single_quote + double_quote);
}

//This function takes the element and returns it cleaned from the quotes.
char	*clean_up_quotes(char *element)
{
	int i = 0;
	int j = 0;
	size_t quotes_count = count_del_quotes(element);
	char *new_element;
	new_element = malloc(ft_strlen(element) - quotes_count + 1);
	while (element[i])
	{
		if(element[i] == '\'')
		{
			i++;
			while(element[j] && element[i] != '\'')
				new_element[j++] = element[i++];
		}
		else if(element[i] == '"')
		{
			i++;
			while(element[j] && element[i] != '"')
				new_element[j++] = element[i++];
		}
		else
			new_element[j++] = element[i];
		i++;
	}
	new_element[j] = '\0';
	free(element);
	return (new_element);
}

//Goes trough all the tokens and cleans up the element in every one of them.
void	clean_up_tokens(t_token **tokenlist)
{
	t_token	*tmp;

	tmp = *tokenlist;
	while (tmp)
	{
		if(ft_strcmp(tmp->element, "\"\"") == 0)
			return;
		tmp->element = clean_up_quotes(tmp->element);
		tmp = tmp->next;
	}
}

//Returns a pointer to the first element of the linked list of all the tokens in 
//the bash expression.
//You go trough the line, skip whitespaces, and adds a token every time it encounters
//an element of a bash expression. The add token while increment the line pointer 
//to the end of the element so you can then continue trough the rest of the line.
t_token	**tokenize(char	*line, t_env_list **env)
{
	t_token	**tokenlist;

	tokenlist = malloc(sizeof(t_token*));
	*tokenlist = NULL;
	while (*line)
	{
		if ((*line < '\t' || *line > '\r') && *line != ' ')
			line = add_token(line, tokenlist, env);
		else
			line++;
	}
	clean_up_tokens(tokenlist);
	return(tokenlist);
}
