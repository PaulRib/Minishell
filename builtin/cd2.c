/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd2.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:44:26 by meel-war          #+#    #+#             */
/*   Updated: 2025/05/27 18:31:31 by meel-war         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_directory(char *dir, char *home_dir, char *old_dir, t_shell *shell)
{
	if (!ft_strncmp(dir, "-", 2))
	{
		dir = ft_handle_hyphen(dir, old_dir);
		if (!dir)
			return (1);
	}
	if (!ft_strncmp(dir, "~", 1))
		dir = ft_handle_tilde(dir, home_dir);
	if (access(dir, F_OK) != 0)
	{
		ft_putstr_fd("minishell: cd: no such file or directory ", 2);
		ft_putstr_fd(dir, 2);
		ft_putstr_fd("\n", 2);
		shell->exit_status = 1;
		return (1);
	}
	if (chdir(dir) != 0)
	{
		ft_putstr_fd("minishell: cd: permission denied: ", 2);
		ft_putstr_fd(dir, 2);
		ft_putstr_fd("\n", 2);
		return (1);
	}
	return (0);
}

char	*ft_handle_hyphen(char *dir, char *old_dir)
{
	if (!old_dir)
	{
		ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
		free(dir);
		return (NULL);
	}
	free(dir);
	dir = ft_strdup(old_dir);
	ft_printf("%s\n", old_dir);
	return (dir);
}

char	*ft_handle_tilde(char *dir, char *home_dir)
{
	char	*tmp;

	if (!ft_strcmp(dir, "~"))
	{
		free(dir);
		dir = ft_strdup(home_dir);
	}
	else
	{
		tmp = ft_substr(dir, 1, ft_strlen(dir) - 1);
		free(dir);
		dir = ft_strjoin(home_dir, tmp);
		free(tmp);
	}
	return (dir);
}
