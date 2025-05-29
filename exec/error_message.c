/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_message.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 18:08:27 by meel-war          #+#    #+#             */
/*   Updated: 2025/05/29 18:52:24 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	infile_warning_msg(t_shell *shell, char *str)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
	free_all(shell, 1);
}

void	print_heredoc_eof_warning_msg(char *delimiter)
{
	ft_putstr_fd("minishell: warning: ", 2);
	ft_putstr_fd("here-document delimited by end-of-file (wanted `", 2);
	if (delimiter)
		ft_putstr_fd(delimiter, 2);
	else
		ft_putstr_fd("unknown", 2);
	ft_putstr_fd("`)\n", 2);
}

void print_access_error(t_shell *shell, t_token *current)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(current->str, 2);
	ft_putstr_fd(": Permission denied\n", 2);
	free_all(shell, 1);
}

void print_directory_error(t_shell *shell, t_token *current)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(current->str, 2);
	ft_putstr_fd(": Is a directory\n", 2);
	free_all(shell, 1);
}

void	verify_access_fd(t_shell *shell)
{
	t_token *current;
	struct stat info;

	current = shell->token;
	while (current)
	{
		if (current->type == FILE_IN || current->type == FILE_OUT)
		{
			if (access(current->str, R_OK) == -1)
				print_access_error(shell, current);
			if (stat(current->str, &info) == 0)
				if (S_ISDIR(info.st_mode))
					print_directory_error(shell, current);
		}
		current = current->next;
	}
}
