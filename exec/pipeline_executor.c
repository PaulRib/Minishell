/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_executor.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 16:42:32 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/28 18:12:25 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_pipeline_child(t_shell *shell, int proc_i, t_pipe *pipe,
		char *cmd_str)
{
	init_signals_cmd();
	setup_pipeline_redir(shell, proc_i, pipe);
	close_all_pipe_fds(shell, pipe->pipe_fds, proc_i);
	if (!is_cmd_a_builtin(shell, pipe))
		execute_command(shell, cmd_str);
	else
		exit(0);
}

int	fork_pipeline_command(t_shell *shell, int proc_i, t_pipe *pipe)
{
	char	*cmd_str;

	pipe->global_idx = get_global_cmd_idx(shell, proc_i, pipe->cmd_idx);
	cmd_str = give_curr_cmd(shell, pipe->global_idx);
	if (!cmd_str)
	{
		shell->exit_status = 1;
		free_all(shell, 1);
	}
	pipe->pids[pipe->cmd_idx] = fork();
	if (pipe->pids[pipe->cmd_idx] < 0)
	{
		free(cmd_str);
		shell->exit_status = 1;
		free_all(shell, 1);
	}
	if (pipe->pids[pipe->cmd_idx] == 0)
		handle_pipeline_child(shell, proc_i, pipe, cmd_str);
	else
		signal_block();
	free(cmd_str);
	return (1);
}

void	execute_pipeline_commands(t_shell *shell, int proc_i,
		int (*pipe_fds)[2], pid_t *pids)
{
	t_pipe	pipe;

	pipe.pipe_fds = pipe_fds;
	pipe.pids = pids;
	pipe.cmd_idx = 0;
	while (pipe.cmd_idx < shell->exec->nb_cmd[proc_i])
	{
		fork_pipeline_command(shell, proc_i, &pipe);
		pipe.cmd_idx++;
	}
}

void	setup_pipeline_redir(t_shell *shell, int proc_i, t_pipe *pipe)
{
	if (pipe->cmd_idx == 0)
	{
		if (shell->exec->fd_in[proc_i] > 2)
		{
			dup2(shell->exec->fd_in[proc_i], STDIN_FILENO);
			close(shell->exec->fd_in[proc_i]);
		}
	}
	else
		dup2(pipe->pipe_fds[pipe->cmd_idx - 1][0], STDIN_FILENO);
	if (pipe->cmd_idx == shell->exec->nb_cmd[proc_i] - 1)
	{
		if (shell->exec->fd_out[proc_i] > 2)
		{
			dup2(shell->exec->fd_out[proc_i], STDOUT_FILENO);
			close(shell->exec->fd_out[proc_i]);
		}
	}
	else
	{
		dup2(pipe->pipe_fds[pipe->cmd_idx][1], STDOUT_FILENO);
		close(pipe->pipe_fds[pipe->cmd_idx][1]);
	}
}
