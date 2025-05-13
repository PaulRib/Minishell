#ifndef HEREDOC_INPUT_PROCESSOR_H
# define HEREDOC_INPUT_PROCESSOR_H

#include "../includes/minishell.h"

// Processes all heredoc inputs. Reads from STDIN until delimiters are found.
// Writes content to appropriate pipes stored in shell->heredoc list.
// Handles SIGINT and EOF (Ctrl+D) during input.
// Returns 0 on success, 1 on interruption (e.g., SIGINT) or critical error.
// g_exit_status will be set accordingly (e.g., 130 for SIGINT).
int		process_heredoc_inputs_loop(t_shell *shell);

#endif

