/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 15:03:45 by pribolzi          #+#    #+#             */
/*   Updated: 2025/03/25 15:14:08 by meel-war         ###   ########.fr       */
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

# define WORD 1
# define PIPE 2
# define REDIR_IN 3
# define REDIR_OUT 4
# define APPEND 5
# define HEREDOC 6
# define END 7
# define FILE_IN 8
# define FILE_OUT 9
# define ENDOF 10
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
void	add_to_history(t_shell *shell, char *command);
int		unset_handling(t_shell *shell);

#endif