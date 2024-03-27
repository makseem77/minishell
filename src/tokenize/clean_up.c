/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_up.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 11:57:41 by ymeziane          #+#    #+#             */
/*   Updated: 2024/03/27 09:44:46 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	clean_up_new_el(char *element, char *new_element)
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

// Returns the number of quotes we need to delete on the element.
// We don't count the squotes or dquotes that are inside quotes.
static int	count_del_quotes(char *element)
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
		return (-1);
	}
	return (single_quote + double_quote);
}

// This function takes the element and returns it cleaned from the quotes.
t_token	*clean_up_quotes(t_token *tmp)
{
	char	*new_element;
	int		quotes_to_del;

	quotes_to_del = count_del_quotes(tmp->element);
	if (quotes_to_del == -1)
		return (NULL);
	if (ft_strcmp(tmp->element, "\"\"") == 0
		|| ft_strcmp(tmp->element, "\'\'") == 0)
		tmp->ttype = EMPTY;
	new_element = malloc(ft_strlen(tmp->element) - (quotes_to_del) + 1);
	clean_up_new_el(tmp->element, new_element);
	free(tmp->element);
	tmp->element = new_element;
	return (tmp);
}

// Goes trough all the tokens and cleans up the element in every one of them.
int	clean_up_tokens(t_token **tokenlist)
{
	t_token	*tmp;

	tmp = *tokenlist;
	tmp = clean_up_quotes(tmp);
	if (ft_strncmp(tmp->element, "|", 1) == 0)
	{
		print_error(NULL, NULL, "syntax error near unexpected token `|'");
		return (-1);
	}
	while (tmp)
	{
		tmp = clean_up_quotes(tmp);
		if (!tmp)
			return (-1);
		if (tmp->element == NULL)
			return (-1);
		tmp = tmp->next;
	}
	return (1);
}
