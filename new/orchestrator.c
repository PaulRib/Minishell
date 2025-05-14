/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   orchestrator.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 16:42:28 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/14 17:39:52 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
// Assuming new headers will be created for v2 files if necessary
// For now, direct function declarations or rely on minishell.h if updated by user

// Forward declarations for functions from user's original files (or new v2 files)
// User's functions (ensure prototypes are in minishell.h or accessible)
// extern void count_process(t_shell *shell); // from user's exec_utils.c
// extern void initiate_exec(t_shell *shell); // from user's exec_utils.c
// extern void count_element(t_shell *shell); // from user's exec_utils.c
// extern int  open_infile(t_shell *shell);   // from user's exec_fd.c
// extern void open_outfile(t_shell *shell);  // from user's exec_fd.c
// extern void ft_free_exec(t_shell *shell);  // from user's exec_utils.c
// extern void ft_free_heredoc(t_shell *shell); // from user's heredoc_utils.c

// Main function to call after parsing, replaces old exec_hub entry point
void execute_parsed_line(t_shell *shell)
{
    pid_t pid;

    // Handle builtins that must run in the parent and modify its environment,
    // but only if they are the *only* command (no pipes).
    // User's `count_process` and `initiate_exec` must be called first to have `shell->exec->process`
    // and `shell->count->nb_pipe` (from `count_element`) available for this check.
    // This implies a slight reordering or that these builtins are checked even before `count_process`.
    // For now, assuming this check is done based on shell->token and a preliminary pipe count.
    // if (shell->count->nb_pipe == 0 && is_simple_builtin_modifying_parent(shell))
    // {
    //     execute_simple_builtin_in_parent(shell);
    //     return;
    // }

    pid = fork();
    if (pid < 0)
    {
        perror("minishell: fork");
        g_exit_status = 1;
        return;
    }
    if (pid == 0)
    {
        // Child process executes this
        // Ensure `shell->exec` is NULL before `count_process` if `count_process` re-mallocs it.
        // Based on user's exec_utils.c, count_process does the first malloc for shell->exec.
        run_global_child_process_v2(shell);
    }
    else
    {
        // Parent process waits here
        wait_for_global_child_v2(pid, shell);
    }
}

// Logic for the single global child process
void run_global_child_process_v2(t_shell *shell)
{
    int infile_status;

    init_signals_child(); // Setup signal handlers for child

    // 1. Use user's functions to count processes and initialize structures
    count_process(shell);   // User's function from exec_utils.c (mallocs shell->exec)
    initiate_exec(shell);   // User's function from exec_utils.c (mallocs arrays in shell->exec)
    count_element(shell);   // User's function from exec_utils.c (fills nb_cmd, counts pipes etc.)

    // 2. Handle heredocs (using adapted v2 functions)
    if (shell->count->nb_heredoc > 0)
    {
        initiate_heredoc(shell);  // Prepares heredoc list, uses user's check_current_type
        stock_all_heredoc(shell); // Gathers EOF strings
        if (handle_all_heredocs_globally_v2(shell) != 0) // Processes inputs, sets prev_fd
        {
            // Error or interruption during heredoc processing (e.g., SIGINT sets g_exit_status = 130)
            ft_free_exec(shell);    // User's function
            ft_free_heredoc(shell); // User's function
            exit(g_exit_status);
        }
    }

    // 3. Open regular input/output files using user's functions
    // These will use shell->exec->fd_in[proc_idx] and fd_out[proc_idx]
    infile_status = 0;
    if (shell->count->nb_redir_in > 0)
        infile_status = open_infile(shell); // User's function from exec_fd.c

    if (infile_status == -1) // Error opening infile from user's open_infile
    {
        ft_free_exec(shell);
        ft_free_heredoc(shell);
        exit(g_exit_status); // g_exit_status should be set by open_infile
    }

    if (shell->count->nb_redir_out > 0 || shell->count->nb_append > 0)
        open_outfile(shell); // User's function from exec_fd.c
        // If open_outfile fails, it should set g_exit_status. We might need to check it.

    // 4. Execute the command sequence (single or pipeline)
    execute_commands_sequence_child_v2(shell); // Defined in command_executor_v2.c

    // 5. Cleanup
    ft_free_exec(shell);    // User's function
    ft_free_heredoc(shell); // User's function
    exit(g_exit_status);      // Global child exits with the final status
}

// Parent process waits for the global child and sets exit status
void wait_for_global_child_v2(pid_t child_pid, t_shell *shell)
{
    int status;
    int sig;

    (void)shell; // shell might be used if more complex status handling is needed
    waitpid(child_pid, &status, 0);
    if (WIFEXITED(status))
        g_exit_status = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
    {
        sig = WTERMSIG(status);
        g_exit_status = 128 + sig;
        if (sig == SIGQUIT)
            ft_putendl_fd("Quit (core dumped)", STDERR_FILENO); // Or your error printing
        // SIGINT message (like ^C) is usually handled by terminal or child's signal handler printing newline
    }
}

