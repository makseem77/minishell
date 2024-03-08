/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 17:40:09 by ymeziane          #+#    #+#             */
/*   Updated: 2024/03/08 16:53:26 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Returns the len of the element it encounters in the line.
// Elements in line are separated by whitespaces, but we have to check
// that the whitespaces are not inside quotes.
size_t	compute_len(char *line)
{
	size_t	len;
	int		flag_double_quotes;
	int		flag_single_quotes;

	len = 0;
	flag_double_quotes = 0;
	flag_single_quotes = 0;
	if (*line == '|' || *line == '>')
		return (1);
	while (line[len])
	{
		if ((line[len] == ' ' || line[len] == '|' || line[len] == '>') && flag_double_quotes % 2 == 0
			&& flag_single_quotes % 2 == 0)
			break ;
		if (line[len] == '"' && flag_single_quotes % 2 == 0)
			flag_double_quotes++;
		if (line[len] == '\'' && flag_double_quotes % 2 == 0)
			flag_single_quotes++;
		len++;
	}
	return (len);
}

// Returns the number of quotes we need to delete on the element.
// We don't count the squotes or dquotes that are inside quotes.
size_t	count_del_quotes(char *element)
{
	size_t	single_quote;
	size_t	double_quote;

	single_quote = 0;
	double_quote = 0;
	while (*element)
	{
		if (*element == '\'' && double_quote % 2 == 0)
			single_quote++;
		if (*element == '"' && single_quote % 2 == 0)
			double_quote++;
		element++;
	}
	if (single_quote % 2 != 0 || double_quote % 2 != 0)
	{
		ft_putstr_fd("Error: Unmatched quotes\n", 2);
		exit(EXIT_FAILURE);
	}
	return (single_quote + double_quote);
}

static void	new_element_clean_up(char *element, char *new_element)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (element[i])
	{
		if (element[i] == '\'')
		{
			i++;
			while (element[j] && element[i] != '\'')
				new_element[j++] = element[i++];
		}
		else if (element[i] == '"')
		{
			i++;
			while (element[j] && element[i] != '"')
				new_element[j++] = element[i++];
		}
		else
			new_element[j++] = element[i];
		i++;
	}
	new_element[j] = '\0';
}

// This function takes the element and returns it cleaned from the quotes.
char	*clean_up_quotes(char *element)
{
	char	*new_element;

	new_element = malloc(ft_strlen(element) - count_del_quotes(element) + 1);
	if (!new_element)
		return (NULL);
	new_element_clean_up(element, new_element);
	free(element);
	return (new_element);
}

// Goes trough all the tokens and cleans up the element in every one of them.
void	clean_up_tokens(t_token **tokenlist)
{
	t_token	*tmp;

	tmp = *tokenlist;
	while (tmp)
	{
		tmp->element = clean_up_quotes(tmp->element);
		if (tmp->element == NULL)
			return ;
		tmp = tmp->next;
	}
}
