#include "minishell.h"

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
			printf("Element = %s\n", tmp->element);
			tmp = tmp->next;
		}
	}
}
