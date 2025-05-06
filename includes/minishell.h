/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 15:03:45 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/06 17:40:52 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <sys/types.h>
# include <sys/wait.h>
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
	int 				process;
	struct s_heredoc	*next;
}				t_heredoc;

typedef struct s_exec
{
	int		*nb_cmd;
	int		p_fd[2];
	int		*fd_in;
	int		*fd_out;
	int		process;
	int		*prev_fd;
}				t_exec;

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

/* Environnement */
char					**copy_env(char **env, t_data *data);
char					**build_env(t_data *data);
int						print_env(t_shell *hub);
char					*ft_get_env(char **env, char *var_name);
int						find_env_var(char **env, char *var);
char					**add_env_var(char **env, char *new_var);
int						update_env_var(t_data *data, char *var_name,
							char *var_value);

/* Tokenisation */
void					get_type(t_shell *shell);
void					new_node_token(t_token *ptr);
void					ft_split_word(t_shell *shell);
void					second_token(t_shell *shell);

/* Parsing */
void					ft_minisplit(char *line, t_shell *shell);
void					ft_hub_parsing(t_shell *shell, char *line);
void					associate_options_commands(t_shell *shell);
void					handling_quotes(t_shell *shell);
int						is_empty(t_token *current, int start);
int						is_closed(t_token *current, int i, char c);
void					empty_quote_before(t_token *current, t_token *new,
							int end, char c);
void					after_quote(t_token *current, t_token *new, int end);
void					before_quote(t_token *current, int start);
void					after_mult_quote(t_token *current, int end);
int						check_for_more_words(char *str, int i);
void					expand_all_tokens(t_shell *shell);
char					*expand_variables(t_shell *shell, char *str);
void					append_normal_char(char **result, char *str, int *i);

/* Signals*/
void					init_signals(void);

/* Builtins */
int						is_builtin(t_shell *shell, t_token *token_list);
/* export */
int						check_export(t_shell *shell, t_token *token_ptr);
int						export_no_args(t_shell *shell);
/* cd */
int						check_cd(t_shell *shell, t_token *token_ptr);
int						ft_cd(t_data *data, char *path_name);
char					*ft_handle_tilde(char *dir, char *home_dir);

/* history */
int						ft_history(t_shell *hub);
void					add_to_history(t_shell *shell, char *command);
/* unset */
int						check_unset(t_shell *shell, t_token *token_ptr);
int						ft_unset(t_data *data, char *var_name);
/* pwd */
int						ft_pwd(void);
int						check_pwd(t_shell *shell, t_token *token_ptr);
char					*ft_handle_tilde(char *dir, char *home_dir);

/*Execution*/
void					exec_hub(t_shell *shell);
void					here_doc_hub(t_shell *shell);
/* echo */
int						check_echo(t_token *token_ptr);

/* Utils */

char					**add_env_var(char **env, char *new_var);
int						find_env_var(char **env, char *var);
char					*ft_get_env(char **env, char *var_name);
int						update_env_var(t_data *data, char *var_name,
							char *var_value);

#endif