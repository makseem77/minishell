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

int	get_fd_in_and_out(t_token **tokenlist, int *fd_in, int *fd_out)
{
	t_token	*tmp;

	tmp = *tokenlist;
	while (tmp)
	{
		if (tmp->fd_in != -1)
			*fd_in = tmp->fd_in;
		if (tmp->fd_out != -1)
			*fd_out = tmp->fd_out;
		tmp = tmp->next;
	}
	return (0);
}

void    configure_io(int index, int **fds, int nb_pipe, int fd_in, int fd_out)
{
    if (index == 0 && nb_pipe > 0)
    {
        dup2(fds[0][1], STDOUT_FILENO);
        if (fd_in != -1)
            dup2(fd_in, STDIN_FILENO);
    }
    else if (index > 0 && index < nb_pipe)
    {
        dup2(fds[index - 1][0], STDIN_FILENO);
        dup2(fds[index][1], STDOUT_FILENO);
    }
    else if (index == nb_pipe && nb_pipe > 0)
    {
        dup2(fds[nb_pipe - 1][0], STDIN_FILENO);
        if (fd_out != -1)
            dup2(fd_out, STDOUT_FILENO);
    }
    else if (nb_pipe == 0)
    {
        if (fd_in != -1)
            dup2(fd_in, STDIN_FILENO);
        if (fd_out != -1)
            dup2(fd_out, STDOUT_FILENO);
    }
    close_all_pipes(fds, nb_pipe);
    if (fd_in != -1)
        close(fd_in);
    if (fd_out != -1)
        close(fd_out);
}


void	exec(t_token **tokenlist, t_data **data, int index, int **fds,
		char **args, int fd_in, int fd_out)
{
	char	**bin_paths;
	char	*path_cmd;
	pid_t	pid;
	char	**expression;
	int		temp_stdout;
	int		temp_stdin;

	if ((*data)->nb_pipe == 0)
	{
		expression = args;
		if (type(expression[0], (*data)->env) == BUILTIN)
		{
			temp_stdin = -1;
			if (fd_in != -1)
			{
                printf("fd_in: %d\n", fd_in);
				temp_stdin = dup(STDIN_FILENO);
				if (dup2(fd_in, STDIN_FILENO) == -1)
					perror("dup2 - fd_in");
				close(fd_in);
			}
			if (fd_out != -1)
			{
				temp_stdout = dup(STDOUT_FILENO);
				if (dup2(fd_out, STDOUT_FILENO) == -1)
					perror("dup2 - fd_out");
				close(fd_out);
				execute_bultin(tokenlist, data, expression);
				if (dup2(temp_stdout, STDOUT_FILENO) == -1)
					perror("dup2 restore stdout");
				close(temp_stdout);
			}
			else
				execute_bultin(tokenlist, data, expression);
			if (temp_stdin != -1)
			{
				if (dup2(temp_stdin, STDIN_FILENO) == -1)
					perror("dup2 restore stdin");
				close(temp_stdin);
			}
			return ;
		}
	}
	expression = cut_arrays_into_expression(args, index);
	bin_paths = find_bin_paths((*data)->env);
	path_cmd = get_path_cmd(bin_paths, expression[0]);
	free_double_array(bin_paths);
	pid = fork();
	if (pid == 0)
	{
		configure_io(index, fds, (*data)->nb_pipe, fd_in, fd_out);
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
			if (execve(path_cmd, expression, env_list_to_array((*data)->env)) ==
				-1)
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
	int i;
	int **fds;
	char **args;
	int fd_in;
	int fd_out;

    fd_in = -1;
    fd_out = -1;
	get_fd_in_and_out(tokenlist, &fd_in, &fd_out);
    printf("fd_in: %d\n", fd_in);
	args = tokens_to_array(tokenlist);
	state = 1;
	fds = init_pipes(data);
	i = (*data)->nb_pipe;
	while (i >= 0)
	{
		exec(tokenlist, data, i, fds, args, fd_in, fd_out);
		i--;
	}
	i = (*data)->nb_pipe;
	while (i > 0)
	{
		close(fds[i - 1][1]);
		i--;
	}
	if (fd_in != -1)
		close(fd_in);
	if (fd_out != -1)
		close(fd_out);
	while (wait(NULL) > 0)
		;
	close_all_pipes(fds, (*data)->nb_pipe);
	free_double_array(args);
	free_fds_array(fds, (*data)->nb_pipe);
	(*data)->nb_pipe = 0;
	state = 0;
}