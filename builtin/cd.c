/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:03:35 by pribolzi          #+#    #+#             */
/*   Updated: 2025/04/04 17:33:16 by meel-war         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	validate_tokens(t_token *token_ptr)
{
	if (!token_ptr->next)
		return (0);
	if (token_ptr->next->type != WORD)
	{
		ft_putstr_fd("syntax error near unexpected token 'newline'\n", 2);
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

static int	handle_home_directory(t_data *data, char *home_dir)
{
	if (acces(home_dir, F_OK) != 0)
	{
		ft_putstr_fd("cd: no such file or directory", 2);
		ft_putstr_fd(home_dir, 2);
		ft_putstr_fd("\n", 2);
		return (1);
	}
	if (chdir(home_dir) != 0)
	{
		ft_putstr_fd("cd: permission denied: ", 2);
		ft_putstr_fd(home_dir, 2);
		ft_putstr_fd("\n", 2);
		return (1);
	}
	return (0);
}

// Peut etre joint en une fonction probablement pour gerer le cas ou ont doit acceder
// a home ou path

static int	handle_path_directory(char *path_dir)
{
	if (acces(path_dir, F_OK) != 0)
	{
		ft_putstr_fd("cd: no such file or directory", 2);
		ft_putstr_fd(path_dir, 2);
		ft_putstr_fd("\n", 2);
		return (1);
	}
	if (chdir(path_dir) != 0)
	{
		ft_putstr_fd("cd: permission denied: ", 2);
		ft_putstr_fd(path_dir, 2);
		ft_putstr_fd("\n", 2);
		return (1);
	}
	return (0);
}

int	ft_cd(t_data *data, char *path_name)
{
	char *home_dir;
	int result;
	
	if (!path_name)
	{
		home_dir = ft_get_env(data->new_env, "HOME");
		if (!home_dir)
		{
			ft_putstr_fd("cd: HOME not set\n", 2);
			return (1);
		}
		result = handle_home_directory(data, home_dir);
	}
	else
		result = handle_path_directory(path_name);
	if(result != 0)
		return(result);
	if(getcwd(data->cur_dir, PATH_MAX) != NULL)
	{
		update_env_var(data, "OLDPWD", data->old_dir);
		update_env_var(data, "PWD", data->cur_dir);
	}
	return(0);
}
