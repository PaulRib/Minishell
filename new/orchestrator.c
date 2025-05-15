/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   orchestrator.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 16:42:28 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/15 19:03:09 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void execute_parsed_line(t_shell *shell)
{
    pid_t pid;

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
        run_global_child_process_v2(shell);
    else
        wait_for_global_child_v2(pid, shell);
}

void run_global_child_process_v2(t_shell *shell)
{
    int infile_status;

    init_signals_child();
    count_process(shell);
    initiate_exec(shell);
    count_element(shell);

    if (shell->count->nb_heredoc > 0)
    {
        initiate_heredoc(shell);
        stock_all_heredoc(shell);
        if (handle_all_heredocs_globally_v2(shell) != 0)
        {
            ft_free_exec(shell);
            ft_free_heredoc(shell);
            exit(g_exit_status);
        }
    }
	if (shell->count->nb_redir_out > 0 || shell->count->nb_append > 0)
        open_outfile(shell);
    infile_status = 0;
    if (shell->count->nb_redir_in > 0)
        infile_status = open_infile(shell);
    if (infile_status == -1)
    {
        ft_free_exec(shell);
        ft_free_heredoc(shell);
        exit(g_exit_status); // g_exit_status should be set by open_infile
    }
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

