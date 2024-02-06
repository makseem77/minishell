#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/inc/libft.h"
# include <stdio.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdbool.h>

enum				ttype
{
	COMMAND,
	META_CHAR,
	BUILTIN,
	STRING
};

typedef struct s_token
{
	char			*element;
	int				pos;
	int				ttype;
	struct s_token	*next;
}					t_token;

//1. start with the listening loop that will read the user-input from the terminal.
//2. scan the user-input and cut it into tokens. for instance: "cat file.txt" would give 2 tokens: "cat" -> COMMAND and "file.txt" -> FILENAME.
//Save data on each token in a struct.
//3. Start executing the user-input by going trough each token one by one.

//	TOKENIZE
t_token				**tokenize(char *line, char **env);
char				*add_token(char *line, t_token **tokenlist, char **env);
char				*get_element(char *line);
t_token				*create_new_token(char *element);
char				*replace_in_line(char *line, char **env);
int				has_a_variable(char *element);


//	ENV
char				**dup_env(char **env);
char				**find_bin_paths(char **env);

//	PARSING
void	set_token_types(t_token **tokenlist, char **env);

//	LOOP
void	listening_loop(char **env);

//	HANDLE_ENV_VARS
char	*replace_in_line(char *line, char **env);

#endif
