#ifndef MINISHELL_H
#define MINISHELL_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../lib/libft.h"

enum	ttype{COMMAND, COMMAND_ARGS, SPE_CHAR, BUILTIN};

typedef struct s_token
{
	char	*element;
	int				pos;
	int	ttype;
	struct s_token	*next;
}					t_token;

//1. start with the listening loop that will read the user-input from the terminal.
//2. scan the user-input and cut it into tokens. for instance: "cat file.txt" would give 2 tokens: "cat" -> COMMAND and "file.txt" -> FILENAME.
//Save data on each token in a struct.
//3. Start executing the user-input by going trough each token one by one.
 
t_token	**tokenize(char	*line);
char	*add_token(char	*line, t_token **tokenlist);
char	*get_element(char *line);
t_token	*create_new_token(char *element);

char	**dup_env(char **env);
char	**find_bin_paths(char **env);
int	is_a_command(char *element, char **bin_paths);

void	set_token_types(t_token **tokenlist);

#endif
