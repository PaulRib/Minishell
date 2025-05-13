/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_input_processor.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 17:43:07 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/13 17:53:38 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h" // For t_shell, g_exit_status, etc.
//#include "heredoc_input_processor.h"
#include <stdio.h> // For perror, if not in libft

// Assumed global for SIGINT handling during heredoc, set by signal handler
// extern int g_sigint_heredoc_received;
// For now, we'll assume get_next_line behavior or another mechanism handles SIGINT detection.
// If get_next_line returns NULL on SIGINT, that needs to be distinguished from EOF.
// Let's assume a simplified path: if GNL returns NULL, it's EOF or error.
// Proper SIGINT handling would require a flag set by a signal handler.

static void print_heredoc_eof_warning_msg(const char *delimiter)
{
	ft_putstr_fd("minishell: warning: here-document delimited by end-of-file (wanted `", 2);
	ft_putstr_fd((char *)delimiter, 2);
	ft_putstr_fd("`)\n", 2);
}

// Handles the logic when a heredoc node is finished or loop is ending for it.
// Returns 0 if there are more heredoc nodes, -1 if all done.
static int finalize_heredoc_node(t_shell *shell, t_heredoc **current_hd_ptr,
                                 int *delim_idx_ptr)
{
	t_heredoc *node;

	node = *current_hd_ptr;
	if (node->hrd == true && shell->exec && node->process < shell->exec->process)
		shell->exec->prev_fd[node->process] = node->p_fd[0];
	if (node->p_fd[1] > 0) // Close write end of the pipe for this heredoc
	{
		close(node->p_fd[1]);
		node->p_fd[1] = -1; // Mark as closed
	}
	if (node->next)
	{
		*current_hd_ptr = node->next;
		*delim_idx_ptr = 0;
		return (0); // More nodes
	}
	*current_hd_ptr = NULL; // No more heredoc nodes
	return (-1);          // All done
}

// Processes one line of input for the current heredoc delimiter.
// Returns 0 to continue, 1 to break from this node, 2 for overall error/interrupt.
static int process_line_for_heredoc(t_shell *shell, t_heredoc *node,
                                    int *d_idx, char *line)
{
	int d_len;

	d_len = ft_strlen(node->eof_heredoc[*d_idx]);
	if (ft_strncmp(line, node->eof_heredoc[*d_idx], d_len) == 0 &&
	    line[d_len] == '\n')
	{
		(*d_idx)++; // Matched this delimiter
	}
	else if (*d_idx == node->nb_heredoc - 1 || *d_idx == node->nb_heredoc)
	{
		if (node->hrd == true)
		{
			line = expand_variables(shell, line); // expand_variables might realloc line
			ft_putstr_fd(line, node->p_fd[1]);
		}
	}
	// If not delimiter and not matching the specific write condition, line is consumed
	free(line);
	if (*d_idx == node->nb_heredoc) // All delimiters for this node met
		return (1); // Break from this node
	return (0);     // Continue reading for this node
}

// Reads input lines for a single heredoc node until all its delimiters are met or EOF.
// Returns 0 on success for this node, 1 on EOF/warning, 2 on SIGINT/critical error.
static int read_input_for_node(t_shell *shell, t_heredoc *current_hd_node,
                               int *delim_idx_for_node)
{
	char *line;
	int  status;

	while (*delim_idx_for_node < current_hd_node->nb_heredoc)
	{
		ft_putstr_fd("> ", 1); // Prompt
		line = get_next_line(0);
		// TODO: Proper SIGINT check here. If SIGINT, set g_exit_status=130, free line, return 2.
		// For now, GNL returning NULL is EOF or error.
		if (!line)
		{
			print_heredoc_eof_warning_msg(current_hd_node->eof_heredoc[*delim_idx_for_node]);
			g_exit_status = 0; // Bash behavior
			return (1);        // EOF occurred, stop processing heredocs
		}
		status = process_line_for_heredoc(shell, current_hd_node, 
		                                  delim_idx_for_node, line);
		if (status == 1) // All delims for this node met
			break;
		// if status == 2 (error), it would have returned already from process_line (not current impl)
	}
	return (0); // Success for this node (all its delimiters found)
}

// Main loop to process inputs for all heredocs.
// Called by handle_all_heredocs_globally (in heredoc_manager.c).
// Returns 0 on success, 1 on EOF/warning, 2 on SIGINT/critical error.
int process_heredoc_inputs_loop(t_shell *shell)
{
	t_heredoc *current_hd_node;
	int delim_idx_for_node;
	int node_status;

	current_hd_node = shell->heredoc;
	delim_idx_for_node = 0;
	while (current_hd_node)
	{
		node_status = read_input_for_node(shell, current_hd_node, &delim_idx_for_node);
		if (node_status != 0) // EOF or SIGINT occurred
		{
			// Finalize current node partially if EOF, then return status
			// If it was SIGINT (status 2), g_exit_status is 130.
			// If it was EOF (status 1), g_exit_status is 0.
			finalize_heredoc_node(shell, &current_hd_node, &delim_idx_for_node); 
			return (node_status == 2 ? 2 : 1); // Return 2 for sigint, 1 for eof
		}
		// All delimiters for current_hd_node were found successfully.
		if (finalize_heredoc_node(shell, &current_hd_node, &delim_idx_for_node) == -1)
			break; // All heredoc nodes processed
	}
	return (0); // All heredocs processed successfully
}

