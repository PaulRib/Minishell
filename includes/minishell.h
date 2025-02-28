/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 15:03:45 by pribolzi          #+#    #+#             */
/*   Updated: 2025/02/28 14:21:30 by meel-war         ###   ########.fr       */
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
	char *cur_dir;
	char **new_env;
}			t_data;

typedef struct s_list
{
	char	*str;
	t_data	*next;
	t_data	*prev;
	t_token	*token;
}					t_list;

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