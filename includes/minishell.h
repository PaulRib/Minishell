/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 15:03:45 by pribolzi          #+#    #+#             */
/*   Updated: 2025/04/08 17:55:35 by pribolzi         ###   ########.fr       */
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
// int	g_exit_status;

typedef struct s_data
{
	char	*cur_dir;
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

typedef struct s_count
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

/* Tokenisation */
void	get_type(t_shell *shell);
void	new_node_token(t_token *ptr);
void	ft_split_word(t_shell *shell);
void	second_token(t_shell *shell);
void	count_element(t_shell *shell, char *line);

/* Parsing */
void	ft_minisplit(char *line, t_shell *shell);
void	ft_hub_parsing(t_shell *shell, char *line);
void	associate_options_commands(t_shell *shell);
void	handling_quotes(t_shell *shell);
int		is_empty(t_token *current, int start);
int		is_space(t_token *after);
int		is_closed(t_token *current, int i, char c);
void	empty_quote_before(t_token *current, t_token *new, int end, char c);
void	after_quote(t_token *current, t_token *new, int end);
void	before_quote(t_token *current, int start);
void	after_mult_quote(t_token *current, int end);

/*Builtins*/
void	add_to_history(t_shell *shell, char *command);

#endif