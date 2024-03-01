#include "minishell.h"

static char	**cut_arrays_into_expression(char **array, int index)
{
	int		i;
	int		pipecount;
	char	**expression;

	i = 0;
	pipecount = 0;
	while (*array && index > 0)
	{
		if (ft_strcmp(*array, "|") == 0)
		{
			pipecount++;
			if (pipecount == index)
			{
				array++;
				break ;
			}
		}
		array++;
	}
	while (array[i] && ft_strcmp(array[i], "|"))
		i++;
	expression = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (array[i] && ft_strcmp(array[i], "|"))
	{
		expression[i] = ft_strdup(array[i]);
		i++;
	}
	expression[i] = 0;
	return (expression);
}

void	exec(t_token **tokenlist, t_data **data, int index, int **fds, char **args)
{
	char **expression;

	
	for(int i = 0; args[i]; i++)
		printf("args[%d] = %s\n", i, args[i]);
	if ((*data)->nb_pipe == 0)
		expression = args;
	else
		expression = cut_arrays_into_expression(args, index);
	printf("index = %d\n", index);
	printf("expression[0] = %s\n", expression[0]);
	if (type(expression[0], (*data)->env) == BUILTIN)	
		execute_bultin(tokenlist, data, expression); 
 	else if (type(expression[0], (*data)->env) == COMMAND)
		exec_cmd(data, index, fds, expression);
	else
		print_error(expression[0], NULL, "command not found");
}

void	execute_line(t_token **tokenlist, t_data **data)
{
	int	i;
	int	**fds;
	char	**args;
	
	args = tokens_to_array(tokenlist);
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
