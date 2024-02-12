/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 17:40:09 by ymeziane          #+#    #+#             */
/*   Updated: 2024/02/12 17:44:34 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
