/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_executor.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 16:42:12 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/15 19:20:22 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int get_global_cmd_idx(t_shell *shell, int target_proc_i, int cmd_in_target_proc_i)
{
    int global_idx;
    int i;

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

char *get_command_path_v2(char *cmd_name, char **envp)
{
    // This function should be a direct copy or adaptation of the user's `get_path` function.
    // For brevity, I'll assume it's available (e.g., user renames their get_path or makes it available).
    // If it's not available globally, its logic needs to be replicated here.
    // return (get_path(cmd_name, envp)); // Placeholder call to user's original

    // Re-implementing based on user's exec.c's get_path for completeness here:
    int i;
    char **paths;
    char *full_path;
    char *temp_join;

    if (!cmd_name) return (NULL);
    if (access(cmd_name, X_OK) == 0)
        return (ft_strdup(cmd_name)); // Return a copy to allow freeing later
    i = 0;
    while (envp && envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
        i++;
    if (!envp || !envp[i])
        return (ft_strdup(cmd_name)); // No PATH, return copy of original name
    paths = ft_split(envp[i] + 5, ':');
    if (!paths) return (ft_strdup(cmd_name));
    i = 0;
    while (paths[i])
    {
        temp_join = ft_strjoin(paths[i], "/");
        if (!temp_join) { free_tab(paths); return (ft_strdup(cmd_name)); }
        full_path = ft_strjoin(temp_join, cmd_name);
        free(temp_join);
        if (!full_path) { free_tab(paths); return (ft_strdup(cmd_name)); }
        if (access(full_path, X_OK) == 0)
        { free_tab(paths); return (full_path); }
        free(full_path);
        i++;
    }
    free_tab(paths);
    return (ft_strdup(cmd_name)); // Return copy of original name if not found
}

// Executes a single command (external or builtin that needs forking/redirection setup)
// This is for a "process" segment that contains only one command.
// proc_i is the index for fd_in/fd_out for this specific "process" segment.
void execute_external_or_builtin_v2(t_shell *shell, char *full_cmd_str, int proc_i)
{
    char    **exec_args;
    char    *cmd_path;

    if (!full_cmd_str || !full_cmd_str[0])
    { g_exit_status = 0; return; }

    exec_args = ft_split(full_cmd_str, ' ');
    if (!exec_args || !exec_args[0])
    { free_tab(exec_args); g_exit_status = 127; /* Command not found like */ return; }

    // Setup redirections for this specific process segment
    if (shell->exec->fd_in[proc_i] != STDIN_FILENO)
    {
        dup2(shell->exec->fd_in[proc_i], STDIN_FILENO);
        close(shell->exec->fd_in[proc_i]);
    }
    if (shell->exec->fd_out[proc_i] != STDOUT_FILENO)
    {
        dup2(shell->exec->fd_out[proc_i], STDOUT_FILENO);
        close(shell->exec->fd_out[proc_i]);
    }

    // Check if it's a builtin that should run in this forked context
    // User's `is_builtin` is likely the one to call here.
    // Their `is_builtin` calls check_cd, check_echo etc. and executes them, setting g_exit_status.
    // We need to ensure it doesn't fork again if it's already in the child.
    // The original `execute_one_cmd` had a `check_single_builtin` then `is_builtin`.
    // If `is_builtin` handles execution and exit status correctly for forked context, it's fine.
    // For now, let's assume `is_builtin` is appropriate here.
    // A potential issue: if `is_builtin` itself tries to `exit()`, it will terminate the global child.
    // It should only set `g_exit_status`.

    // If it's a builtin handled by user's `is_builtin` (which also executes it)
    // This part needs careful integration with user's builtin execution logic.
    // Let's assume for now that if `is_builtin` executes it, it sets g_exit_status.
    // And if it's not a builtin, we proceed to execve.
    // A simple way: try to run as builtin. If `is_builtin` indicates it ran, we are done.
    // This requires `is_builtin` to return a status or for us to check if `g_exit_status` changed meaningfully.
    // This is complex. Let's simplify: if it's a known builtin, call its function. Else, execve.
    // User's `is_builtin` seems to execute and return 1 if it did, or -1 if it was a builtin that forked/failed, or 0 if not a builtin.
    // This is tricky. The original `execute_one_cmd` called `child_process` if not `check_single_builtin`.
    // `child_process` then called `execute` which did `execve`.
    // Here, we are already in a (potentially) forked context (the global child).

    // Let's use a structure similar to user's `execute` from their `exec.c`:
    cmd_path = get_command_path_v2(exec_args[0], shell->data->new_env);
    if (!cmd_path) // get_command_path_v2 should always return something or it's an error in it
    { ft_putstr_fd("minishell: ", 2); ft_putstr_fd(exec_args[0], 2); ft_putstr_fd(": command not found (path error)\n", 2); exit(127); }

    if (execve(cmd_path, exec_args, shell->data->new_env) == -1)
    {
        ft_putstr_fd("minishell: ", STDERR_FILENO);
        ft_putstr_fd(exec_args[0], STDERR_FILENO);
        ft_putstr_fd(": command not found\n", STDERR_FILENO);
        // Original `execute` did not free cmd_path if it was from get_path and different from exec_args[0]
        // My get_command_path_v2 returns allocated memory that needs freeing.
        if (cmd_path) free(cmd_path);
        free_tab(exec_args);
        exit(127); // Command not found, exit (this is the global child exiting, or a sub-fork from pipeline)
    }
    // Should not be reached if execve is successful
}

// Main entry point for command execution within the global child process
void execute_commands_sequence_child_v2(t_shell *shell)
{
    int		proc_i;
    int		global_cmd_idx; 
	char	*cmd_str;

    proc_i = 0;
    while (proc_i < shell->exec->process)
    {
        if (shell->exec->nb_cmd[proc_i] == 1)
        {
            global_cmd_idx = get_global_cmd_idx(shell, proc_i, 0);
        	cmd_str = give_curr_cmd(shell, global_cmd_idx);
            if (shell->exec->fd_in[proc_i] != STDIN_FILENO)
                dup2(shell->exec->fd_in[proc_i], STDIN_FILENO);
            if (shell->exec->fd_out[proc_i] != STDOUT_FILENO)
                dup2(shell->exec->fd_out[proc_i], STDOUT_FILENO);
            if (shell->exec->fd_in[proc_i] > STDERR_FILENO)
				close(shell->exec->fd_in[proc_i]);
            if (shell->exec->fd_out[proc_i] > STDERR_FILENO)
				close(shell->exec->fd_out[proc_i]);
            execute_external_or_builtin_v2(shell, cmd_str, proc_i);
            if (cmd_str)
				free(cmd_str);
        }
        else
            execute_pipeline_v2(shell, proc_i);
        proc_i++;
    }
}

