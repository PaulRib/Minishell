/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_hub.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 16:42:28 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/20 18:14:25 by pribolzi         ###   ########.fr       */
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
		perror("minishell: fork");
		shell->exit_status = 1;
		return;
	}
	if (pid == 0)
		run_global_child_process_v2(shell);
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
	if (handle_all_heredocs_globally_v2(shell) != 0)
	{
		ft_free_exec(shell);
		ft_free_heredoc(shell);
		return (1);
	}
	return (0);
}

void	run_global_child_process_v2(t_shell *shell)
{
	init_signals();
	if (shell->count->nb_heredoc > 0)
		if (process_heredocs(shell))
			exit(shell->exit_status);
	if (process_redirections(shell))
		exit(shell->exit_status);
	setup_heredoc_fds(shell);
	execute_commands_sequence_child_v2(shell);
	ft_free_exec(shell);
	ft_free_heredoc(shell);
	exit(shell->exit_status);
}

void	execute_commands_sequence_child_v2(t_shell *shell)
{
	int	proc_i;

	proc_i = 0;
	while (proc_i < shell->exec->process)
	{
		execute_pipeline_v2(shell, proc_i);
		proc_i++;
	}
}

void	execute_pipeline_v2(t_shell *shell, int proc_i)
{
	int		(*pipe_fds)[2];
	pid_t	*pids;

	pipe_fds = NULL;
	if (shell->exec->nb_cmd[proc_i] > 1)
		if (!create_pipeline_pipes(shell, &pipe_fds, proc_i))
			exit(1);
	pids = malloc(sizeof(pid_t) * shell->exec->nb_cmd[proc_i]);
	if (!pids)
		//free all
	if (!pids)
	{
		close_all_pipe_fds(shell, pipe_fds, proc_i);
		free(pipe_fds);
		exit(1);
	}
	execute_pipeline_commands(shell, proc_i, pipe_fds, pids);
	close_all_pipe_fds(shell, pipe_fds, proc_i);
	free(pipe_fds);
	wait_for_all_commands(shell, pids, proc_i);
	init_signals();
	free(pids);
}
