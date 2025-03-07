/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:03:35 by pribolzi          #+#    #+#             */
/*   Updated: 2025/03/07 16:13:03 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	cd_handling(t_list *hub)
{
	if (ft_strncmp(hub->str, "~/", 2) == 0)
	{
		erase_all(hub);
		hub->data->cur_dir = malloc(sizeof(char) * (ft_strlen(hub->str)));
		hub->data->cur_dir = ft_strjoin(hub->data->cur_dir, hub->str);
	}
	if (ft_strncmp(hub->str, "../", 3) == 0)
	{
		going_back(hub);
		hub->data->cur_dir = ft_strjoin(hub->data->cur_dir, hub->str);
	}
	else if (ft_strncmp(hub->str, "..", 2) == 0)
		going_back(hub)
	else
		hub->data->cur_dir = ft_strjoin(hub->data->cur_dir, hub->str);
	chdir(hub->str);
}

void	going_back(t_list *hub)
{
	int		i;

	i = 0;
	while (hub->data->cur_dir[i])
		i++;
	while (hub->data->cur_dir[i] != "/")
		i--;
	hub->data->cur_dir[i] = '\0';
}

void	erase_all(t_list *hub)
{
	free(hub->data->cur_dir);
	hub->data->cur_dir[0] = '\0';
}
/*
int	cd_handling(t_list *hub, char *path)
{
	char	*new_dir;
	char	*old_dir;

	old_dir = ft_strdup(hub->data->cur_dir);
	if (!path || !*path || ft_strcmp(path, "~") == 0)
	{
		// Handle cd to HOME directory
		char *home = getenv("HOME");
		if (!home)
			return (print_error("cd: HOME not set", 1));
		chdir(home);
		free(hub->data->cur_dir);
		hub->data->cur_dir = ft_strdup(home);
	}
	else if (ft_strcmp(path, "-") == 0)
	{
		// Handle cd to previous directory (OLDPWD)
		char *oldpwd = getenv("OLDPWD");
		if (!oldpwd)
			return (print_error("cd: OLDPWD not set", 1));
		chdir(oldpwd);
		printf("%s\n", oldpwd);
		free(hub->data->cur_dir);
		hub->data->cur_dir = ft_strdup(oldpwd);
	}
	else if (path[0] == '/')
	{
		// Handle absolute path
		if (chdir(path) != 0)
			return (print_error("cd: No such file or directory", 1));
		free(hub->data->cur_dir);
		hub->data->cur_dir = ft_strdup(path);
	}
	else if (ft_strncmp(path, "~/", 2) == 0 || ft_strcmp(path, "~") == 0)
	{
		// Handle path with tilde
		char *home = getenv("HOME");
		if (!home)
			return (print_error("cd: HOME not set", 1));
		
		new_dir = ft_strjoin(home, path + 1);
		if (chdir(new_dir) != 0)
		{
			free(new_dir);
			return (print_error("cd: No such file or directory", 1));
		}
		free(hub->data->cur_dir);
		hub->data->cur_dir = new_dir;
	}
	else
	{
		// Handle relative path
		new_dir = ft_strjoin(hub->data->cur_dir, "/");
		char *temp = new_dir;
		new_dir = ft_strjoin(new_dir, path);
		free(temp);
		
		if (chdir(path) != 0)
		{
			free(new_dir);
			return (print_error("cd: No such file or directory", 1));
		}
		free(hub->data->cur_dir);
		hub->data->cur_dir = ft_strdup(path);
		free(new_dir);
	}
	
	// Update PWD and OLDPWD environment variables
	update_env(hub, "OLDPWD", old_dir);
	update_env(hub, "PWD", hub->data->cur_dir);
	free(old_dir);
	
	return (0);
}

/* Helper function to update environment variables */
void update_env(t_list *hub, char *var, char *value)
{
	int i = 0;
	char *new_var;
	
	// Format: VAR=value
	new_var = malloc(ft_strlen(var) + ft_strlen(value) + 2);
	ft_strcpy(new_var, var);
	ft_strcat(new_var, "=");
	ft_strcat(new_var, value);
	
	// Find if variable already exists
	while (hub->data->new_env[i])
	{
		if (ft_strncmp(hub->data->new_env[i], var, ft_strlen(var)) == 0 && 
			hub->data->new_env[i][ft_strlen(var)] == '=')
		{
			free(hub->data->new_env[i]);
			hub->data->new_env[i] = new_var;
			return;
		}
		i++;
	}
	
	// If not found, add it to the environment
	// This requires resizing the environment array, which isn't shown here
	// You should implement a function to extend the environment array
}

/* Helper function to print errors */
int print_error(char *message, int return_code)
{
	ft_putstr_fd(message, 2);
	ft_putstr_fd("\n", 2);
	return (return_code);
}
*/