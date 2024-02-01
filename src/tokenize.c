/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 15:23:42 by ymeziane          #+#    #+#             */
/*   Updated: 2024/02/01 15:48:06 by maxborde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

//Creates a new token, adds a duplicate of element and returns it.
t_token	*create_new_token(char *element)
{
	t_token	*token;

	printf("start createnewtoken\n");
	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->element = ft_strdup(element);
	token->next = NULL;
	return (token);
}

//Returns the size of the element between quotes. 
//Handles " and ' quotes.
int	compute_quotes_size(char *line)
{
	int	i;

	i = 0;
	if(line[i] == 39)
	{
		i++;
		while(line[i] && line[i] != 39)
			i++;
		i++;
	}
	else if(line[i] == 34)
	{
		i++;
		while(line[i] && line[i] != 34)
			i++;
		i++;
	}
	return (i);
}


//Takes the pointer to line and extracts and returns the first element  it encounters.
//We first calculate the lenght of the element and then duplicate it. If the element starts with 
//quote we go until the next quote.
char	*get_element(char *line)
{
	char	*element;		
	int	i;
	int	j;

	printf("start getelement\n");
	i = 0;
	j = 0;
	printf("Line in GE = %s\n", line);
	if (line[i] == 39 || line[i] == 34)
		i += compute_quotes_size(line);
	else
	{
		while (line[i] && line[i] != ' ' && line[i] != 39 && line[i] != 34)
			i++;	
	}
	printf("Size of element = %d\n", i);
	element = malloc(sizeof(char) * (i + 1));
	while (j < i)
	{
		element[j] = line[j];
		j++;
	}
	element[j] = 0;
	return (element);
}

//Creates and adds a new token to the end of tokenlist. Returns the pointer to line incremented
//to the end of the element.
char	*add_token(char	*line, t_token **tokenlist)
{
	t_token	*tmp;

	if (!(*tokenlist))
	{
		printf("start addtoken\n");
		*tokenlist = create_new_token(get_element(line));
		printf("Element: %s\n", (*tokenlist)->element);
		return (line + ft_strlen((*tokenlist)->element));
	}
	tmp = *tokenlist;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = create_new_token(get_element(line));
	return (line + ft_strlen(tmp->next->element));
}

char	*clean_up_quotes(char *element)
{
	if (*element == 39)
	{
		printf("strlen = %ld\n", ft_strlen(element));
		if(element[ft_strlen(element) - 1] == 39)
			return(ft_strndup(element + 1, ft_strlen(element) - 2));
					
	}
	if (*element == 34)
	{
		if(element[ft_strlen(element) - 1] == 34)
			return(ft_strndup(element + 1, ft_strlen(element) - 2));
	}
	return (element);
}

void	clean_up_tokens(t_token **tokenlist)
{
	t_token	*tmp;

	tmp = *tokenlist;
	while (tmp)
	{
		tmp->element = clean_up_quotes(tmp->element);
		tmp = tmp->next;
	}
}

//Returns a pointer to the first element of the linked list of all the tokens in 
//the bash expression.
//You go trough the line, skip whitespaces, and adds a token every time it encounters
//an element of a bash expression. The add token while increment the line pointer 
//to the end of the element so you can then continue trough the rest of the line.
t_token	**tokenize(char	*line)
{
	t_token	**tokenlist;

	tokenlist = malloc(sizeof(t_token*));
	*tokenlist = NULL;
	printf("start tokenize\n");
	while (*line)
	{
		if ((*line < 9 || *line > 13) && *line != ' ')
			line = add_token(line, tokenlist);
		else
			line++;
		printf("Line = %s\n", line);
	}
	clean_up_tokens(tokenlist);
	return(tokenlist);
}
