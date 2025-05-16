/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_fd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 15:41:29 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/16 20:22:01 by meel-war         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	open_outfile(t_shell *shell)
{
	int		i;
	t_token	*current;

	current = shell->token;
	i = 0;
	while (current)
	{
		if (current->type == FILE_OUT)
		{
			if (shell->exec->fd_out[i] != 1)
				close (shell->exec->fd_out[i]);
			if (current->prev->type == REDIR_OUT)
				shell->exec->fd_out[i] = open(current->str, O_WRONLY
						| O_CREAT | O_TRUNC, 0777);
			else if (current->prev->type == APPEND)
				shell->exec->fd_out[i] = open(current->str, O_WRONLY
						| O_CREAT | O_APPEND, 0777);
		}
		if (shell->exec->fd_out[i] > 1 && current->type == PIPE)
			i++;
		current = current->next;
	}
}

int	open_infile(t_shell *shell)
{
	int		i;
	t_token	*current;

	current = shell->token;
	i = 0;
	while (current->next)
	{
		if (current->type == FILE_IN)
		{
			if (shell->exec->fd_in[i])
				close (shell->exec->fd_in[i]);
			shell->exec->fd_in[i] = open(current->str, O_RDONLY);
			if (shell->exec->fd_in[i] == -1)
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(current->str, 2);
				ft_putstr_fd(": No such file or directory", 2);
				return (-1);
			}
			if (current->type == HEREDOC)
				shell->exec->fd_in[i] = 0;
		}
		if (current->type == PIPE && (current->next->type == REDIR_IN
				|| current->next->type == HEREDOC))
			i++;
		current = current->next;
	}
	return (0);
}

void	execute_pipe(t_shell *shell)
{
	int		i;
	int		proc;
	int		cmd;

	cmd = 0;
	proc = 0;
	while (proc < shell->exec->process)
	{
		if (shell->exec->prev_fd[proc] == 0)
			shell->exec->prev_fd[proc] = shell->exec->fd_in[proc];
		i = 0;
		while (i < shell->exec->nb_cmd[proc])
		{
			if (pipe(shell->exec->p_fd) == -1)
				exit(0);
			child_process(give_curr_cmd(shell, cmd), shell, proc, i);
			if (shell->exec->prev_fd[proc] > 0)
				close(shell->exec->prev_fd[proc]);
			close(shell->exec->p_fd[1]);
			shell->exec->prev_fd[proc] = shell->exec->p_fd[0];
			i++;
			cmd++;
		}
		proc++;
	}
}

void close_fd_exec(t_shell *shell)
{
	int	i;
	
	i = 0;
	while (i < shell->exec->process)
	{
		if (shell->exec->fd_in[i] != 0)
			close(shell->exec->fd_in[i]);
		if (shell->exec->fd_out[i] != 1)
			close(shell->exec->fd_out[i]);
		i++;
	}
}

void execute_one_cmd(t_shell *shell)
{
	if (check_single_builtin(shell) == -1)
		child_process(give_curr_cmd(shell, 0), shell, 0, 0);
	else
	{
		if (shell->exec->fd_in[0] != 0)
			dup2(shell->exec->fd_in[0], STDIN_FILENO);
		if (shell->exec->fd_out[0] != 1)
			dup2(shell->exec->fd_out[0], STDOUT_FILENO);
		is_builtin(shell);
	}
	dup2(STDIN_FILENO, 1);
	dup2(STDOUT_FILENO, 0);
}

int check_single_builtin(t_shell *shell)
{
	t_token	*current;

	current = shell->token;
	while (current)
	{
		if (ft_strncmp(current->str, "cd", 3) == 0)
			return (1);
		if (ft_strncmp(current->str, "echo", 5) == 0)
			return (1);
		if (ft_strncmp(current->str, "pwd", 4) == 0)
			return (1);
		if (ft_strncmp(current->str, "env", 4) == 0)
			return (1);
		if (ft_strncmp(current->str, "history", 8) == 0)
			return (1);
		if (ft_strncmp(current->str, "unset", 6) == 0)
			return (1);
		if (ft_strncmp(current->str, "export", 7) == 0)
			return (1);
		current = current->next;
	}
	return (-1);
}