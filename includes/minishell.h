/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 15:03:45 by pribolzi          #+#    #+#             */
/*   Updated: 2025/04/04 17:32:52 by meel-war         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <stdbool.h>

# ifndef PATH_MAX
#	define PATH_MAX 4096
# endif

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
// int	g_exit_status;

typedef struct s_data
{
	char	cur_dir[PATH_MAX];
	char	old_dir[PATH_MAX];
	char	**new_env;
	int		shlvl;
}			t_data;

typedef struct s_history
{
	char				*command;
	struct s_history	*next;
}					t_history;

typedef struct s_token
{
	int				type;
	char			*str;
	struct s_token	*next;
	struct s_token	*prev;
}				t_token;

typedef	struct s_count
{
	int	nb_pipe;
	int	is_redir_in;
	int	is_redir_out;
}				t_count;

typedef struct s_shell
{
	char		*str;
	t_data		*data;
	t_token		*token;
	t_history	*history;
	t_count		*number;
}					t_shell;

// typedef struct s_pipex
// {
// 	int	p_fd[2];
// 	int	fd_in;
// 	int	fd_out;
// }				t_pipex;

/* Environnement */
char	**copy_env(char **env, t_data *data);
char	**build_env(t_data *data);
int		print_env(t_shell *hub);
char	*ft_get_env(char **env,  char *var_name);
int		find_env_var(char **env, char *var);
char	**add_env_var(char **env, char *new_var);
int		update_env_var(t_data *data, char *var_name, char *var_value);

/* Tokenisation */
void	get_type(t_shell *shell);
void	new_node_token(t_token *ptr, bool place, t_token *target);
void	ft_split_word(t_shell *shell);
void	second_token(t_shell *shell);
void	count_element(t_shell *shell, char *line);

/* Parsing */
void	ft_minisplit(char *line, t_shell *shell);
void	ft_hub_parsing(t_shell *shell, char *line);
void	associate_options_commands(t_shell *shell);

/*Builtins*/
int		is_builtin (t_shell *shell, t_token *token_list);
void	add_to_history(t_shell *shell, char *command);
int		check_unset(t_shell *shell, t_token *token_ptr);
int		ft_unset(t_data *data, char *var_name);
int		ft_pwd(void);

#endif