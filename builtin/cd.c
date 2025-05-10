/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:03:35 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/09 16:44:54 by meel-war         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	validate_tokens(t_token *token_ptr)
{
	if (!token_ptr->next)
		return (0);
	if (token_ptr->next->type != WORD)
	{
		ft_putstr_fd("bash: syntax error near unexpected token 'newline'\n", 2);
		return (1);
	}
	else if (token_ptr->next && token_ptr->next->next)
	{
		ft_putstr_fd("bash: cd: too many arguments\n", 2);
		return (1);
	}
	return (0);
}

int	check_cd(t_shell *shell, t_token *token_ptr)
{
	int	is_valid;

	if (ft_strcmp(token_ptr->str, "cd") != 0)
		return (-1);
	is_valid = validate_tokens(token_ptr);
	if (is_valid != 0)
		return (is_valid);
	if (token_ptr->next)
		return (ft_cd(shell->data, token_ptr->next->str));
	else
		return (ft_cd(shell->data, NULL));
}

static int	update_cur_dir(t_data *data, char *path_name, char *old_dir)
{
	char	real_path[PATH_MAX];

	if (path_name && path_name[0] == '/')
		ft_strlcpy(data->cur_dir, path_name, PATH_MAX);
	else if (path_name && !ft_strcmp(path_name, "-"))
		ft_strlcpy(data->cur_dir, old_dir, PATH_MAX);
	else if (!path_name)
		ft_strlcpy(data->cur_dir, old_dir, PATH_MAX);
	else
	{
		if (getcwd(real_path, PATH_MAX) == NULL)
		{
			ft_putstr_fd("bash: cd: getcwd error\n", 2);
			return (1);
		}
		ft_strlcpy(data->cur_dir, real_path, PATH_MAX);
	}
	return (0);
}

int	execute_cd(t_data *data, char *path_name, char *home_dir, char *old_dir)
{
	int	result;

	if (!path_name)
	{
		if (!home_dir)
		{
			ft_putstr_fd("bash: cd: HOME not set\n", 2);
			return (1);
		}
		result = handle_directory(home_dir, NULL, NULL);
	}
	else
		result = handle_directory(path_name, home_dir, old_dir);
	if (result != 0)
		return (result);
	ft_strlcpy(data->old_dir, data->cur_dir, PATH_MAX);
	if (update_cur_dir(data, path_name, old_dir) != 0)
		return (1);
	update_env_var(data, "OLDPWD", data->old_dir);
	update_env_var(data, "PWD", data->cur_dir);
	return (0);
}

int	ft_cd(t_data *data, char *path_name)
{
	char	*home_dir;
	char	*old_dir;

	home_dir = ft_get_env(data->new_env, "HOME");
	old_dir = ft_get_env(data->new_env, "OLDPWD");
	if (getcwd(data->old_dir, PATH_MAX) == NULL)
	{
		ft_putstr_fd("bash: cd: getcwd error\n", 2);
		return (1);
	}
	return (execute_cd(data, path_name, home_dir, old_dir));
}
