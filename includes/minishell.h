/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 15:03:45 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/30 16:07:08 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>

# define PATH_MAX 4096

# define CMD 0
# define WORD 1
# define PIPE 2
# define REDIR_IN 3
# define REDIR_OUT 4
# define APPEND 5
# define HEREDOC 6
# define END 7
# define FILE_IN 8
# define FILE_OUT 9
# define S_QUOTE 10
# define D_QUOTE 11

extern int				g_exit_status;

typedef struct s_data
{
	char				cur_dir[PATH_MAX];
	char				old_dir[PATH_MAX];
	char				**new_env;
	int					shlvl;
	char				**sorted_env;
}						t_data;

typedef struct s_history
{
	char				*command;
	struct s_history	*next;
}						t_history;

typedef struct s_token
{
	int					type;
	char				*str;
	struct s_token		*next;
	struct s_token		*prev;
}						t_token;

typedef struct s_count
{
	int					nb_pipe;
	int					nb_redir_in;
	int					nb_redir_out;
	int					nb_append;
	int					nb_heredoc;
}						t_count;

typedef struct s_heredoc
{
	char				**eof_heredoc;
	int					p_fd[2];
	int					nb_heredoc;
	bool				hrd;
	int					process;
	struct s_heredoc	*next;
}						t_heredoc;

typedef struct s_exec
{
	int					*nb_cmd;
	int					p_fd[2];
	int					*fd_in;
	int					*fd_out;
	int					process;
	int					*prev_fd;
}						t_exec;

typedef struct s_pipe
{
	int					(*pipe_fds)[2];
	pid_t				*pids;
	int					cmd_idx;
	int					global_idx;
}						t_pipe;

typedef struct s_quote
{
	int					start;
	int					end;
	char				c;
}						t_quote;

typedef struct s_parse
{
	int					w_end;
	int					next_word;
	int					op_len;
}						t_parse;

typedef struct s_export
{
	char				*var_name;
	char				*var_value;
}						t_export;

typedef struct s_shell
{
	char				*str;
	char				*prompt;
	t_data				*data;
	t_token				*token;
	t_history			*history;
	t_count				*count;
	t_exec				*exec;
	t_heredoc			*heredoc;
	int					exit_status;
}						t_shell;

/*BUILTIN*/

/*Hub*/
int						is_simple_builtin(t_shell *shell);
int						check_one_builtin(t_shell *shell);
int						is_target_builtin(t_shell *shell, t_token *target);

/*Cd*/
int						validate_tokens(t_shell *shell, t_token *token_ptr);
int						check_cd(t_shell *shell, t_token *token_ptr);
int						execute_cd(t_shell *shell, char *path_name,
							char *home_dir, char *old_dir);
int						ft_cd(t_shell *shell, char *path_name);
int						check_many_args(t_token *token_ptr);
int						handle_directory(char **dir, char *home_dir,
							char *old_dir, t_shell *shell);
char					*ft_handle_hyphen(char *dir, char *old_dir,
							t_shell *shell);
char					*ft_handle_tilde(char *dir, char *home_dir,
							t_shell *shell);

/*Echo*/
int						ft_count(t_token *current, t_shell *shell);
int						check_echo(t_token *token_ptr, t_shell *shell);

/*Export*/
int						check_export(t_shell *shell, t_token *token_ptr);
int						is_valid_identifier(char *var);
int						check_var_export(char **sorted_env, char *var_name);
void					print_export_value(char *env_var, int equal_sign);
void					print_export_format(char *env_var);
int						invalid_identifier_export(char *var);
int						export_no_args(t_shell *shell);
int						check_export(t_shell *shell, t_token *token_ptr);

/*Exit*/
int						ft_exit(t_shell *shell, t_token *token_ptr);

/*History*/
void					add_to_history(t_shell *shell, char *command);
int						ft_history(t_shell *hub);

/*PWD*/
int						ft_pwd(t_shell *shell);
int						check_pwd(t_shell *shell, t_token *token_ptr);

/*Unset*/
int						ft_unset(t_data *data, char *var_name, t_shell *shell);
int						check_unset(t_shell *shell, t_token *token_ptr);

/*Env*/
int						print_env(t_shell *hub);
char					**build_env(t_shell *shell);
char					**copy_env(char **env, t_shell *shell);
void					env_exists(char **env, t_shell *shell);

/*EXEC*/

/*Error message*/
void					infile_warning_msg(t_shell *shell, char *str);
void					print_heredoc_eof_warning_msg(char *delimiter);
void					print_access_error(t_shell *shell, t_token *current);
void					print_directory_error(t_shell *shell, t_token *current);

/*Fds*/
void					verify_access_fd(t_shell *shell);
void					open_outfile(t_shell *shell);
void					open_infile(t_shell *shell);
void					close_fd_exec(t_shell *shell);
void					setup_heredoc_fds(t_shell *shell);
void					process_redirections(t_shell *shell);

/*Hub*/
void					execute_parsed_line(t_shell *shell);
int						process_heredocs(t_shell *shell);
void					run_global_child_process(t_shell *shell);
void					execute_child(t_shell *shell);
void					execute_pipe(t_shell *shell, int proc_i);

/*Utils*/
void					count_element(t_shell *shell);
void					initiate_exec(t_shell *shell);
void					count_process(t_shell *shell);
void					ft_free_exec(t_shell *shell);
char					**extract_cmd(t_shell *shell, t_token *current,
							int count);

/*Execution*/
int						is_cmd_a_builtin(t_shell *shell, t_pipe *pipe);
char					**give_curr_cmd(t_shell *shell, int i);
int						get_global_cmd_idx(t_shell *shell, int target_proc_i,
							int cmd_in_target_proc_i);
char					*get_path(char *cmd, char **envp, t_shell *shell);
char					*path_util(t_shell *shell, char **path, char *cmd);
void					execute_command(t_shell *shell, char **exec_args,
							t_pipe *pipe);

/*Pipe*/
void					exec_child(t_shell *shell, int proc_i, t_pipe *pipe,
							char **cmd_str);
int						fork_command(t_shell *shell, int proc_i, t_pipe *pipe);
void					execute_all_commands(t_shell *shell, int proc_i,
							int (*pipe_fds)[2], pid_t *pids);
void					setup_pipeline_redir(t_shell *shell, int proc_i,
							t_pipe *pipe);
int						command_in_pipe(t_token *current);

/*Pipe utils*/
void					close_all_pipe_fds(t_shell *shell, int (*pipe_fds)[2],
							int proc_i);
int						create_pipes(t_shell *shell, int (**pipe_fds)[2],
							int proc_i);
void					wait_for_all_commands(t_shell *shell, pid_t *pids,
							int proc_i);

/*Heredoc*/

/*Utils*/
int						handle_heredoc_eof(t_shell *shell, char *delimiter);
int						check_end(int *i, t_shell *shell, t_heredoc **curr);
void					check_and_create(t_shell *shell, t_token *current,
							t_heredoc *tmp);
void					check_current_type(t_token *current, t_heredoc *tmp,
							int process);
void					ft_free_heredoc(t_shell **shell);

/*Process*/
void					close_heredoc_fds(t_shell *shell);
int						process_heredoc_inputs_loop(t_shell *shell);
void					stock_all_heredoc(t_shell *shell);
void					initiate_heredoc(t_shell *shell);
int						create_heredoc_pipes(t_shell *shell);
int						handle_all_heredocs(t_shell *shell);

/*PARSING*/

/*Expand*/
char					*handle_dollar(t_shell *shell, char *str, int *i);
char					*expand_variables(t_shell *shell, char *str);
void					expand_all_tokens(t_shell *shell);
char					*append_normal_char(char *result, char *str, int *i,
							t_shell *shell);
char					*join_expanded_var(t_shell *shell, char *result,
							char *str, int *i);

/*Join quote*/
int						find_space(t_token *current, int i);
t_token					*change_after(t_shell *shell, t_token *current,
							int end);
int						join_check(t_token *current, int len);
void					join_quote(t_shell *shell);

/*Quote*/
int						is_closed(t_token *current, int i, char c);
int						is_empty(t_token *current, int start);
void					empty_quote_before(t_token *current, t_token *new,
							t_quote qte, t_shell *shell);
void					before_quote(t_token *current, int start,
							t_shell *shell);
void					after_mult_quote(t_token *current, int end,
							t_shell *shell);
void					after_quote(t_token *current, t_token *new, int end,
							t_shell *shell);
int						handling_quotes(t_shell *shell);

/*Parser*/
int						check_for_more_words(char *str, int i);
int						is_delimiter(char c);
int						is_special_operator(char *str, int i);
int						skip_whitespace(char *str, int i);
int						find_word_limit(char *str, int *start);
void					ft_split_word(t_shell *shell);

/*Syntax*/
int						syntax_hub(t_shell *shell);

/*Tokenizer*/
void					get_type(t_shell *shell);
void					insert_new_token(t_quote qte, t_token *new_token,
							t_token *current);
void					second_token(t_shell *shell);

/*SIGNALS*/

/*Sigint*/
void					handle_sigint_status(t_shell *shell);
void					enable_echoctl(void);
void					disable_echoctl(void);

/*Utils*/
void					signal_block(void);
void					handle_sigquit_cmd(int sig);
void					handle_sigint_cmd(int sig);
void					init_signals_cmd(void);

/*Signals*/
void					handle_sigint(int sig);
void					handle_sigint_heredoc(int sig);
void					init_signals(void);
void					init_signals_child(void);
void					init_signals_heredoc(void);

/*MAIN*/
void					initiate_all(t_shell *shell);
void					ft_hub_parsing(t_shell *shell, char *line);
void					shell_loop(t_shell *shell);
void					ft_free_node(t_shell *shell);

/*UTILS*/

/*Free all*/
char					*safe_strjoin(char *s1, char *s2, t_shell *shell,
							int free_s1);
char					*safe_strdup(char *s, t_shell *shell);
void					free_all(t_shell *shell, int status);

/*Env var*/
char					**add_env_var(char **env, char *new_var, t_shell *shell);
int						find_env_var(char **env, char *var);
char					*ft_get_env(char **env, char *var_name);
int						update_env_var(t_shell *shell, char *var_name,
							char *var_value);

#endif