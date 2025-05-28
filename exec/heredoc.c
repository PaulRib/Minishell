/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 13:36:54 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/28 14:55:41 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	stock_all_heredoc(t_shell *shell)
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
		if (!tmp->eof_heredoc)
			free_all(shell, 1);
		while (current)
		{
			if (current->type == END)
				tmp->eof_heredoc[i++] = ft_strdup(current->str);
			if (current->type == PIPE && current->next->type != REDIR_IN
			&& current->next->type != HEREDOC)
			{
				current = current->next;
				break ;
			}
			current = current->next;
		}
		tmp = tmp->next;
	}
}

void	initiate_heredoc(t_shell *shell)
{
	t_token		*current;
	t_heredoc	*tmp;
	int			process;

	process = 0;
	current = shell->token;
	shell->heredoc = malloc(sizeof(t_heredoc));
	if (!shell->heredoc)
		free_all(shell, 1);
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

static void close_all_heredoc_fds(t_heredoc *heredoc_list)
{
    t_heredoc *current_hd;

    current_hd = heredoc_list;
    while (current_hd)
    {
        if (current_hd->p_fd[0] > 0 && current_hd->p_fd[0] != -1)
        {
            close(current_hd->p_fd[0]);
            current_hd->p_fd[0] = -1;
        }
        if (current_hd->p_fd[1] > 0 && current_hd->p_fd[1] != -1)
        {
            close(current_hd->p_fd[1]);
            current_hd->p_fd[1] = -1;
        }
        current_hd = current_hd->next;
    }
}

int create_heredoc_pipes_v2(t_shell *shell)
{
    t_heredoc *current;

    current = shell->heredoc;
    while (current)
    {
        current->p_fd[0] = -1;
        current->p_fd[1] = -1;
        if (current->hrd == true)
        {
            if (pipe(current->p_fd) == -1)
            {
                perror("minishell: pipe for heredoc");
                shell->exit_status = 1;
                close_all_heredoc_fds(shell->heredoc);
                return (1);
            }
        }
        current = current->next;
    }
    return (0);
}

int handle_all_heredocs_globally_v2(t_shell *shell)
{
    int process_status;

    if (!shell->heredoc)
        return (0);
    if (create_heredoc_pipes_v2(shell) != 0)
		free_all(shell, 1);
    process_status = process_heredoc_inputs_loop(shell);
    if (process_status == 0 || process_status == 1)
        return (0);
    else 
    {
        close_all_heredoc_fds(shell->heredoc);
    	ft_free_heredoc(&shell);
        shell->heredoc = NULL;
        if (process_status == 2)
            return (130);
        else
            return (1);
    }
}
