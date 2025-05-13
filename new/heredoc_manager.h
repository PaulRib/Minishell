#ifndef HEREDOC_MANAGER_H
# define HEREDOC_MANAGER_H

# include "../includes/minishell.h"

// Main function to handle all heredocs. Replaces old here_doc_hub logic (without fork).
// Returns 0 on success, non-zero on failure or interruption (g_exit_status will be set).
int		handle_all_heredocs_globally(t_shell *shell);

// Initializes the list of heredoc structures.
// Adapted from initiate_heredoc in original heredoc.c
void	initiate_heredoc_list(t_shell *shell);

// Stocks all heredoc end-of-file delimiters.
// Adapted from stock_all_heredoc in original heredoc.c
void	stock_heredoc_delimiters(t_shell *shell);

// Processes inputs for all heredocs in the list.
// Adapted from here_doc_process in original heredoc.c
// Returns 0 on success, non-zero on error/interruption.
int		process_heredoc_inputs_loop(t_shell *shell);

// Creates pipes for all heredocs that need them.
// Adapted from do_all_pipe in original heredoc.c
// Returns 0 on success, non-zero if pipe creation fails.
int		create_heredoc_pipes(t_shell *shell);

#endif

