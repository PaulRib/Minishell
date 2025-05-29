/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd2.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:44:26 by meel-war          #+#    #+#             */
/*   Updated: 2025/05/29 19:16:03 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	check_change_dir(char *dir, t_shell *shell)
{
	if (access(dir, F_OK) != 0)
	{
		ft_putstr_fd("minishell: cd: no such file or directory ", 2);
		ft_putstr_fd(dir, 2);
		ft_putstr_fd("\n", 2);
		shell->exit_status = 1;
		free(dir);
		return (1);
	}
	if (chdir(dir) != 0)
	{
		ft_putstr_fd("minishell: cd: permission denied: ", 2);
		ft_putstr_fd(dir, 2);
		ft_putstr_fd("\n", 2);
		free(dir);
		return (1);
	}
	return (0);
}

int	handle_directory(char **dir, char *home_dir, char *old_dir, t_shell *shell)
{
	int result;

	if (!ft_strncmp(*dir, "-", 2))
	{
		*dir = ft_handle_hyphen(*dir, old_dir, shell);
		if (!*dir)
			return (1);
	}
	if (!ft_strncmp(*dir, "~", 2))
	{
		*dir = ft_handle_tilde(*dir, home_dir, shell);
		if (!*dir)
			return (1);
	}
	result = check_change_dir(*dir, shell);
	printf("PATH AFTER SHIT:%s\n", *dir);
	return (result);
}

char	*ft_handle_hyphen(char *dir, char *old_dir, t_shell *shell)
{
	if (!old_dir)
	{
		ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
		return (NULL);
	}
	free(dir);
	dir = ft_strdup(old_dir);
	if (!dir)
	{
		free(old_dir);
		free_all(shell, 1);
	}
	ft_printf("%s\n", old_dir);
	return (dir);
}

char	*ft_handle_tilde(char *dir, char *home_dir, t_shell *shell)
{
	char	*tmp;

	if (!ft_strcmp(dir, "~"))
	{
		if (home_dir)
			dir = safe_strdup(home_dir, shell);
		else
		{
			ft_putstr_fd("bash: cd: HOME not set\n", 2);
			return (NULL);
		}
	}
	else
	{
		tmp = ft_substr(dir, 1, ft_strlen(dir) - 1);
		free(dir);
		if (!tmp)
			free_all(shell, 1);
		dir = ft_strjoin(home_dir, tmp);
		free(tmp);
		if (!dir)
			free_all(shell, 1);
	}
	return (dir);
}
