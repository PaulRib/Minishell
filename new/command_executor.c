/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_executor.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 16:42:12 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/20 15:08:37 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	get_global_cmd_idx(t_shell *shell, int target_proc_i, int cmd_in_target_proc_i)
{
	int	global_idx;
	int	i;

	global_idx = 0;
	i = 0;
	while (i < target_proc_i)
	{
		global_idx += shell->exec->nb_cmd[i];
		i++;
	}
	global_idx += cmd_in_target_proc_i;
	return (global_idx);
}

// Check if command exists in the given path
static char	*check_command_in_paths(char *cmd_name, char **paths)
{
	int		i;
	char	*full_path;
	char	*temp_join;

	i = 0;
	while (paths[i])
	{
		temp_join = ft_strjoin(paths[i], "/");
		if (!temp_join)
		{
			free_tab(paths);
			return (ft_strdup(cmd_name));
		}
		full_path = ft_strjoin(temp_join, cmd_name);
		free(temp_join);
		if (!full_path)
		{
			free_tab(paths);
			return (ft_strdup(cmd_name));
		}
		if (access(full_path, X_OK) == 0)
		{
			free_tab(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	return (NULL);
}

// Get the path for a command
char	*get_command_path(char *cmd_name, char **envp)
{
	int		i;
	char	**paths;
	char	*result;

	if (!cmd_name)
		return (NULL);
	if (access(cmd_name, X_OK) == 0)
		return (ft_strdup(cmd_name));
	i = 0;
	while (envp && envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp || !envp[i])
		return (ft_strdup(cmd_name));
	paths = ft_split(envp[i] + 5, ':');
	if (!paths)
		return (ft_strdup(cmd_name));
	result = check_command_in_paths(cmd_name, paths);
	if (result)
		return (result);
	free_tab(paths);
	return (ft_strdup(cmd_name));
}

// Execute command with execve
void	execute_command(t_shell *shell, char *full_cmd_str)
{
	char	**exec_args;
	char	*cmd_path;

	if (!full_cmd_str || !full_cmd_str[0])
	{
		g_exit_status = 0;
		return;
	}
	exec_args = ft_split(full_cmd_str, ' ');
	if (!exec_args || !exec_args[0])
	{
		free_tab(exec_args);
		g_exit_status = 127;
		return;
	}
	cmd_path = get_command_path(exec_args[0], shell->data->new_env);
	if (!cmd_path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(exec_args[0], 2);
		ft_putstr_fd(": command not found (path error)\n", 2);
		free_tab(exec_args);
		exit(127);
	}
	if (execve(cmd_path, exec_args, shell->data->new_env) == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(exec_args[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		free(cmd_path);
		free_tab(exec_args);
		exit(127);
	}
}

void	execute_commands_sequence_child_v2(t_shell *shell)
{
	int	proc_i;

	proc_i = 0;
	while (proc_i < shell->exec->process)
	{
		execute_pipeline_v2(shell, proc_i);
		proc_i++;
	}
}
