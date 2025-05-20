/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_input_processor.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 18:58:14 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/20 15:08:40 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	print_heredoc_eof_warning_msg_v2(char *delimiter)
{
	ft_putstr_fd("minishell: warning: ", 2);
	ft_putstr_fd("here-document delimited by end-of-file (wanted `", 2);
	if (delimiter)
		ft_putstr_fd(delimiter, 2);
	else
		ft_putstr_fd("unknown", 2);
	ft_putstr_fd("`)\n", 2);
}

static int	check_delimiter_match(char *line_read, char *delimiter)
{
	int	eof_str_len;

	eof_str_len = ft_strlen(delimiter);
	if (ft_strncmp(line_read, delimiter, eof_str_len) == 0
		&& line_read[eof_str_len] == '\n')
		return (1);
	return (0);
}

static int	process_heredoc_line(t_shell *shell, t_heredoc *hd_node,
		char *line_read)
{
	char	*line_to_write;

	if (hd_node->hrd == true)
	{
		line_to_write = expand_variables(shell, line_read);
		ft_putstr_fd(line_to_write, hd_node->p_fd[1]);
		free(line_to_write);
	}
	else
		ft_putstr_fd(line_read, hd_node->p_fd[1]);
	return (0);
}

void	setup_heredoc_fd_for_process(t_shell *shell, t_heredoc *hd_node, int proc_idx)
{
	if (hd_node->p_fd[0] > 2)
	{
		shell->exec->prev_fd[proc_idx] = hd_node->p_fd[0];
		if (hd_node->p_fd[1] > 2)
			close(hd_node->p_fd[1]);
	}
}

static int	read_heredoc_line(t_shell *shell, t_heredoc *hd_node, char *delimiter)
{
	char	*line_read;

	write(STDOUT_FILENO, "> ", 2);
	init_signals_heredoc();
	line_read = get_next_line(STDIN_FILENO);
	// init_signals(); sers a rien je pense
	if (g_exit_status == 1)
	{
		if (line_read)
			free(line_read);
		shell->exit_status = 130;
		return (3);
	}
	if (!line_read)
	{
		print_heredoc_eof_warning_msg_v2(delimiter);
		return (2);
	}
	if (check_delimiter_match(line_read, delimiter))
	{
		free(line_read);
		return (1);
	}
	process_heredoc_line(shell, hd_node, line_read);
	free(line_read);
	return (0);
}

void	close_heredoc_fds(t_shell *shell)
{
	t_heredoc	*current;
	int			i;

	current = shell->heredoc;
	while (current)
	{
		i = 0;
		while (i < current->nb_heredoc)
		{
			if (current->p_fd[1] > 2)
				close(current->p_fd[1]);
			i++;
		}
		current = current->next;
	}
}

int	process_heredoc_inputs_loop_v2(t_shell *shell)
{
	t_heredoc	*current;
	int			i;
	int			status;

	current = shell->heredoc;
	i = 0;
	while (current)
	{
		if (i >= current->nb_heredoc)
		{
			if (check_end(&i, shell, &current) == -1)
				break;
		}
		if (!current->eof_heredoc || !current->eof_heredoc[i])
		{
			g_exit_status = 1;
			return (3);
		}
		status = read_heredoc_line(shell, current, current->eof_heredoc[i]);
		if (status == 1)
			i++;
		else if (status == 2 || status == 3)
			return(status);
	}
	return (0);
}
