/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   orchestrator.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 17:42:59 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/13 18:20:27 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Forward declarations that might be in other new files or need organizing
// int is_simple_builtin_modifying_parent(t_shell *shell);
// void execute_simple_builtin_in_parent(t_shell *shell);
void run_global_child_process(t_shell *shell);
void wait_for_global_child(pid_t child_pid, t_shell *shell);

// Main function to call after parsing, replaces old exec_hub entry point
void execute_parsed_line(t_shell *shell)
{
	pid_t pid;

	// Handle builtins that must run in the parent and modify its environment,
	// but only if they are the *only* command (no pipes).
	// if (shell->count->nb_pipe == 0 && is_simple_builtin_modifying_parent(shell))
	// {
	// 	execute_simple_builtin_in_parent(shell);
	// 	return;
	// }
	pid = fork();
	if (pid < 0)
	{
		perror("minishell: fork"); // Basic error handling for fork failure
		g_exit_status = 1;
		return;
	}
	if (pid == 0)
		run_global_child_process(shell); // Child process executes this
	else
		wait_for_global_child(pid, shell); // Parent process waits here
}

// Logic for the single global child process
void run_global_child_process(t_shell *shell)
{
	init_signals_child(); // Setup signal handlers for child

	// Initialize execution-related structures (fd arrays, counts, etc.)
	initiate_exec(shell); // from exec_utils.c
	count_process(shell); // from exec_utils.c - counts segments for fd arrays
	count_element(shell); // from exec_utils.c - counts pipes, redirections, heredocs

	if (shell->count->nb_heredoc > 0)
	{
		initiate_heredoc(shell);  // from heredoc.c - prepares heredoc list
		stock_all_heredoc(shell); // from heredoc.c - gathers EOF strings
		// here_doc_process (from heredoc.c) is called here.
		// It must be adapted to not exit on GNL EOF/SIGINT but set g_exit_status and return.
		// Pipe errors within here_doc_process (e.g. in do_all_pipe) can still exit(1).
		here_doc_process(shell); 
	}

	// If heredoc processing was interrupted (e.g., SIGINT), g_exit_status would be non-zero.
	// Bash typically aborts the command line if heredoc is interrupted by SIGINT (status 130).
	if (g_exit_status == 130) // Check if heredoc was interrupted by SIGINT
	{
		ft_free_exec(shell);
		ft_free_heredoc(shell);
		exit(130);
	}
	// If g_exit_status is non-zero from other heredoc errors (e.g. GNL failure if adapted to set status)
	// we might also consider exiting early, but bash often proceeds if it's just a warning.
	// For now, we proceed unless it was SIGINT.

	execute_commands_sequence_child(shell); // Defined in command_executor.c

	ft_free_exec(shell);    // from exec_utils.c
	ft_free_heredoc(shell); // from heredoc_utils.c
	exit(g_exit_status);      // Global child exits with the final status
}

// Parent process waits for the global child and sets exit status
void wait_for_global_child(pid_t child_pid, t_shell *shell)
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
			ft_putendl_fd("Quit (core dumped)", STDERR_FILENO);
		// SIGINT message (like ^C) is usually handled by terminal or child's signal handler printing newline
	}
}

