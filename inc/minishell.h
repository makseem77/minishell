#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/inc/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <strings.h>
# include <sys/wait.h>
# include <unistd.h>

extern bool				state;

typedef enum e_type
{
	COMMAND,
	META_CHAR,
	BUILTIN
}						t_type;

typedef enum e_exportcases
{
	VAR_INVALID,
	VAR_DEFINED,
	VAR_UNDEFINED
}						t_exportcases;

typedef struct s_token
{
	char				*element;
	int					ttype;
	struct s_token		*next;
}						t_token;

typedef struct s_data
{
	char				*old_pwd;
	char				*home_dir;
	struct s_env_list	**env;
	struct s_env_list	**exp_list;
	int					nb_pipe;
}						t_data;

typedef struct s_env_list
{
	char				*variable;
	struct s_env_list	*next;
}						t_env_list;

// 1. start with the listening loop that will read the user-input from the terminal.
// 2. scan the user-input and cut it into tokens. for instance: "cat file.txt" would give 2 tokens: "cat"
//-> COMMAND and "file.txt" -> FILENAME.
// Save data on each token in a struct.
// 3. Start executing the user-input by going trough each token one by one.

//////////////////////////////////////////////////////////////////////////////////////
//	TOKENIZE FOLDER
//	TOKENIZE
t_token					**tokenize(char *line, t_env_list **env);

//	TOKENIZE UTILS
size_t					compute_len(char *element);
size_t					count_del_quotes(char *element);
char					*clean_up_quotes(char *element);
void					clean_up_tokens(t_token **tokenlist);

//	HANDLE_ENV_VARS
char					*replace_in_line(char *line, t_env_list **env);
//////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////
//	BUILTINS FOLDER

//	EXPORT FOLDER
//	EXPORT
t_env_list				**get_export_variables(t_env_list **env);
void					export(char **args, t_env_list **env,
							t_env_list **exp_list);
//	EXPORT UTILS
int						check_var(char *arg, t_env_list **export_variables,
							int offset);
char					*append_declare_prefix_and_quotes(char *variable);
void					sort_alphabetically(t_env_list **expvars, int size);

//	CD
void					cd(char *absolute_path, t_data **data);
//	ECHO
void					echo(char **args);
//	ENV
void					env(char **args, t_env_list **env);
//	EXIT
void					exit_bash(char *status, t_data **data, t_token **token);
//	PWD
void					pwd(void);
char					*get_current_dir(void);
//	UNSET
char					*extract_var_name(char *arg);
void					unset(char **args, t_env_list **env,
							t_env_list **exp_list);
int	compute_bytes_to_cmp(char *variableinlist,
							char *variablename);
//////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////
//	EXECUTION FOLDER
//	EXEC_BUILTINS
void					execute_bultin(t_token **tokenlist, t_data **data,
							char **expression);
size_t					count_tokens(t_token **token);
char					**tokens_to_array(t_token **token);
//	EXEC_LINE
void					execute_line(t_token **tokenlist, t_data **data);
//	EXEC_UTILS
char					*get_path_cmd(char **paths, char *cmd);
// INIT PIPES
int						**init_pipes(t_data **data);
void					close_all_pipes(int **fds, int nb_pipe);
//////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////
//	UTILS FOLDER
//	LST UTILS
int						lst_del_one(t_env_list **lst, char *variable,
							int offset);
void					lst_add_back(t_env_list **lst, t_env_list *new);
t_env_list				*lst_new(char *variable);
t_env_list				*lst_last(t_env_list *lst);
int						lst_size(t_env_list **lst);
//	PRINT UTILS
void					print_error(char *command, char *arg,
							char *error_message);
void					print_export(t_env_list **export_variables);
//	ENV UTILS
t_env_list				**dup_env(char **env);
char					**find_bin_paths(t_env_list **env);
char					*get_env(char *variable, t_env_list **env);
char					**env_list_to_array(t_env_list **env);
//////////////////////////////////////////////////////////////////////////////////////

//	FREE
void					free_token_list(t_token **t_token);
void					free_double_array(char **darray);
void	free_variable_lists(t_env_list **export_list,
							t_env_list **env_list);
void					free_data_struct(t_data *data);
void					free_lst_content(t_env_list *lst, char *var_name);
void					free_fds_array(int **fds, int nb_pipe);

//	LOOP
void					listening_loop(t_data **data);

//	PARSING
int						set_token_types(t_token **tokenlist, t_env_list **env,
							int *nb_pipe);
int						type(char *element, t_env_list **env);

//	SIGNALS
void					handle_signals(void);

#endif
