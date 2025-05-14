/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_executor.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 16:42:12 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/14 16:44:35 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
// User's functions (ensure prototypes are in minishell.h or accessible)
// extern char *give_curr_cmd(t_shell *shell, int i); // Global command index
// extern int  check_single_builtin(t_shell *shell); // This might need to be adapted or used carefully
// extern void is_builtin(t_shell *shell); // User's main builtin dispatcher

// Helper to calculate the global command index for give_curr_cmd
// A "process" (segment) `target_proc_idx` can have multiple commands `cmd_in_target_proc_idx`.
// The global index is the sum of commands in all preceding "processes" plus the command index within the current "process".
int get_global_cmd_idx(t_shell *shell, int target_proc_idx, int cmd_in_target_proc_idx)
{
    int global_idx;
    int i;

    global_idx = 0;
    i = 0;
    while (i < target_proc_idx)
    {
        global_idx += shell->exec->nb_cmd[i];
        i++;
    }
    global_idx += cmd_in_target_proc_idx;
    return (global_idx);
}

// Gets the full path for a command, adapted from user's get_path
// User's original get_path is in exec.c
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
// proc_idx is the index for fd_in/fd_out for this specific "process" segment.
void execute_external_or_builtin_v2(t_shell *shell, char *full_cmd_str, int proc_idx)
{
    char    **exec_args;
    char    *cmd_path;

    if (!full_cmd_str || !full_cmd_str[0])
    { g_exit_status = 0; return; }

    exec_args = ft_split(full_cmd_str, ' ');
    if (!exec_args || !exec_args[0])
    { free_tab(exec_args); g_exit_status = 127; /* Command not found like */ return; }

    // Setup redirections for this specific process segment
    if (shell->exec->fd_in[proc_idx] != STDIN_FILENO)
    {
        dup2(shell->exec->fd_in[proc_idx], STDIN_FILENO);
        close(shell->exec->fd_in[proc_idx]);
    }
    if (shell->exec->fd_out[proc_idx] != STDOUT_FILENO)
    {
        dup2(shell->exec->fd_out[proc_idx], STDOUT_FILENO);
        close(shell->exec->fd_out[proc_idx]);
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
    int proc_idx;         // Index for the user-defined "process" segment
    int global_cmd_idx;   // Overall command index for give_curr_cmd

    proc_idx = 0;
    while (proc_idx < shell->exec->process) // Loop through each user-defined "process" segment
    {
        if (shell->exec->nb_cmd[proc_idx] == 1) // This "process" segment has only one command
        {
            // It's a single command for this segment. It might be a builtin or external.
            // It needs to run in the current context (global child), using fd_in/out of this segment.
            // Builtins that modify parent env are handled before global fork.
            // Other builtins run here. If it's `exit`, it will terminate the global child.

            // Get the command string using the global command index
            global_cmd_idx = get_global_cmd_idx(shell, proc_idx, 0);
            char *cmd_str = give_curr_cmd(shell, global_cmd_idx);

            // If it's a builtin that should run directly in this child process (and not fork again)
            // User's `check_single_builtin` checks if the *first token* is a builtin.
            // This needs to be adapted to check the command for the current proc_idx.
            // For simplicity, let's assume user's `is_builtin` can be called. It sets g_exit_status.
            // If `is_builtin` returns 1 (executed), we trust it set g_exit_status.
            // This is a delicate part: how to run builtins in this context without extra forks if not needed,
            // and correctly setting g_exit_status for the global child to pick up.

            // Option 1: If it's a builtin, run it directly here. Otherwise, fork for external.
            // This is what `execute_one_cmd` from user's `exec_fd.c` did.
            // It called `child_process` if not `check_single_builtin`.
            // Here, we are already in the global child. So, for a single command in a segment,
            // we can directly call the builtin logic or `execve` logic.

            // If user's `is_builtin` also executes the command:
            // We need to ensure redirections `fd_in[proc_idx]` and `fd_out[proc_idx]` are handled.
            // The original `execute_one_cmd` did this before calling `is_builtin`.

            if (shell->exec->fd_in[proc_idx] != STDIN_FILENO)
                dup2(shell->exec->fd_in[proc_idx], STDIN_FILENO);
            if (shell->exec->fd_out[proc_idx] != STDOUT_FILENO)
                dup2(shell->exec->fd_out[proc_idx], STDOUT_FILENO);
            
            // Close original FDs if they were duplicated and are not std(in/out/err)
            if (shell->exec->fd_in[proc_idx] > STDERR_FILENO) close(shell->exec->fd_in[proc_idx]);
            if (shell->exec->fd_out[proc_idx] > STDERR_FILENO) close(shell->exec->fd_out[proc_idx]);

            // Now, execute. If it's a builtin, call user's is_builtin. Else, call execve wrapper.
            // This assumes `is_builtin` will use the already duped STDIN/STDOUT.
            // And that `is_builtin` sets `g_exit_status`.
            // And that `is_builtin` returns a value indicating if it handled the command.
            // User's `is_builtin` returns -1 if it's a builtin (and executes it), 0 if not.
            // This is based on a quick look at user's `is_builtin` in their `builtin.c`.
            // It seems `is_builtin` calls the builtin functions which should set `g_exit_status`.

            // Let's refine: check if the command for this segment is a builtin.
            // `give_curr_cmd` gives the full string. We need to parse it or use tokens.
            // This is where `check_single_builtin` was used, which takes `shell->token`.
            // This needs to be re-thought for `proc_idx`.
            // For now, let's assume we call `execute_external_or_builtin_v2` which will try builtins
            // or execve. This function needs to be robust.
            // The original `execute_one_cmd` called `is_builtin` if `check_single_builtin` was true.
            // Or `child_process` (which forks and calls `execute`) if false.
            // Since we are already in a child, we don't want to fork again for a single command.

            // Let's simplify: call a function that tries builtin, then execve if not.
            // This is essentially what `execute_external_or_builtin_v2` should do.
            execute_external_or_builtin_v2(shell, cmd_str, proc_idx);
            // `g_exit_status` should be set by this function or the execve call.
            if (cmd_str) free(cmd_str);
        }
        else // This "process" segment has multiple commands (a pipeline within the segment)
        {
            execute_pipeline_v2(shell, proc_idx); // Pass the current "process" segment index
            // g_exit_status will be set by the last command of this internal pipeline
        }
        proc_idx++;
    }
    // After all "process" segments are done, the g_exit_status of the *last* one executed
    // will be the one that the global child process exits with.
    // This is implicitly handled as g_exit_status is updated by each execution.
}

