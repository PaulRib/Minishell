/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:01:22 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/29 15:39:14 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_cmd_a_builtin(t_shell *shell, t_pipe *pipe)
{
	int		i;
	t_token	*current;

	current = shell->token;
	i = pipe->global_idx;
	while (current && i)
	{
		if (current->type == PIPE)
			i--;
		current = current->next;
	}
	if (is_target_builtin(shell, current) > 0)
		return (1);
	return (0);
}

char	**give_curr_cmd(t_shell *shell, int i)
{
	t_token	*current;
	int		count;
	t_token	*tmp;

	count = 0;
	current = shell->token;
	while (current && i)
	{
		if (current->type == PIPE)
			i--;
		current = current->next;
	}
	tmp = current;
	while (tmp && tmp->type != PIPE)
	{
		count++;
		tmp = tmp->next;
	}
	return (extract_cmd(shell, current, count));
}

int	get_global_cmd_idx(t_shell *shell, int target_proc_i,
		int cmd_in_target_proc_i)
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

char	*get_path(char *cmd, char **envp, t_shell *shell)
{
	int		i;
	char	**path;

	i = 0;
	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	while (envp[i] && ft_strncmp(envp[i], "PATH", 4) != 0)
		i++;
	if (!envp[i])
		return (NULL);
	else
	{
		path = ft_split(envp[i] + 5, ':');
		return (path_util(shell, path, cmd));
	}
}

void	execute_command(t_shell *shell, char **exec_args)
{
	char	*cmd_path;

	cmd_path = get_path(exec_args[0], shell->data->new_env, shell);
	if (!cmd_path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(exec_args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
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
