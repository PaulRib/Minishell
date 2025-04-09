/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:03:35 by pribolzi          #+#    #+#             */
/*   Updated: 2025/04/09 18:04:39 by meel-war         ###   ########.fr       */
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
	else if (token_ptr->next && token_ptr->next->next)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
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

static int	handle_directory(char *dir, char *home_dir)
{
	// if(!ft_strncmp(dir, "-", 2))
	// {
	// 	ft_handle_hyphen();
	// }
	if(!ft_strncmp(dir, "~", 1))
	{
		dir = ft_handle_tilde(dir, home_dir);
	}
	if (access(dir, F_OK) != 0)
	{
		ft_putstr_fd("minishell: cd: no such file or directory", 2);
		ft_putstr_fd(dir, 2);
		ft_putstr_fd("\n", 2);
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

char *ft_handle_tilde(char *dir, char *home_dir)
{
	char *tmp;
	
	if(!ft_strcmp(dir, "~"))
	{
		free(dir);
		dir = ft_strdup(home_dir);
	}
	else
	{
		tmp = ft_substr(dir, 1, ft_strlen(dir) - 1);
		free (dir);
		dir = ft_strjoin(home_dir, tmp);
		free(tmp);
	}
	return(dir);
}

int	ft_cd(t_data *data, char *path_name)
{
	char *home_dir;
	int result;
	
	home_dir = ft_get_env(data->new_env, "HOME");
	if (!path_name)
	{
		if (!home_dir)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
			return (1);
		}
		result = handle_directory(home_dir, NULL);
	}
	else
		result = handle_directory(path_name, home_dir);
	if(result != 0)
		return(result);
	if(getcwd(data->cur_dir, PATH_MAX) != NULL)
	{
		update_env_var(data, "OLDPWD", data->old_dir);
		update_env_var(data, "PWD", data->cur_dir);
	}
	return(0);
}
