/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 16:01:44 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/29 14:21:50 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	count_element(t_shell *shell)
{
	t_token	*current;
	int		i;

	i = 0;
	current = shell->token;
	while (current->next)
	{
		if (current->type == PIPE && current->next->type != REDIR_IN
			&& current->next->type != HEREDOC)
		{
			shell->count->nb_pipe++;
			shell->exec->nb_cmd[i]++;
		}
		if (current->type == REDIR_IN)
			shell->count->nb_redir_in++;
		if (current->type == REDIR_OUT)
			shell->count->nb_redir_out++;
		if (current->type == APPEND)
			shell->count->nb_append++;
		if (current->type == HEREDOC)
			shell->count->nb_heredoc++;
		if (current->type == FILE_OUT && current->next->type == PIPE)
			i++;
		current = current->next;
	}
}

void	initiate_exec(t_shell *shell)
{
	int	i;

	i = 0;
	shell->exec->fd_in = malloc(sizeof(int) * shell->exec->process);
	shell->exec->fd_out = malloc(sizeof(int) * shell->exec->process);
	shell->exec->nb_cmd = malloc(sizeof(int) * shell->exec->process);
	shell->exec->prev_fd = malloc(sizeof(int) * shell->exec->process);
	if (!shell->exec->fd_in || !shell->exec->fd_out || !shell->exec->nb_cmd
		|| !shell->exec->prev_fd)
		free_all(shell, 1);
	while (i < shell->exec->process)
	{
		shell->exec->prev_fd[i] = 0;
		shell->exec->fd_in[i] = 0;
		shell->exec->fd_out[i] = 1;
		shell->exec->nb_cmd[i] = 1;
		i++;
	}
}

void	count_process(t_shell *shell)
{
	t_token	*current;

	shell->exec = malloc(sizeof(t_exec));
	if (!shell->exec)
		free_all(shell, 1);
	ft_memset(shell->exec, 0, sizeof(t_exec));
	current = shell->token;
	shell->exec->process = 1;
	while (current->next)
	{
		if (current->type == PIPE && (current->next->type == REDIR_IN
				|| current->next->type == HEREDOC))
			shell->exec->process++;
		current = current->next;
	}
}

void	ft_free_exec(t_shell *shell)
{
	if (shell->exec)
	{
		close_fd_exec(shell);
		if (shell->exec->nb_cmd)
			free(shell->exec->nb_cmd);
		if (shell->exec->fd_in)
			free(shell->exec->fd_in);
		if (shell->exec->fd_out)
			free(shell->exec->fd_out);
		if (shell->exec->prev_fd)
			free(shell->exec->prev_fd);
		free(shell->exec);
		shell->exec = NULL;
	}
}

char	**extract_cmd(t_shell *shell, t_token *current, int count)
{
	int		i;
	char	**cmd;

	cmd = malloc(sizeof(char *) * (count + 1));
	i = 0;
	while (current && current->type != PIPE)
	{
		if (current->type == WORD || current->type == CMD
			|| current->type == S_QUOTE || current->type == D_QUOTE)
		{
			cmd[i] = current->str;
			if (!cmd[i])
				free_all(shell, 1);
			i++;
		}
		current = current->next;
	}
	cmd[i] = NULL;
	return (cmd);
}
