/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_hub.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 16:42:28 by pribolzi          #+#    #+#             */
/*   Updated: 2025/06/03 17:29:49 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	execute_parsed_line(t_shell *shell)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
	{
		shell->exit_status = 1;
		return ;
	}
	if (pid == 0)
		run_global_child_process(shell);
	else
	{
		signal_block();
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			shell->exit_status = WEXITSTATUS(status);
		init_signals();
	}
}

int	process_heredocs(t_shell *shell)
{
	initiate_heredoc(shell);
	stock_all_heredoc(shell);
	process_heredoc_inputs_loop(shell);
	return (0);
}

void	run_global_child_process(t_shell *shell)
{
	init_signals();
	if (shell->count->nb_heredoc > 0)
		process_heredocs(shell);
	process_redirections(shell);
	setup_heredoc_fds(shell);
	execute_child(shell);
	free_all(shell, shell->exit_status);
}

void	execute_child(t_shell *shell)
{
	int	proc_i;

	proc_i = 0;
	while (proc_i < shell->exec->process)
	{
		execute_pipe(shell, proc_i);
		proc_i++;
	}
}

void	execute_pipe(t_shell *shell, int proc_i)
{
	pid_t	*pids;
	int		(*pipe_fds)[2];

	pipe_fds = NULL;
	if (shell->exec->nb_cmd[proc_i] > 1)
		if (!create_pipes(shell, &pipe_fds, proc_i))
			free_all(shell, 1);
	pids = malloc(sizeof(pid_t) * shell->exec->nb_cmd[proc_i]);
	if (!pids)
	{
		close_all_pipe_fds(shell, pipe_fds, proc_i);
		free(pipe_fds);
		free_all(shell, 1);
	}
	execute_all_commands(shell, proc_i, pipe_fds, pids);
	close_all_pipe_fds(shell, pipe_fds, proc_i);
	free(pipe_fds);
	wait_for_all_commands(shell, pids, proc_i);
	free(pids);
	init_signals();
}
