/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_message.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 18:08:27 by meel-war          #+#    #+#             */
/*   Updated: 2025/06/04 13:49:32 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	print_access_error(t_token *current)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(current->str, 2);
	ft_putstr_fd(": Permission denied\n", 2);
	return (1);
}

void	print_directory_error(t_token *current)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(current->str, 2);
	ft_putstr_fd(": Is a directory\n", 2);
}

int	verify_access_fd(t_token *current, int type)
{
	struct stat	info;

	if (access(current->str, F_OK) == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(current->str, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (1);
	}
	if (stat(current->str, &info) == 0)
		if (S_ISDIR(info.st_mode))
		{
			print_directory_error(current);
			return (1);
		}
	if (type == 0)
		if (access(current->str, R_OK) == -1)
			return (print_access_error(current));
	if (type == 1)
		if (access(current->str, W_OK) == -1)
			return (print_access_error(current));
	return (1);
}
