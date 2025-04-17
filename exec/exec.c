/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:01:22 by pribolzi          #+#    #+#             */
/*   Updated: 2025/04/17 14:26:34 by pribolzi         ###   ########.fr       */
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

static void	append_heredoc(t_shell *shell, int type)
{
	t_token	*current;

	current = shell->token;
	while (current->next)
	{
		if (type == 0)
		{
			if (current->type == REDIR_OUT)
				shell->exec->append = false;
			if (current->type == APPEND)
				shell->exec->append = true;
		}
		if (type == 1)
		{
			if (current->type == REDIR_IN)
				shell->exec->heredoc = false;
			if (current->type == HEREDOC)
				shell->exec->heredoc = true;
		}
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

// static void	stock_all_cmd(t_shell *shell)
// {
// 	int		i;
// 	t_token	*first;

// 	first = shell->token;
// 	shell->exec = malloc(sizeof(t_exec));
// 	ft_memset(shell->exec, 0, sizeof(t_exec));
// 	while (first->next)
// 	{
// 		if (first->type == CMD)
// 			shell->exec->nb_cmd++;
// 		first = first->next;
// 	}
// 	i = 0;
// 	shell->exec->cmd = malloc(sizeof(char *) * shell->exec->nb_cmd);
// 	first = shell->token;
// 	while (first->next && i < shell->exec->nb_cmd)
// 	{
// 		if (first->type == CMD)
// 		{
// 			shell->exec->cmd[i] = first->str;
// 			i++;
// 		}
// 		first = first->next;
// 	}
// 	shell->exec->cmd[i] = NULL;
// }

void	exec_hub(t_shell *shell)
{
	shell->exec = malloc(sizeof(t_exec));
	ft_memset(shell->exec, 0, sizeof(t_exec));
	count_element(shell);
	if (shell->count->nb_redir_in == 0 && shell->count->nb_redir_out == 0
		&& shell->count->nb_append == 0 && shell->count->nb_heredoc == 0)
	{
		shell->token->type = CMD;
		shell->exec->nb_cmd++;
	}
	if (shell->count->nb_heredoc > 1)
		stock_all_eof(shell);
	append_heredoc(shell, 0);
	append_heredoc(shell, 1);
}
