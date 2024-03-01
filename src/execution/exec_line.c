#include "minishell.h"

void	exec(t_token **tokenlist, t_data **data, int index, int **fds, char **args)
{
	if (type(args[0], (*data)->env) == BUILTIN)	
		execute_bultin(tokenlist, data, args[0]); 
 	else if (type(args[0], (*data)->env) == COMMAND)
		exec_cmd(data, index, fds, args);
	else
		print_error(args[0], NULL, "command not found");
}

void	execute_line(t_token **tokenlist, t_data **data)
{
	int	i;
	int	**fds;
	char	**args;
	
	args = tokens_to_array(tokenlist);
	for(int i = 0; args[i]; i++)
		printf("args[%d] = %s\n", i, args[i]);
	state = 1;
	fds = init_pipes(data);
	i = (*data)->nb_pipe;
	while (i >= 0)
	{
		exec(tokenlist, data, i, fds, args);
		i--;
	}
	close_all_pipes(fds, (*data)->nb_pipe);
	while (wait(NULL) > 0);
	(*data)->nb_pipe = 0;
	state = 0;
}
