/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 15:03:45 by pribolzi          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/02/27 15:01:01 by pribolzi         ###   ########.fr       */
=======
/*   Updated: 2025/02/28 12:43:40 by meel-war         ###   ########.fr       */
>>>>>>> main
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

int	g_exit_status;

typedef struct s_data
{
<<<<<<< HEAD
	char	*cur_dir;
=======
	char *cur_dir;
	char **new_env;
>>>>>>> main
}			t_data;

typedef struct s_list
{
	char	*str;
<<<<<<< HEAD
	t_list	*next;
	t_list	*prev;
	t_env	*env;
=======
	t_data	*next;
	t_data	*prev;
>>>>>>> main
	t_token	*token;
	t_data	*dir;
}					t_list;

<<<<<<< HEAD
typedef struct s_env
{
	char	*str;
	t_env	*next;
	t_env	*prev;
}				t_env;

=======
>>>>>>> main
typedef struct s_token
{
	int				token;
	char			*str;
	struct s_token	*next;
}				t_token;

typedef struct s_pipex
{
	int	p_fd[2];
	int	fd_in;
	int	fd_out;
}				t_pipex;

#endif