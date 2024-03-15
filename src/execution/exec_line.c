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

int	get_output_fd(t_token **tokenlist, int index)
{
	t_token	*tmp;
	int	i;

	tmp = *tokenlist;
	i = 0;
	while (tmp)
	{
		if (tmp->ttype == COMMAND || tmp->ttype == BUILTIN)
		{
			if (i == index)
				return (tmp->fd_out);
			else
				i++;
		}
		tmp = tmp->next;
	}
	return (1);
}

int	get_input_fd(t_token **tokenlist, int index)
{
	t_token	*tmp;
	int	i;

	tmp = *tokenlist;
	i = 0;
	while (tmp)
	{
		if (tmp->ttype == COMMAND || tmp->ttype == BUILTIN)
		{
			if (i == index)
				return (tmp->fd_in);
			else
				i++;
		}
		tmp = tmp->next;
	}
	return (0);
}

void    configure_io(t_token **tokenlist, int index, int **fds, int nb_pipe)
{
	int	fd_out;
	int	fd_in;

	fd_out = get_output_fd(tokenlist, index);
	fd_in = get_input_fd(tokenlist, index);
	// printf("FDout = %d\n", fd_out);
	// printf("FDin = %d\n", fd_in);
	// printf("index = %d\n", index);
	if (index == 0 && nb_pipe > 0)
	{
		dup2(fds[0][1], STDOUT_FILENO);
		if (fd_in)
			dup2(fd_in, STDIN_FILENO);
		if (fd_out)
			dup2(fd_out, STDOUT_FILENO);
	}
	else if (index > 0 && index < nb_pipe)
	{
		dup2(fds[index - 1][0], STDIN_FILENO);
		dup2(fds[index][1], STDOUT_FILENO);
		if (fd_in)
			dup2(fd_in, STDIN_FILENO);
		if (fd_out)
			dup2(fd_out, STDOUT_FILENO);
	}
	else if (index == nb_pipe && nb_pipe > 0)
	{
		dup2(fds[nb_pipe - 1][0], STDIN_FILENO);
		if (fd_in)
			dup2(fd_in, STDIN_FILENO);
		if (fd_out)
			dup2(fd_out, STDOUT_FILENO);
	}	
	else if (index == 0 && nb_pipe == 0)
	{
		dup2(fd_out, STDOUT_FILENO);
		if (fd_in != STDIN_FILENO)
			dup2(fd_in, STDIN_FILENO);
		if (fd_out != STDOUT_FILENO)
			dup2(fd_out, STDOUT_FILENO);
	}
	close_all_pipes(fds, nb_pipe);
}

bool	check_and_exec_single_builtin(t_token **tokenlist, t_data **data, char **args)
{
	char	**expression;
	int saved_stdout;
	int saved_stdin;

	expression = args;
	if (type(expression[0], (*data)->env) == BUILTIN && (*data)->nb_pipe == 0)
	{
		saved_stdout = dup(STDOUT_FILENO);
		saved_stdin = dup(STDIN_FILENO);
		configure_io(tokenlist, 0, NULL, 0);
		execute_bultin(tokenlist, data, expression, args);
		dup2(saved_stdout, STDOUT_FILENO);
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdout);
		close(saved_stdin);
		return true;
	}
	else
		return false;

}

void	free_after_execution(t_token **tokenlist, t_data **data, int **fds, char **args, char **expression, char *path_cmd)
{
	free(path_cmd);
	free_double_array(expression);
	free_double_array(args);
	free_fds_array(fds, (*data)->nb_pipe);
	free_data_struct(*data);
	free_token_list(tokenlist);
}	


void	exec(t_token **tokenlist, t_data **data, int index, int **fds,
		char **args)
{
	char	*path_cmd;
	pid_t	pid;
	char	**expression;

	if (check_and_exec_single_builtin(tokenlist, data, args))
		return;
	expression = cut_arrays_into_expression(args, index);
	path_cmd = get_path_cmd((*data)->bin_paths, expression[0]);
	pid = fork();
	if (pid == 0)
	{
		configure_io(tokenlist, index, fds, (*data)->nb_pipe);
		if (type(expression[0], (*data)->env) == BUILTIN)
		{
			execute_bultin(tokenlist, data, expression, args);
			free_after_execution(tokenlist, data, fds, args, expression, path_cmd);
			exit(EXIT_SUCCESS);
		}
		if (type(expression[0], (*data)->env) == COMMAND)
		{
			if (execve(path_cmd, expression, env_list_to_array((*data)->env)) ==
					-1)
				exit(1);
		}
		else
		{
			print_not_found(expression[0], NULL);
			free_after_execution(tokenlist, data, fds, args, expression, path_cmd);
			exit(127);
		}
	}
	free_double_array(expression);
	free(path_cmd);
}

int		status_child(int status)
{
	int exit_status;

	if (WIFEXITED(status))
		exit_status = WEXITSTATUS(status);
	if (WIFSIGNALED(status))
	{
		exit_status = WTERMSIG(status);
		if (exit_status != 131)
			exit_status += 128;
	}
	if (WIFSTOPPED(status))
		exit_status = WSTOPSIG(status);
	if (WIFCONTINUED(status))
		exit_status = 0;
	return (exit_status);
}

void	execute_line(t_token **tokenlist, t_data **data, int *exit_status)
{
	int i; 
	int **fds; 
	char **args;
	int status;

	args = tokens_to_array(tokenlist);
	state = 1;
	fds = init_pipes(data);
	i = (*data)->nb_pipe;
	while (i >= 0)
	{
		exec(tokenlist, data, i, fds, args);
		i--;
	}
	i = (*data)->nb_pipe;
	while (i > 0)
	{
		close(fds[i - 1][1]);
		i--;
	}
	while (wait(&status) > 0)
		;
	if(!(type(args[0], (*data)->env) == BUILTIN && (*data)->nb_pipe == 0))
		*exit_status = status_child(status);
	close_all_pipes(fds, (*data)->nb_pipe);
	free_double_array(args);
	free_fds_array(fds, (*data)->nb_pipe);
	if((*data)->here_doc)
		unlink("tmp");
	(*data)->nb_pipe = 0;
	state = 0;
}
