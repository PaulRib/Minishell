/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 13:36:54 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/09 16:54:34 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	do_all_pipe(t_shell *shell)
{
	t_heredoc	*current;

	current = shell->heredoc;
	while (current)
	{
		if (current->hrd == true)
			if (pipe(current->p_fd) == -1)
				exit(0);
		current = current->next;
	}
}

static void	here_doc_process(t_shell *shell)
{
	int			i;
	char		*line;
	int			len;
	t_heredoc	*current;

	i = 0;
	current = shell->heredoc;
	do_all_pipe(shell);
	while (1)
	{
		ft_putstr_fd(">", 1);
		len = ft_strlen(current->eof_heredoc[i]);
		line = get_next_line(0);
		if (!line)
			exit(0);
		if (!ft_strncmp(line, current->eof_heredoc[i], len)
			&& line[len] == '\n')
			i++;
		else if (i == current->nb_heredoc - 1)
			if (current->hrd == true)
			{
				line = expand_variables(shell, line);
				ft_putstr_fd(line, current->p_fd[1]);
			}
		free(line);
		if (i == current->nb_heredoc && check_end(&i, shell, &current) == -1)
			break ;
	}
}

static void	stock_all_heredoc(t_shell *shell)
{
	t_token		*current;
	t_heredoc	*tmp;
	int			i;

	current = shell->token;
	tmp = shell->heredoc;
	while (tmp)
	{
		i = 0;
		tmp->eof_heredoc = malloc(sizeof(char *)
				* tmp->nb_heredoc);
		while (current)
		{
			if (current->type == END)
				tmp->eof_heredoc[i++] = current->str;
			if (current->type == END && current->next->type != HEREDOC)
			{
				current = current->next;
				break ;
			}
			current = current->next;
		}
		tmp = tmp->next;
	}
}

static void	initiate_heredoc(t_shell *shell)
{
	t_token		*current;
	t_heredoc	*tmp;
	int			process;

	process = 0;
	current = shell->token;
	shell->heredoc = malloc(sizeof(t_heredoc));
	if (!shell->heredoc)
		return ;
	ft_memset(shell->heredoc, 0, sizeof(t_heredoc));
	shell->heredoc->next = NULL;
	tmp = shell->heredoc;
	while (current)
	{
		check_current_type(current, tmp, process);
		if (current->type == PIPE && (current->next->type == REDIR_IN
				|| current->next->type == HEREDOC))
		{
			check_and_create(shell, current->next, tmp);
			if (tmp->next)
				tmp = tmp->next;
			process++;
		}
		current = current->next;
	}
}

void	here_doc_hub(t_shell *shell)
{
	pid_t		pid;
	t_heredoc	*hdr;
	int			status;

	initiate_heredoc(shell);
	stock_all_heredoc(shell);
	hdr = shell->heredoc;
	pid = fork();
	if (pid == -1)
		exit(0);
	if (pid == 0)
		here_doc_process(shell);
	waitpid(pid, &status, WUNTRACED);
	ft_free_heredoc(shell);
}
