/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 13:36:54 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/06 19:35:39 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void do_all_pipe(t_shell *shell)
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
		if (!ft_strncmp(line, current->eof_heredoc[i], len) && line[len] == '\n')
			i++;
		else if (i == current->nb_heredoc - 1)
			if (current->hrd == true)
				ft_putstr_fd(line, current->p_fd[1]);
		if (i == current->nb_heredoc)
		{
			if (current->hrd == true)
				shell->exec->prev_fd[current->process] = current->p_fd[0];
			// else
			// 	close(current->p_fd[0]);
			close(current->p_fd[1]);
			if (current->next)
			{
				i = 0;
				current = current->next;
			}
			else
				break ;
		}
		free(line);
	}
}


static void	stock_all_heredoc(t_shell *shell)
{
	t_token	*current;
	t_heredoc *tmp;
	int		i;

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
			{
				tmp->eof_heredoc[i] = current->str;
				i++;
			}
			if (current->type == PIPE && (current->next->type == REDIR_IN
				|| current->next->type == HEREDOC))
				break ;
			current = current->next;
		}
		tmp = tmp->next;
	}
}

static void	create_new_hrd(t_shell *shell, t_heredoc *tmp)
{
	t_heredoc *new;
	
	(void)shell;
	new = malloc(sizeof(t_heredoc));
	ft_memset(new, 0, sizeof(t_heredoc));
	while (tmp)
		tmp = tmp->next;
	tmp->next = new;
}

static int	check_and_create(t_shell *shell, t_token *current, t_heredoc *tmp)
{
	while (current)
	{
		if (current->type == END)
		{
			create_new_hrd(shell, tmp);
			return (0);
		}
		if (current->type == PIPE && (current->next->type == REDIR_IN
			|| current->next->type == HEREDOC))
			return (0);
		current = current->next;
	}
	return (-1);
}

static void	initiate_heredoc(t_shell *shell)
{
	t_token		*current;
	t_heredoc	*tmp;
	int			process;

	process = 0;
	current = shell->token;
	shell->heredoc = malloc(sizeof(t_heredoc));
	ft_memset(shell->heredoc, 0, sizeof(t_heredoc));
	shell->heredoc->next = NULL;
	tmp = shell->heredoc;
	while (current)
	{
		if (current->type == FILE_IN || current->type == REDIR_IN)
			shell->heredoc->hrd = false;
		if (current->type == END)
		{
			shell->heredoc->hrd = true;
			tmp->nb_heredoc++;
			tmp->process = process;
		}
		if (current->type == PIPE && (current->next->type == REDIR_IN
			|| current->next->type == HEREDOC))
		{
			if (check_and_create(shell, current->next, tmp) == -1)
					return ;
			if (tmp->next)
				tmp = tmp->next;
			process++;
		}
		current = current->next;
	}
}

static void print_heredoc(t_shell *shell)
{
	t_heredoc *tmp;
	int i;

	tmp = shell->heredoc;
	while (tmp)
	{
		for (i = 0; i < tmp->nb_heredoc; i++)
		{
			if (tmp->eof_heredoc && tmp->eof_heredoc[i])
				printf("EOF %d: %s\n", i, tmp->eof_heredoc[i]);
			else
				printf("EOF %d: (null)\n", i);
		}
		printf("nb_heredoc: %d\n", tmp->nb_heredoc);
		printf("hrd: %d\n", tmp->hrd);
		printf("process: %d\n", tmp->process);
		printf("---\n");
		tmp = tmp->next;
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
	// if (pid != 0)
	if (pid > 0)
		print_heredoc(shell);
	if (pid == 0)
		here_doc_process(shell);
	//close(p_fd[1]);
	waitpid(pid, &status, WUNTRACED);
}
