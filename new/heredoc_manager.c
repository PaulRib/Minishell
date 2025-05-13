/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_manager.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 17:43:05 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/13 18:00:50 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
//#include "heredoc_manager.h"
// Assuming heredoc_utils.c functions are available via minishell.h or direct include if necessary
// e.g., check_current_type, check_and_create, create_new_hrd, check_end

// Part of initiate_heredoc_list
// Initializes a single new heredoc node if conditions are met during token scan.
static t_heredoc *manage_heredoc_node_creation(t_shell *shell, t_token *tok,
                                             t_heredoc *current_hd_node,
                                             int *proc_idx)
{
    if (tok->type == PIPE && tok->next &&
        (tok->next->type == REDIR_IN || tok->next->type == HEREDOC))
    {
        // check_and_create might call create_new_hrd which appends to tmp->next
        check_and_create(shell, tok->next, current_hd_node);
        if (current_hd_node->next)
            current_hd_node = current_hd_node->next;
        (*proc_idx)++;
    }
    return (current_hd_node);
}

// Adapted from initiate_heredoc in original heredoc.c
void initiate_heredoc_list(t_shell *shell)
{
    t_token     *current_token;
    t_heredoc   *temp_hd_node;
    int         process_idx;

    process_idx = 0;
    current_token = shell->token;
    shell->heredoc = malloc(sizeof(t_heredoc));
    if (!shell->heredoc)
    {
        g_exit_status = 1; // Malloc failure
        return;
    }
    ft_memset(shell->heredoc, 0, sizeof(t_heredoc));
    shell->heredoc->next = NULL;
    temp_hd_node = shell->heredoc;
    while (current_token)
    {
        check_current_type(current_token, temp_hd_node, process_idx); // from heredoc_utils
        temp_hd_node = manage_heredoc_node_creation(shell, current_token,
                                                  temp_hd_node, &process_idx);
        current_token = current_token->next;
    }
}

// Helper for stock_heredoc_delimiters: allocates memory for delimiter strings
static int allocate_node_delimiters_array(t_heredoc *hd_node)
{
    if (hd_node->nb_heredoc > 0)
    {
        hd_node->eof_heredoc = malloc(sizeof(char *) * hd_node->nb_heredoc);
        if (!hd_node->eof_heredoc)
        {
            g_exit_status = 1; // Malloc failure
            return (0);        // Indicate failure
        }
    }
    return (1); // Indicate success
}

// Helper for stock_heredoc_delimiters: finds and stores delimiters for one node
// Returns the advanced token_iter. Relies on nb_heredoc being correct.
static t_token *find_delims_for_one_node(t_heredoc *hd_node, t_token *token_iter)
{
    int delim_idx;

    delim_idx = 0;
    while (token_iter && delim_idx < hd_node->nb_heredoc)
    {
        if (token_iter->type == END) // END is the type for delimiter string
        {
            hd_node->eof_heredoc[delim_idx] = token_iter->str;
            delim_idx++;
        }
        // Original break: if (current->type == END && current->next->type != HEREDOC)
        // This logic determines if we should stop scanning tokens for *this* heredoc group.
        // It implies that if a delimiter is found, and the *next* token is not another heredoc
        // (for a different segment), then this group is done.
        if (token_iter->type == END && token_iter->next &&
            token_iter->next->type != HEREDOC && token_iter->next->type != END)
        {
            token_iter = token_iter->next; // Consume the token that caused break
            break;                        // Done with this node's delimiters
        }
        token_iter = token_iter->next;
    }
    return (token_iter);
}

// Adapted from stock_all_heredoc in original heredoc.c
void stock_heredoc_delimiters(t_shell *shell)
{
    t_token     *token_scanner;
    t_heredoc   *current_hd_node;

    token_scanner = shell->token; // Scan from the beginning of the token list
    current_hd_node = shell->heredoc;

    while (current_hd_node)
    {
        if (!allocate_node_delimiters_array(current_hd_node))
            return; // Malloc error, g_exit_status is set
        
        token_scanner = find_delims_for_one_node(current_hd_node, token_scanner);
        
        current_hd_node = current_hd_node->next;
    }
}



// Adapted from do_all_pipe in original heredoc.c
// Creates pipes for all heredoc structures that are marked as active (hrd == true)
// Returns 0 on success, 1 on pipe creation failure (g_exit_status will be set).
int create_heredoc_pipes(t_shell *shell)
{
    t_heredoc *current_hd_node;

    current_hd_node = shell->heredoc;
    while (current_hd_node)
    {
        if (current_hd_node->hrd == true) // Only create pipe if heredoc is active
        {
            if (pipe(current_hd_node->p_fd) == -1)
            {
                perror("minishell: pipe for heredoc");
                g_exit_status = 1; // Indicate critical error
                return (1);        // Failure
            }
        }
        current_hd_node = current_hd_node->next;
    }
    return (0); // Success
}

// Main function to handle all heredocs. Replaces old here_doc_hub logic (without fork).
// This is called by the global child process.
// Returns 0 on success, non-zero on failure or interruption (g_exit_status will be set).
int handle_all_heredocs_globally(t_shell *shell)
{
    // initiate_heredoc_list and stock_heredoc_delimiters are called by orchestrator
    // before this function, or should be called here if orchestrator doesn't.
    // Based on orchestrator.c, they are called before here_doc_process.
    // Let's assume they are already called and shell->heredoc is populated.

    if (create_heredoc_pipes(shell) != 0)
        return (g_exit_status); // Pipe creation failed, g_exit_status is set

    // process_heredoc_inputs_loop will read input for all heredocs.
    // It needs to be adapted from the original here_doc_process.
    // It should not fork and should handle signals/EOF by setting g_exit_status and returning.
    if (process_heredoc_inputs_loop(shell) != 0)
        return (g_exit_status); // Error or interruption during input reading

    return (0); // All heredocs processed successfully
}

// Placeholder for the complex heredoc input processing loop.
// This will be a significant adaptation of the original `here_doc_process`.
// It must not fork. It must handle SIGINT by setting g_exit_status = 130 and returning error.
// It must handle GNL errors/EOF by setting g_exit_status and returning error.
// int process_heredoc_inputs_loop(t_shell *shell)
// {
//     // TODO: Implement the detailed logic from here_doc_process here.
//     // This will involve iterating through shell->heredoc nodes,
//     // reading lines with get_next_line, comparing with delimiters,
//     // expanding variables if necessary, and writing to the correct pipe fd (p_fd[1]).
//     // Crucially, signal handling (SIGINT) needs to be managed to set g_exit_status
//     // and allow graceful exit from this loop, rather than exiting the whole process.
//     // The original `check_end` logic will also be integrated here or as a helper.
//     (void)shell;
//     ft_putstr_fd("Heredoc input loop to be implemented.\n", STDERR_FILENO);
//     // For now, assume success if no heredocs, or simulate an error if there are.
//     if (shell && shell->heredoc && shell->heredoc->nb_heredoc > 0)
//     {
//         // g_exit_status = 1; // Simulate an error for now if heredocs exist
//         // return (1);
//     }
//     return (0); // Placeholder: assume success
// }

