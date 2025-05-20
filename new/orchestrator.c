/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_hub.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 16:42:28 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/20 14:36:33 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	execute_parsed_line(t_shell *shell)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("minishell: fork");
		g_exit_status = 1;
		return;
	}
	if (pid == 0)
		run_global_child_process_v2(shell);
	else
		wait_for_global_child_v2(pid, shell);
}

// Initialize execution environment for child process
void	init_child_execution(t_shell *shell)
{
	init_signals_child();
	count_process(shell);
	initiate_exec(shell);
	count_element(shell);
}

// Setup heredoc file descriptors
void	setup_heredoc_fds(t_shell *shell)
{
	int	i;

	i = 0;
	while (i < shell->exec->process)
	{
		if (shell->exec->prev_fd[i] > 2)
		{
			shell->exec->fd_in[i] = shell->exec->prev_fd[i];
			shell->exec->prev_fd[i] = 0;
		}
		i++;
	}
}

// Handle heredoc processing
int	process_heredocs(t_shell *shell)
{
	if (shell->count->nb_heredoc > 0)
	{
		initiate_heredoc(shell);
		stock_all_heredoc(shell);
		if (handle_all_heredocs_globally_v2(shell) != 0)
		{
			ft_free_exec(shell);
			ft_free_heredoc(shell);
			return (1);
		}
	}
	return (0);
}

// Handle file redirections
int	process_redirections(t_shell *shell)
{
	int	infile_status;

	if (shell->count->nb_redir_out > 0 || shell->count->nb_append > 0)
		open_outfile(shell);
	infile_status = 0;
	if (shell->count->nb_redir_in > 0)
		infile_status = open_infile(shell);
	if (infile_status == -1)
	{
		ft_free_exec(shell);
		ft_free_heredoc(shell);
		return (1);
	}
	return (0);
}

void	run_global_child_process_v2(t_shell *shell)
{
	init_child_execution(shell);
	if (process_heredocs(shell))
		exit(g_exit_status);
	if (process_redirections(shell))
		exit(g_exit_status);
	setup_heredoc_fds(shell);
	execute_commands_sequence_child_v2(shell);
	ft_free_exec(shell);
	ft_free_heredoc(shell);
	exit(g_exit_status);
}

void	wait_for_global_child_v2(pid_t child_pid, t_shell *shell)
{
	int	status;
	int	sig;

	(void)shell;
	waitpid(child_pid, &status, 0);
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		g_exit_status = 128 + sig;
		if (sig == SIGQUIT)
			ft_putendl_fd("Quit (core dumped)", STDERR_FILENO);
	}
}
