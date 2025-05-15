/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_input_processor.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 18:58:14 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/15 19:21:17 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void print_heredoc_eof_warning_msg_v2(char *delimiter)
{
    ft_putstr_fd("minishell: warning: ", 2);
	ft_putstr_fd("here-document delimited by end-of-file (wanted `", 2);
    if (delimiter)
        ft_putstr_fd(delimiter, 2);
    else
        ft_putstr_fd("unknown", 2);
    ft_putstr_fd("`)\n", 2);
}

static int read_heredoc_line(t_shell *shell, t_heredoc *hd_node,char *delimiter)
{
    char *line_read;
    char *line_to_write;
    int eof_str_len;

    ft_putstr_fd(">", 1);
    line_read = get_next_line(STDIN_FILENO);
    if (g_exit_status == 130)
        if (line_read) 
            return(free(line_read), 3);
    if (!line_read)
    {
        print_heredoc_eof_warning_msg_v2(delimiter);
        g_exit_status = 0;
        return (2);
    }
    eof_str_len = ft_strlen(delimiter);
    if (ft_strncmp(line_read, delimiter, eof_str_len) == 0 &&
        line_read[eof_str_len] == '\n')
        return(free(line_read), 1);
    if (hd_node->hrd == true)
    {
        line_to_write = expand_variables(shell, line_read);
        ft_putstr_fd(line_to_write, hd_node->p_fd[1]);
        free(line_to_write);
    }
    return (free(line_read), 0);
}

int process_heredoc_inputs_loop_v2(t_shell *shell)
{
    t_heredoc   *current;
    int         i;
    int         status;

    current = shell->heredoc;
    i = 0;
    while (current)
    {
        if (i >= current->nb_heredoc)
            if (check_end(&i, shell, &current) == -1)
                break;
        if (!current->eof_heredoc || !current->eof_heredoc[i])
        {
			g_exit_status = 1;
            return (3);
        }
        status = read_heredoc_line(shell, current, current->eof_heredoc[i]);
        if (status == 1)
            i++;
        else if (status == 2)
            return (1);
        else if (status == 3)
            return (2);
    }
    return (0);
}
