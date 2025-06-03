/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_message.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 18:08:27 by meel-war          #+#    #+#             */
/*   Updated: 2025/06/03 15:29:34 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_access_error(t_token *current)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(current->str, 2);
	ft_putstr_fd(": Permission denied\n", 2);
}

void	print_directory_error(t_token *current)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(current->str, 2);
	ft_putstr_fd(": Is a directory\n", 2);
}

int	verify_access_fd(t_token *current)
{
	struct stat	info;

	if (stat(current->str, &info) == 0)
		if (S_ISDIR(info.st_mode))
		{
			print_directory_error(current);
			return (1);
		}
	if (access(current->str, R_OK) == -1)
	{
		print_access_error(current);
		return (1);
	}
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(current->str, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
	return (1);
}
