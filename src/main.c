/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/31 15:26:32 by ymeziane          #+#    #+#             */
/*   Updated: 2024/01/31 16:29:16 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	listening_loop(void) //don't forget to free the line
{
	char	*line;
	t_token **tokenlist;
	t_token *tmp;

	while(1)
	{
		line = readline("minishell> ");
        if (!line)
			return;
		if (*line)
			add_history(line);
		tokenlist = tokenize(line);
        free(line);
		tmp = *tokenlist;
		while (tmp)
		{
			printf("Element: %s\n", tmp->element);
			tmp = tmp->next;
		}
	}
}

int	main(int argc, char *argv[], char *envp[])
{
	char	**envdup;
	char	**bin_paths;
    bool   is_a_command_bool;

	printf("Value = %s\n", getenv("LANGUAGE"));
	argc += 1;
	*argv += 1;
	envdup = dup_env(envp);
	bin_paths = find_bin_paths(envdup);
    is_a_command_bool = is_a_command("ls", bin_paths);
	printf("\n\npath found = %d\n", is_a_command_bool);
	listening_loop();
    return (0);
}