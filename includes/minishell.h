/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 15:03:45 by pribolzi          #+#    #+#             */
/*   Updated: 2025/03/05 15:36:31 by pribolzi         ###   ########.fr       */
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

# define WORD 1
# define PIPE 2
# define REDIR_IN 3
# define REDIR_OUT 4
# define APPEND 5
# define HEREDOC 6
# define END 7
// int	g_exit_status;

typedef struct s_data
{
	char	*cur_dir;
	char	**new_env;
	int		shlvl;
}			t_data;

char	**copy_env(char **env, t_data data);
char	**build_env(t_data data);

typedef struct s_list
{
	char	*str;
	t_data	*data;
	t_token	*token;
}					t_list;

typedef struct s_token
{
	int				*type;
	char			**str;
}				t_token;

// typedef struct s_pipex
// {
// 	int	p_fd[2];
// 	int	fd_in;
// 	int	fd_out;
// }				t_pipex;

#endif