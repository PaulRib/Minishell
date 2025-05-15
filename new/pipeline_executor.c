/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_executor.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 16:42:32 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/15 19:30:14 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/minishell.h"

// User's functions (ensure prototypes are in minishell.h or accessible)
// extern char *give_curr_cmd(t_shell *shell, int global_cmd_idx);
// extern void init_signals_child(void);

// Helper to wait for a single pipeline command process
void wait_for_pipeline_command_v2(pid_t pid, char *cmd_str)
{
    int status;
    int sig;

    waitpid(pid, &status, 0);
    if (WIFEXITED(status))
        g_exit_status = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
    {
        sig = WTERMSIG(status);
        g_exit_status = 128 + sig;
        if (sig == SIGQUIT)
            ft_putendl_fd("Quit (core dumped)", STDERR_FILENO);
    }
    if (cmd_str)
        free(cmd_str);
}

// Sets up FDs for a command within an internal pipeline of a "process" segment.
// proc_i: index of the overall "process" segment (for fd_in/fd_out of the segment).
// cmd_in_segment_idx: index of the command within this segment's internal pipeline.
// num_cmds_in_segment: total number of commands in this segment's internal pipeline.
// pipe_fds: array of pipe FDs for this internal pipeline. pipe_fds[j][0] is read, pipe_fds[j][1] is write.
void setup_internal_pipe_fds_v2(t_shell *shell, int proc_i,
                                int cmd_in_segment_idx, int num_cmds_in_segment,
                                int (*pipe_fds)[2])
{
    init_signals_child();

    // Input redirection
    if (cmd_in_segment_idx == 0) // First command in the internal pipeline
    {
        // Use the main input for this "process" segment
        // This could be from a heredoc (via prev_fd) or a file redirection
        if (shell->exec->prev_fd[proc_i] != STDIN_FILENO && shell->exec->prev_fd[proc_i] != 0) // Check if prev_fd is set and valid
        {
            dup2(shell->exec->prev_fd[proc_i], STDIN_FILENO);
            close(shell->exec->prev_fd[proc_i]);
        }
        else if (shell->exec->fd_in[proc_i] != STDIN_FILENO)
        {
            dup2(shell->exec->fd_in[proc_i], STDIN_FILENO);
            close(shell->exec->fd_in[proc_i]);
        }
    }
    else // Not the first command, take input from the previous command's pipe
    {
        dup2(pipe_fds[cmd_in_segment_idx - 1][0], STDIN_FILENO);
    }

    // Output redirection
    if (cmd_in_segment_idx == num_cmds_in_segment - 1) // Last command in the internal pipeline
    {
        // Use the main output for this "process" segment
        if (shell->exec->fd_out[proc_i] != STDOUT_FILENO)
        {
            dup2(shell->exec->fd_out[proc_i], STDOUT_FILENO);
            close(shell->exec->fd_out[proc_i]);
        }
    }
    else // Not the last command, output to the next command's pipe
    {
        dup2(pipe_fds[cmd_in_segment_idx][1], STDOUT_FILENO);
    }

    // Close all pipe fds in the child, as they are now duplicated to stdin/stdout or not needed
    for (int i = 0; i < num_cmds_in_segment - 1; ++i)
    {
        close(pipe_fds[i][0]);
        close(pipe_fds[i][1]);
    }
}

// Main function to execute a pipeline of commands within a specific "process" segment.
void execute_pipeline_v2(t_shell *shell, int proc_i)
{
    int num_cmds_in_seg;
    int (*pipe_fds)[2]; // Array of pipes for this internal pipeline
    pid_t *pids;
    char *cmd_str;
    int global_cmd_idx_base;
    int i;

    num_cmds_in_seg = shell->exec->nb_cmd[proc_i];
    pipe_fds = NULL;
    pipe_fds = malloc(sizeof(int[2]) * (num_cmds_in_seg - 1));
    if (!pipe_fds)
		exit(0);
	i = 0;
    while (i < num_cmds_in_seg - 1)
    {
        if (pipe(pipe_fds[i]) == -1)
			exit(0);
		i++;
    }
    pids = malloc(sizeof(pid_t) * num_cmds_in_seg);
    if (!pids)
		exit(0);
    global_cmd_idx_base = get_global_cmd_idx(shell, proc_i, 0);
    i = 0;
    while (i < num_cmds_in_seg)
    {
        cmd_str = give_curr_cmd(shell, global_cmd_idx_base + i);
        if (!cmd_str) { g_exit_status = 1; /* Error getting command */ break; }

        pids[i] = fork();
        if (pids[i] < 0)
        { perror("minishell: fork"); free(cmd_str); g_exit_status = 1; break; }

        if (pids[i] == 0) // Child process for this command in the internal pipeline
        {
            setup_internal_pipe_fds_v2(shell, proc_i, i, num_cmds_in_seg, pipe_fds);
            // execute_external_or_builtin_v2 will handle builtins or execve, and exit.
            // It uses fd_in[proc_i] and fd_out[proc_i] if it were a single command,
            // but setup_internal_pipe_fds_v2 has already redirected STDIN/STDOUT.
            // So, execute_external_or_builtin_v2 needs to be careful not to re-dup if called from here.
            // For now, we pass proc_i, but its fd_in/fd_out usage inside might need adjustment
            // if called from a pipeline context where STDIN/STDOUT are already piped.
            // A simpler execute_external_or_builtin_v2 might just take cmd_str and rely on STDIN/STDOUT being set.
            execute_external_or_builtin_v2(shell, cmd_str, proc_i); // Pass proc_i for context if needed by builtin logic
            exit(g_exit_status); // Should be redundant if execute_external_or_builtin_v2 exits
        }
        // Parent (global child) continues to fork next command in segment
        free(cmd_str); // Parent frees the command string from give_curr_cmd
        i++;
    }

    // Parent (global child) closes all its copies of pipe FDs for this internal pipeline
    if (pipe_fds)
    {
        for (i = 0; i < num_cmds_in_seg - 1; ++i)
        {
            close(pipe_fds[i][0]);
            close(pipe_fds[i][1]);
        }
        free(pipe_fds);
    }

    // Parent (global child) waits for all commands in this internal pipeline
    // g_exit_status will be that of the last command waited for.
    for (i = 0; i < num_cmds_in_seg; ++i)
    {
        if (pids[i] > 0) // Only wait if fork was successful for this command
        {
            // We don't have the cmd_str here anymore to pass to wait_for_pipeline_command_v2.
            // The original wait_for_pipeline_command took cmd_str to free it.
            // Here, cmd_str is freed in the loop above. So, a simpler wait is needed.
            int temp_status;
            waitpid(pids[i], &temp_status, 0);
            if (i == num_cmds_in_seg - 1) // If it's the last command of the pipeline segment
            {
                if (WIFEXITED(temp_status))
                    g_exit_status = WEXITSTATUS(temp_status);
                else if (WIFSIGNALED(temp_status))
                {
                    int sig = WTERMSIG(temp_status);
                    g_exit_status = 128 + sig;
                    if (sig == SIGQUIT)
                        ft_putendl_fd("Quit (core dumped)", STDERR_FILENO);
                }
            }
        }
    }
    free(pids);
}

