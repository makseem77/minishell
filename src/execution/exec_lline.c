#include "minishell.h"

int	count_tokens(t_token **tokenlist)
{
	int	count;
	t_token	*tmp;

	count = 0;
	tmp = *tokenlist;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	return (count);
}

char	**split_tokens_into_array(t_token **tokenlist)
{
	t_token	*tmp;
	char	**array;
	int	i;

	array = malloc(sizeof(char **) * (count_tokens(tokenlist) + 1));
	tmp = *tokenlist;
	i = 0;
	while (tmp)
	{
		array[i] = ft_strdup(tmp->element);
		i++;
		tmp = tmp->next;
	}
	array[i] = 0;
	return(array);
}

void	print_darray(char **darray)
{
	printf("DARR\n");
	while (*darray)
	{
		printf("DARR %s\n", *darray);
		darray++;
	}
	printf("\n\n");
}

char	**cut_arrays_into_expression(char **array, int index)
{
	int	i;	
	int	pipecount;
	char	**expression;

	i = 0;
	pipecount = 0;
	while(*array && index > 0)
	{
		if (ft_strcmp(*array, "|") == 0)
		{
			if (pipecount == index - 1)
			{
				array++;
				break;
			}
			else
				pipecount++;
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

void	close_all_pipes(int **fds, int nb_pipe)
{
	int	i = 0;

	while(i < nb_pipe)
	{
		close(fds[i][0]);
		close(fds[i][1]);
		i++;
	}
}

void	exec_loop(int index, int **fds, t_data **data, char **bin_paths, char **argv)
{	
	//char	*argv1[] = {"ls", NULL};
	//char	*argv2[] = {"rev", NULL};
	//char	*argv3[] = {"sort", NULL};
	char	*path_cmd;
	char	**expression;

	if (index == 0)
	{
		int c = fork();
		if (c == 0)
		{
			//In exec child process 1
			dup2(fds[0][1], STDOUT_FILENO);
			close_all_pipes(fds, (*data)->nb_pipe);
			expression = cut_arrays_into_expression(argv, index);
			path_cmd = get_path_cmd(bin_paths, expression[0]);
			execve(path_cmd, expression, env_list_to_array((*data)->env));
		}
	}
	else if (index != (*data)->nb_pipe)
	{
		int d = fork();
		if (d == 0)
		{
			//In exec child process 2
			dup2(fds[index - 1][0], STDIN_FILENO);
			dup2(fds[index][1], STDOUT_FILENO);
			close_all_pipes(fds, (*data)->nb_pipe);
			expression = cut_arrays_into_expression(argv, index);
			path_cmd = get_path_cmd(bin_paths, expression[0]);
			execve(path_cmd, expression, env_list_to_array((*data)->env));
		}
	}
	else if (index == (*data)->nb_pipe)
	{
		int x = fork();
		if (x == 0)
		{
			dup2(fds[index - 1][0], STDIN_FILENO);
			close_all_pipes(fds, (*data)->nb_pipe);
			expression = cut_arrays_into_expression(argv, index);
			path_cmd = get_path_cmd(bin_paths, expression[0]);
			printf("index = %d\n", index);
			printf("expr = %s\n", *expression);
			printf("path_cmd = %s\n", path_cmd);
			execve(path_cmd, expression, env_list_to_array((*data)->env));
		}
	}
}

int	**init_pipes(t_data **data)
{
	int	**fds;
	int	i;

	fds = malloc(sizeof(int *) * (*data)->nb_pipe);
	i = 0;
	printf("pipe = %d\n", (*data)->nb_pipe);
	while (i < (*data)->nb_pipe)
	{
		fds[i] = malloc(sizeof(int) * 2);
		pipe(fds[i]);
		i++;
	}
	return (fds);
}

void	execute_single_command(t_data **data, char **argv)
{
	int	f;	
	char	*path_cmd;
	char	**bin_paths;

	bin_paths = find_bin_paths((*data)->env);
	f = fork();
	if (f == 0)
	{
		path_cmd = get_path_cmd(bin_paths, argv[0]);
		execve(path_cmd, argv, env_list_to_array((*data)->env));
	}
}

void	handle_single_command(char **argv, t_data **data, t_token **tokenlist)
{
	if (type(argv[0], (*data)->env) == BUILTIN)	
		execute_bultin(tokenlist, data, argv[0]); 
 	else if (type(argv[0], (*data)->env) == COMMAND)
		execute_single_command(data, argv);
}

void	execute_line(t_token **tokenlist, t_data **data)
{
	(void)tokenlist;
	int	i;
	int	**fds;
	char	**argv;
	char	**bin_paths;
	
	argv = split_tokens_into_array(tokenlist);
	state = 1;
	if ((*data)->nb_pipe == 0)
		handle_single_command(argv, data, tokenlist);
	else
	{
		fds = init_pipes(data);
		bin_paths = find_bin_paths((*data)->env);
		i = 0;
		while (i <= (*data)->nb_pipe)
		{
			exec_loop(i, fds, data, bin_paths, argv);	
			i++;
		}
		printf("End of execute line in main process\n");
		close_all_pipes(fds, (*data)->nb_pipe);
	}
	while (wait(NULL) > 0);
	(*data)->nb_pipe = 0;
	state = 0;
}
