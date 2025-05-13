/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_executor.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 17:43:10 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/13 18:06:16 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Forward declarations for functions that might be in other files or later in this one
void	execute_single_command_child_logic(t_shell *shell);
void	execute_pipeline_child_logic(t_shell *shell);
char	*get_command_path(char *cmd_name, char **envp); // formerly get_path
void	execute_external_or_builtin(char *full_cmd_str, t_shell *shell); // formerly execute

// Main entry point for command execution within the global child process
// Called from run_global_child_process in orchestrator.c after heredocs
void	execute_commands_sequence_child(t_shell *shell)
{
	int	infile_status;

	infile_status = 0;
	if (shell->count->nb_redir_in > 0)
		infile_status = open_infile(shell); // from exec_fd.c

	if (infile_status == -1) // Error opening infile, e.g., no such file
	{
		g_exit_status = 1; // As per bash behavior for redirection error
		return; // Exit this function, global child will then exit with this status
	}
	if (shell->count->nb_redir_out > 0 || shell->count->nb_append > 0)
		open_outfile(shell); // from exec_fd.c

	if (shell->count->nb_pipe == 0)
		execute_single_command_child_logic(shell);
	else
		execute_pipeline_child_logic(shell);
	// Note: g_exit_status is set by the actual command execution functions
}

// Handles a single command execution (no pipes)
void	execute_single_command_child_logic(t_shell *shell)
{
	char	*cmd_str;

	// If it's a builtin that can run in a forked process (or any builtin if forked)
	if (check_single_builtin(shell) == 1) // from exec_fd.c
	{
		if (shell->exec->fd_in[0] != STDIN_FILENO)
			dup2(shell->exec->fd_in[0], STDIN_FILENO);
		if (shell->exec->fd_out[0] != STDOUT_FILENO)
			dup2(shell->exec->fd_out[0], STDOUT_FILENO);
		// close unused fds from open_infile/outfile if they were opened
		if (shell->exec->fd_in[0] > STDERR_FILENO)
			close(shell->exec->fd_in[0]);
		if (shell->exec->fd_out[0] > STDERR_FILENO)
			close(shell->exec->fd_out[0]);
		is_builtin(shell); // from builtin.c, sets g_exit_status
	}
	else // External command or builtin that needs forking if it's part of a more complex setup (already forked here)
	{
		cmd_str = give_curr_cmd(shell, 0); // from exec.c (needs to be available)
		// This function will handle redirections and execve
		execute_external_or_builtin(cmd_str, shell); 
		free(cmd_str);
	}
}

void execute_pipeline_child_logic(t_shell *shell)
{
    execute_pipeline(shell); // Appel de la fonction qui gère réellement les pipelines
}


// This function will be adapted from your `execute` function in exec.c
// It's called for single external commands or builtins that are forked.
void	execute_external_or_builtin(char *full_cmd_str, t_shell *shell)
{
	char	**exec_args;
	char	*cmd_path;

	if (!full_cmd_str || !full_cmd_str[0])
	{
		g_exit_status = 0; // Bash exits 0 for empty command `""`
		return;
	}
	exec_args = ft_split(full_cmd_str, ' ');
	if (!exec_args || !exec_args[0])
	{
		free_tab(exec_args);
		g_exit_status = 0; // Or 127 if ft_split fails badly? Bash: `$: command not found` (127)
		return;
	}
	// Handle redirections for the single command
	if (shell->exec->fd_in[0] != STDIN_FILENO)
		dup2(shell->exec->fd_in[0], STDIN_FILENO);
	if (shell->exec->fd_out[0] != STDOUT_FILENO)
		dup2(shell->exec->fd_out[0], STDOUT_FILENO);
	// Close original fds if they were opened and duplicated
	if (shell->exec->fd_in[0] > STDERR_FILENO)
		close(shell->exec->fd_in[0]);
	if (shell->exec->fd_out[0] > STDERR_FILENO)
		close(shell->exec->fd_out[0]);
	
	// Check if it's a builtin that should run in this forked context
	// This check might be more complex, or is_builtin could be adapted
	// For now, assume if not caught by check_single_builtin earlier, it's external or needs execve path

	cmd_path = get_command_path(exec_args[0], shell->data->new_env);
	if (execve(cmd_path, exec_args, shell->data->new_env) == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(exec_args[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		free_tab(exec_args);
		if (cmd_path != exec_args[0]) // if get_command_path allocated new memory
			free(cmd_path);
		exit(127); // Command not found, exit child process
	}
	// execve only returns on error, so child exits above if error.
	// This part should not be reached if execve is successful.
}

// This function will be adapted from your `get_path` function in exec.c
char	*get_command_path(char *cmd_name, char **envp)
{
	int		i;
	char	**paths;
	char	*full_path;
	char	*temp_join;

	if (!cmd_name || access(cmd_name, X_OK) == 0)
		return (cmd_name); // Can be NULL if cmd_name is NULL, or cmd_name itself
	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
		return (cmd_name); // No PATH variable, return original name
	paths = ft_split(envp[i] + 5, ':');
	if (!paths)
		return (cmd_name); // Malloc failure in ft_split
	i = 0;
	while (paths[i])
	{
		temp_join = ft_strjoin(paths[i], "/");
		if (!temp_join) // Malloc failure
			break; // continue to free paths and return cmd_name
		full_path = ft_strjoin(temp_join, cmd_name);
		free(temp_join);
		if (!full_path) // Malloc failure
			break; // continue to free paths and return cmd_name
		if (access(full_path, X_OK) == 0)
		{
			free_tab(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_tab(paths);
	return (cmd_name); // Return original name if not found in PATH
}

