#include "minishell.h"

void	listening_loop(void) //don't forget to free the line
{
	char	*line;
	t_token **tokenlist;
	t_token *tmp;

	while(1)
	{
		line = readline("minishell> ");
		tokenlist = tokenize(line);
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

	printf("Value = %s\n", getenv("LANGUAGE"));
	argc += 1;
	*argv += 1;
	envdup = dup_env(envp);
	bin_paths = find_bin_paths(envdup);	
	printf("%d\n\n\n", is_a_command("ls", bin_paths)); 
	//listening_loop();
}
