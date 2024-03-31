/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymeziane <ymeziane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 12:59:41 by ymeziane          #+#    #+#             */
/*   Updated: 2024/03/28 13:00:48 by ymeziane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

extern int				g_status;

typedef enum e_type
{
	COMMAND,
	META_CHAR,
	BUILTIN,
	REDIRECTION,
	REDIRECTION_FILE,
	REDIR_EXEC,
	HERE_DOC,
	DELIMITER,
	EMPTY
}						t_type;

typedef enum e_exportcases
{
	VAR_INVALID,
	VAR_DEFINED,
	VAR_UNDEFINED
}						t_exportcases;

typedef enum e_redirections
{
	HEREDOC,
	OUTPUT_APPEND,
	OUTPUT_TRUNCATE,
	INPUT,
}						t_redirections;

typedef struct s_token
{
	char				*element;
	int					ttype;
	struct s_token		*next;
	int					fd_out;
	int					fd_in;
}						t_token;

typedef struct s_data
{
	char				*old_pwd;
	char				*home_dir;
	char				**bin_paths;
	char				*path_cmd;
	struct s_env_list	**env;
	struct s_env_list	**exp_list;
	int					nb_pipe;
	int					**pipe_fds;
	bool				here_doc;
	bool				invalid_file;
}						t_data;

typedef struct s_env_list
{
	char				*variable;
	struct s_env_list	*next;
}						t_env_list;

typedef struct s_heredoc_handler
{
	char				*g_limiter_stored;
	int	g_fd_hd;
}						t_heredoc_handler;

// 1. start with the listening loop that will read the user-input
//	from the terminal.
// 2. scan the user-input and cut it into tokens. for instance:
//	"cat file.txt" would give 2 tokens: "cat"
//-> COMMAND and "file.txt" -> FILENAME.
// Save data on each token in a struct.
// 3. Start executing the user-input by going trough each token
//	one by one.

//////////////////////////////////////////////////////////////////
//	TOKENIZE FOLDER

//	VARIABLE FOLDER
//	CHECK VARIABLE
char					*convert_element(char *element, t_env_list **env);
int						is_or_has_a_variable(char *element);
// CONVERT TO VALUE
char					*get_var_value(char *var_name, t_env_list **env);
char					*extract_var_name_from_ele(char *var_in_element);
void					convert_var_into_value(char *element_at_var,
							char *new_element, t_env_list **env, int index[2]);
void					convert_exit_status_into_value(char *new_element,
							int index[2]);

// CLEAN UP
t_token					*clean_up_quotes(t_token *tmp);
int						clean_up_tokens(t_token **tokenlist, t_data **data);
// FILL TOKENLIST
t_token					**tokenize(char *line, t_data **data);
t_token					*create_new_token(char *element);
//////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
//	BUILTINS FOLDER

//	EXPORT FOLDER
//	EXPORT UTILS
int						check_var(char *arg, t_env_list **export_variables,
							int offset);
char					*append_declare_prefix_and_quotes(char *variable);
void					sort_alphabetically(t_env_list **expvars, int size);
//	EXPORT
t_env_list				**get_export_variables(t_env_list **env);
void					export(char **args, t_env_list **env,
							t_env_list **exp_list);

//	CD
void					cd(char *absolute_path, t_data **data);
//	ECHO
void					echo(char **args);
//	ENV
void					env(char **args, t_env_list **env);
//	EXIT
void					exit_bash(char **args, t_data **data, t_token **token, char **expression);
//	PWD
void					pwd(void);
char					*get_current_dir(void);
//	UNSET
char					*extract_var_name(char *arg);
void					unset(char **args, t_env_list **env,
							t_env_list **exp_list);
int						compute_bytes_to_cmp(char *variableinlist,
							char *variablename);
//////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
//	EXECUTION FOLDER

//  EXEC LINE FOLDER
// EXEC LINE
void					free_after_execution(t_token **tokenlist, t_data **data,
							char **args, char **expression);
void					execute_line(t_token **tokenlist, t_data **data);
// EXEC EXPRESSION
pid_t					exec_expression(t_token **tokenlist, t_data **data,
							int index, char **args);

//	EXEC_BUILTINS
void					exec_builtins(t_token **tokenlist, t_data **data,
							char **expression, char **args);
bool					check_and_exec_single_builtin(t_token **tokenlist,
							t_data **data, char **args);
//	EXEC_UTILS
char					*get_path_cmd(char **paths, char *cmd);
char					**cut_arrays_into_expression(char **array, int index);
char					**tokens_to_array(t_token **token);
// INIT PIPES
int						**init_pipes(t_data **data);
void					close_all_pipes(t_token **tokenlist, int **fds,
							int nb_pipe);
//	REDIRECTIONS
int						get_output_fd(t_token **tokenlist, int index);
int						get_input_fd(t_token **tokenlist, int index);
int						configure_io(t_token **tokenlist, int index,
							t_data **data);
void						set_up_heredoc(int fd, char *limiter);
int						write_to_heredoc(bool command, t_data **data, t_token **tokenlist);
int	create_or_append_helper(t_token *tmp, t_token *command_token, t_data **data);
int	create_or_truncate_helper(t_token *tmp, t_token *command_token, t_data **data);
void	read_from_file_helper(t_token *tmp, t_token *command_token);
//////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
//	UTILS FOLDER
//	LST UTILS
int						lst_del_one(t_env_list **lst, char *variable,
							int offset);
void					lst_add_back(t_env_list **lst, t_env_list *new);
t_env_list				*lst_new(char *variable);
t_env_list				*lst_last(t_env_list *lst);
int						lst_size(t_env_list **lst);
//	PRINT UTILS
void					print_not_found(char *command, char *arg);
void					print_error(char *command, char *arg,
							char *error_message);
int						error_syntax(t_token *tmp, int *nb_pipe);
void					print_export(t_env_list **export_variables);
//	ENV UTILS
t_env_list				**dup_env(char **env);
char					**find_bin_paths(t_env_list **env);
char					*get_env(char *variable, t_env_list **env);
char					**env_list_to_array(t_env_list **env);
//////////////////////////////////////////////////////////////////
//	FREE FOLDER
//	FREE ARRAY
void					free_double_array(char **darray);
void					free_fds_array(int **fds, int nb_pipe);
//	FREE STRUCT
void					free_lst_content(t_env_list *lst, char *var_name);
void					free_data_struct(t_data *data);
void					free_token_list(t_token **t_token);
void					free_node(t_token *tmp);
//////////////////////////////////////////////////////////////////
// PARSING FOLDER

// REDIRECTIONS FOLDER
// FD REDIRECTS
int						create_or_append(t_token *tmp, t_token *command_token,
							t_data **data);
int						create_or_truncate(t_token *tmp, t_token *command_token,
							t_data **data);
void					create_and_read_from_heredoc(t_token *tmp,
							t_token *command_token, t_data **data,
							t_token **tokenlist);
void					read_from_file(t_token *tmp, t_token *command_token);
// PARS REDIRECTS
void					clean_up_redirection(t_token **tokenlist);
int						handle_redirections(t_token **tokenlist, t_data **data);

// CHECK CMD
int						is_a_command(char *element, t_env_list **env);
// SET TYPE
int						type(char *element, t_env_list **env);
int						set_token_types(t_token **tokenlist, t_data **data);
//////////////////////////////////////////////////////////////////

//	LOOP
void					listening_loop(t_data **data);

//	PARSING
int						set_token_types(t_token **tokenlist, t_data **data);
int						type(char *element, t_env_list **env);

//	SIGNALS
void					init_signals(void);
int						exited_status(int status);

#endif
