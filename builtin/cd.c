/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:03:35 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/30 15:55:57 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	validate_tokens(t_shell *shell, t_token *token_ptr)
{
	if (!token_ptr->next)
		return (0);
	if (token_ptr->next->type != WORD && token_ptr->next->type != S_QUOTE
		&& token_ptr->next->type != D_QUOTE && token_ptr->next->type != PIPE)
	{
		ft_putstr_fd("bash: syntax error near unexpected token 'newline'\n", 2);
		return (1);
	}
	else if (token_ptr->next && token_ptr->next->next
		&& check_many_args(token_ptr->next) == 1)
	{
		ft_putstr_fd("bash: cd: too many arguments\n", 2);
		shell->exit_status = 1;
		return (1);
	}
	return (0);
}

int	check_cd(t_shell *shell, t_token *token_ptr)
{
	int		is_valid;
	char	*str;

	if (ft_strcmp(token_ptr->str, "cd") != 0)
		return (-1);
	is_valid = validate_tokens(shell, token_ptr);
	if (is_valid != 0)
		return (is_valid);
	if (token_ptr->next && token_ptr->next->type != PIPE)
	{
		str = ft_strdup(token_ptr->next->str);
		return (ft_cd(shell, str));
	}
	else
		return (ft_cd(shell, NULL));
}

static int	update_cur_dir(t_data *data, char *path_name, char *old_dir)
{
	char	real_path[PATH_MAX];

	if (path_name && path_name[0] == '/')
		ft_strlcpy(data->cur_dir, path_name, PATH_MAX);
	else if (path_name && !ft_strcmp(path_name, "-"))
		ft_strlcpy(data->cur_dir, old_dir, PATH_MAX);
	else if (!path_name)
	{
		return (0);
	}
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

int	execute_cd(t_shell *shell, char *path_name, char *home_dir, char *old_dir)
{
	int	result;

	if (!path_name)
	{
		if (!home_dir)
		{
			ft_putstr_fd("bash: cd: HOME not set\n", 2);
			shell->exit_status = 1;
			return (1);
		}
		result = handle_directory(&home_dir, NULL, NULL, shell);
		path_name = ft_strdup(home_dir);
	}
	else
		result = handle_directory(&path_name, home_dir, old_dir, shell);
	if (result != 0)
		return (result);
	ft_strlcpy(shell->data->old_dir, shell->data->cur_dir, PATH_MAX);
	if (update_cur_dir(shell->data, path_name, old_dir) != 0)
		return (free(path_name), -1);
	if (ft_get_env(shell->data->new_env, "OLDPWD"))
		update_env_var(shell, "OLDPWD", shell->data->old_dir);
	if (ft_get_env(shell->data->new_env, "PWD"))
		update_env_var(shell, "PWD", shell->data->cur_dir);
	return ((free(path_name), 1));
}

int	ft_cd(t_shell *shell, char *path_name)
{
	char	*home_dir;
	char	*old_dir;

	home_dir = ft_get_env(shell->data->new_env, "HOME");
	old_dir = ft_get_env(shell->data->new_env, "OLDPWD");
	return (execute_cd(shell, path_name, home_dir, old_dir));
}
