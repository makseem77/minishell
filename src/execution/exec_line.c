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

static void	configure_io(int index, int **fds, t_data **data)
{
	if (index == 0)
		dup2(fds[index][1], STDOUT_FILENO);
	else if (index == (*data)->nb_pipe)
		dup2(fds[index - 1][0], STDIN_FILENO);
	else
	{
		dup2(fds[index - 1][0], STDIN_FILENO);
		dup2(fds[index][1], STDOUT_FILENO);
	}
}

void	exec(t_token **tokenlist, t_data **data, int index, int **fds, char **args)
{
	char **bin_paths;
	char *path_cmd;
	pid_t pid;
	char **expression;

	if ((*data)->nb_pipe == 0)
	{
		expression = args;
		if(type(expression[0], (*data)->env) == BUILTIN)
		{
			execute_bultin(tokenlist, data, expression);
			return ;
		}
	}
	expression = cut_arrays_into_expression(args, index);
	bin_paths = find_bin_paths((*data)->env);
	path_cmd = get_path_cmd(bin_paths, expression[0]);
	free_double_array(bin_paths);
	printf("REACHED FORK\n");
	pid = fork();
	if (pid == 0)
	{
		if ((*data)->nb_pipe > 0)
		{
			printf("REACHED IO\n");
			configure_io(index, fds, data);
		}
		close_all_pipes(fds, (*data)->nb_pipe);
		if (type(expression[0], (*data)->env) == BUILTIN)
		{
			execute_bultin(tokenlist, data, expression);
			free(path_cmd);
			free_double_array(expression);
			free_double_array(args);
			free_fds_array(fds, (*data)->nb_pipe);
			free_data_struct(*data);
			free_token_list(tokenlist);
			exit(EXIT_SUCCESS);
		}
		if (type(expression[0], (*data)->env) == COMMAND)
		{
			if (execve(path_cmd, expression, env_list_to_array((*data)->env)) == -1)
				exit(1);
		}
		else
		{
			print_error(expression[0], NULL, "command not found");
			free(path_cmd);
			free_double_array(expression);
			free_double_array(args);
			free_fds_array(fds, (*data)->nb_pipe);
			free_data_struct(*data);
			free_token_list(tokenlist);
			exit(EXIT_FAILURE);
		}
	}
	free(path_cmd);
	free_double_array(expression);
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
	free_double_array(args);
	free_fds_array(fds, (*data)->nb_pipe);
	(*data)->nb_pipe = 0;
	state = 0;
}
