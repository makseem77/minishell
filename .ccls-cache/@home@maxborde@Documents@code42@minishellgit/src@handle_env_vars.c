/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_env_vars.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxborde <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 18:43:05 by maxborde          #+#    #+#             */
/*   Updated: 2024/02/06 23:21:26 by maxborde         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//Returns the size of the first $VAR it encounters in line.
//A $VAR can only be composed of alphadigits and underscores.
int	compute_var_len(char *line)
{
	int	i;
	int	varlen;

	i = 0;
	varlen = 0;
	while (line[i])
	{
		if (line[i] == '$')
		{
			i++;
			while (ft_isdigit(line[i]) || ft_isalpha(line[i]) || line[i] == '_')
			{
				i++;
				varlen++;
			}
			return (varlen);
		}
		i++;
	}
	return (0);
}

//This function takes care of doing update of the line in the cases when the
//$VAR is not an env. It updates only the first occurence of a $VAR.
char	*update_line_with_void(char *line, int newlinelen)
{
	char	*newline;
	int	i;
	int	j;
	
	i = 0;
	j = 0;
	newline = malloc(sizeof(char) * (newlinelen + 1));
	printf("Size of line = %d\n", newlinelen);
	printf("Line bu = %s\n", line);
	while (line[i])
	{
		if (line[i] == '$')
		{
			printf("Xnewline = %c\n", *newline);
			i++;
			while (isalpha(line[i]) || isdigit(line[i]) || line[i] == '_')
				i++;
		}
		else
		{
			newline[j] = line[i];
			printf("newline = %c\n", *newline);
			j++;
			i++;
		}
	}
	newline[j] = 0;
	printf("Line au = %s\n", newline);
	return (newline);
}

//This function takes care of doing update of the line in the cases when the
//$VAR is in env. It updates only the first occurence of a $VAR.
char	*update_line_with_value(char *line, char **env, int newlinelen)
{
	char	*newline;
	int	varlen;
	int	i;
	int	j;
	int	replacedvar;

	newline = malloc(sizeof(char) * (newlinelen + 1));
	varlen = compute_var_len(line);
	i = 0;
	j = 0;
	replacedvar = 0;
	if (*env)
	{
		while (line[i])
		{
			if (line[i] == '$' && replacedvar == 0)
			{
				ft_strlcpy(&newline[i], *env + varlen + 1, ft_strlen(*env + varlen + 1) + 1);
				printf("VALUELEN = %ld\n", ft_strlen(*env + varlen + 1) + 1);
				printf("NEWLINE = %s\n", newline);
				i += varlen + 1;
				j += ft_strlen(*env + varlen + 1);
				replacedvar += 1;
			}
			else
			{
				newline[j] = line[i];
				i++;
				j++;
			}
		}
		newline[j] = 0;
	}
	else
		newline = update_line_with_void(line, newlinelen);
	return (newline);
}

//This function replaces the line by a newline duplicate that has the $VAR replaced
//by its value. It will only replace the first $VAR it encounters, and leave the others
//unchanged as the function will be called again in the tokenizing if there is still a
//$VAR in line. Here we check if the $VAR is in the env, and if so we replace it by it's value
//in update_line. If it's not in the env, we replace it by nothing.
char	*replace_in_line(char *line, char **env)
{
	char	*var;
	int	newlinelen;
	int	varlen;

	varlen = compute_var_len(line);
	var = ft_strndup(ft_strchr(line, '$'), varlen + 1);
	while (*env && ft_strncmp(var + 1, *env, varlen) != 0)
		env++;
	if (*env)
		newlinelen = ft_strlen(line) - varlen + ft_strlen(*env + (varlen + 1));	
	else
		newlinelen = ft_strlen(line) - (varlen + 1);
	return (update_line_with_value(line, env, newlinelen));
}
