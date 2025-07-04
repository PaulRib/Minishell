/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_process.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 18:58:14 by pribolzi          #+#    #+#             */
/*   Updated: 2025/06/03 15:24:31 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
	return (0);
}

static int	read_heredoc_line(t_shell *shell, t_heredoc *hd_node,
								char *delimiter, int i)
{
	char	*line_read;

	write(STDOUT_FILENO, "> ", 2);
	init_signals_heredoc();
	line_read = get_next_line(STDIN_FILENO);
	if (g_exit_status == 1)
	{
		if (line_read)
			free(line_read);
		shell->exit_status = 130;
		close_heredoc_fds(shell);
		free_all(shell, 130);
	}
	if (!line_read)
		return (handle_heredoc_eof(shell, delimiter));
	if (check_delimiter_match(line_read, delimiter))
	{
		free(line_read);
		return (1);
	}
	if (i == hd_node->nb_heredoc - 1)
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
			if (current->p_fd[0] > 2)
				close(current->p_fd[0]);
			i++;
		}
		current = current->next;
	}
}

int	process_heredoc_inputs_loop(t_shell *shell)
{
	t_heredoc	*current;
	int			i;
	int			status;

	if (create_heredoc_pipes(shell) != 0)
		free_all(shell, 1);
	current = shell->heredoc;
	i = 0;
	while (current)
	{
		if (i >= current->nb_heredoc)
		{
			if (check_end(&i, shell, &current) == -1)
				break ;
		}
		status = read_heredoc_line(shell, current, current->eof_heredoc[i], i);
		if (status == 1)
			i++;
	}
	return (0);
}
