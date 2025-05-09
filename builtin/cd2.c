/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd2.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 17:44:26 by meel-war          #+#    #+#             */
/*   Updated: 2025/05/07 17:45:12 by meel-war         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
