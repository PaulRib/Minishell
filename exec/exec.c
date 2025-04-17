/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:01:22 by pribolzi          #+#    #+#             */
/*   Updated: 2025/04/17 15:14:14 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	count_element(t_shell *shell)
{
	t_token	*current;

	current = shell->token;
	while (current->next)
	{
		if (current->type == PIPE)
			shell->count->nb_pipe++;
		if (current->type == REDIR_IN)
			shell->count->nb_redir_in++;
		if (current->type == REDIR_OUT)
			shell->count->nb_redir_out++;
		if (current->type == APPEND)
			shell->count->nb_append++;
		if (current->type == HEREDOC)
			shell->count->nb_heredoc++;
		if (current->type == CMD)
			shell->exec->nb_cmd++;
		current = current->next;
	}
}

static void	stock_all_eof(t_shell *shell)
{
	t_token	*current;
	int		i;

	i = 0;
	current = shell->token;
	shell->exec->eof_heredoc = malloc(sizeof(char *)
			* (shell->count->nb_heredoc) + 1);
	while (current->next)
	{
		if (current->type == END)
			shell->exec->eof_heredoc[i++] = current->str;
		current = current->next;
	}
	shell->exec->eof_heredoc[i] = NULL;
}

void	open_outfile(t_shell *shell)
{
	int		fd;
	t_token	*current;

	current = shell->token;
	while (current->next)
	{
		if (current->type == FILE_OUT)
		{
			if (fd)
				close (fd);
			if (current->prev->type == REDIR_OUT)
				fd = open(current->str, O_WRONLY | O_CREAT | O_TRUNC, 0777);
			else if (current->prev->type == APPEND)
				fd = open(current->str, O_WRONLY | O_CREAT | O_APPEND, 0777);
			if (fd == -1)
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(current->str, 2);
				ft_putstr_fd(": No such file or directory", 2);
			}
		}
	}
	shell->pipex->fd_out = fd;
}

void	open_infile(t_shell *shell)
{
	int		fd;
	t_token	*current;

	current = shell->token;
	while (current->next)
	{
		if (current->type == FILE_IN)
		{
			if (fd)
				close (fd);
			fd = open(current->str, O_RDONLY);
			if (fd == -1)
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(current->str, 2);
				ft_putstr_fd(": No such file or directory", 2);
			}
		}
		else if (current->type == HEREDOC)
			if (fd)
				close (fd);
	}
	shell->pipex->fd_in = fd;
}

void	exec_hub(t_shell *shell)
{
	shell->exec = malloc(sizeof(t_exec));
	ft_memset(shell->exec, 0, sizeof(t_exec));
	count_element(shell);
	if (shell->count->nb_redir_in == 0 && shell->count->nb_redir_out == 0
		&& shell->count->nb_append == 0 && shell->count->nb_heredoc == 0)
	{
		shell->token->type = CMD;$
		shell->exec->nb_cmd++;
	}
	if (shell->count->nb_heredoc > 1)
		stock_all_eof(shell);
	shell->pipex = malloc(sizeof(t_pipex));
	if (shell->count->nb_redir_in > 0)
		open_infile(shell);
}
