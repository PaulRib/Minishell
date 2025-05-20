/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_executor.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 16:42:32 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/20 14:37:19 by meel-war         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Setup input redirection for pipeline command
void	setup_pipeline_input(t_shell *shell, int proc_i, 
		int cmd_idx, int (*pipe_fds)[2])
{
	if (cmd_idx == 0)
	{
		if (shell->exec->prev_fd[proc_i] > 2)
		{
			dup2(shell->exec->prev_fd[proc_i], STDIN_FILENO);
			close(shell->exec->prev_fd[proc_i]);
		}
		else if (shell->exec->fd_in[proc_i] > 2)
		{
			dup2(shell->exec->fd_in[proc_i], STDIN_FILENO);
			close(shell->exec->fd_in[proc_i]);
		}
	}
	else
	{
		dup2(pipe_fds[cmd_idx - 1][0], STDIN_FILENO);
	}
}

// Setup output redirection for pipeline command
void	setup_pipeline_output(t_shell *shell, int proc_i,
		int cmd_idx, int num_cmds, int (*pipe_fds)[2])
{
	if (cmd_idx == num_cmds - 1)
	{
		if (shell->exec->fd_out[proc_i] > 2)
		{
			dup2(shell->exec->fd_out[proc_i], STDOUT_FILENO);
			close(shell->exec->fd_out[proc_i]);
		}
	}
	else
	{
		dup2(pipe_fds[cmd_idx][1], STDOUT_FILENO);
	}
}

// Close all pipe file descriptors
void	close_all_pipe_fds(int num_cmds, int (*pipe_fds)[2])
{
	int	i;

	i = 0;
	while (i < num_cmds - 1)
	{
		if (pipe_fds[i][0] > 2)
			close(pipe_fds[i][0]);
		if (pipe_fds[i][1] > 2)
			close(pipe_fds[i][1]);
		i++;
	}
}

// Sets up FDs for a command within a pipeline
void	setup_pipeline_fds(t_shell *shell, int proc_i,
		int cmd_idx, int num_cmds, int (*pipe_fds)[2])
{
	init_signals_cmd();
	setup_pipeline_input(shell, proc_i, cmd_idx, pipe_fds);
	setup_pipeline_output(shell, proc_i, cmd_idx, num_cmds, pipe_fds);
	close_all_pipe_fds(num_cmds, pipe_fds);
}

// Create pipes for the pipeline
int	create_pipeline_pipes(int num_cmds, int (**pipe_fds)[2])
{
	int	i;
	int	j;

	*pipe_fds = malloc(sizeof(int[2]) * (num_cmds - 1));
	if (!(*pipe_fds))
	{
		return (0);
	}
	i = 0;
	while (i < num_cmds - 1)
	{
		if (pipe((*pipe_fds)[i]) == -1)
		{
			j = 0;
			while (j < i)
			{
				close((*pipe_fds)[j][0]);
				close((*pipe_fds)[j][1]);
				j++;
			}
			free(*pipe_fds);
			*pipe_fds = NULL;
			return (0);
		}
		i++;
	}
	return (1);
}

// Allocate memory for process IDs
pid_t	*allocate_pids(int num_cmds)
{
	pid_t	*pids;

	pids = malloc(sizeof(pid_t) * num_cmds);
	if (!pids)
	{
		return (NULL);
	}
	return (pids);
}

// Handle child process execution
void	handle_pipeline_child(t_shell *shell, int proc_i, int cmd_idx, 
		int num_cmds, int (*pipe_fds)[2], char *cmd_str)
{
	setup_pipeline_fds(shell, proc_i, cmd_idx, num_cmds, pipe_fds);
	execute_command(shell, cmd_str);
	exit(0);
}

// Fork and execute a command in the pipeline
int	fork_pipeline_command(t_shell *shell, int proc_i, int cmd_idx, 
		int num_cmds, int (*pipe_fds)[2], pid_t *pids)
{
	char	*cmd_str;
	int		global_cmd_idx;

	global_cmd_idx = get_global_cmd_idx(shell, proc_i, cmd_idx);
	cmd_str = give_curr_cmd(shell, global_cmd_idx);
	if (!cmd_str)
	{
		shell->exit_status = 1;
		return (0);
	}
	pids[cmd_idx] = fork();
	if (pids[cmd_idx] < 0)
	{
		perror("minishell: fork");
		free(cmd_str);
		shell->exit_status = 1;
		return (0);
	}
	if (pids[cmd_idx] == 0)
	{
		handle_pipeline_child(shell, proc_i, cmd_idx, num_cmds, 
			pipe_fds, cmd_str);
	}
	else
		signal_block();
	free(cmd_str);
	return (1);
}

// Execute all commands in the pipeline
void	execute_pipeline_commands(t_shell *shell, int proc_i, 
		int num_cmds, int (*pipe_fds)[2], pid_t *pids)
{
	int	i;
	int	success;

	i = 0;
	while (i < num_cmds)
	{
		success = fork_pipeline_command(shell, proc_i, i, 
				num_cmds, pipe_fds, pids);
		if (!success)
		{
			break;
		}
		i++;
	}
}

// Wait for a specific command in the pipeline
void	wait_for_command(pid_t pid, int is_last, t_shell *shell)
{
	int	status;
	int	sig;

	waitpid(pid, &status, 0);
	if (is_last)
	{
		if (WIFEXITED(status))
			shell->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{// dans commande bloquante dans le cas de pipe
			sig = WTERMSIG(status);
			if (sig == SIGQUIT)
			{
				shell->exit_status = 131;
				handle_sigquit_cmd(sig);
				//valeur de retour shell->exit_status
			}
			if(sig == SIGINT)
			{
				shell->exit_status = 130;
				handle_sigint_cmd(sig);
				//valeur de retour shell->exit_status
			}
		}
	}
}

// Wait for all commands in the pipeline to complete
void	wait_for_all_commands(int num_cmds, pid_t *pids, t_shell *shell)
{
	int	i;
	int	is_last;

	i = 0;
	while (i < num_cmds)
	{
		if (pids[i] > 0)
		{
			is_last = (i == num_cmds - 1);
			wait_for_command(pids[i], is_last, shell);
		}
		i++;
	}
}

// Main function to execute a pipeline of commands
void	execute_pipeline_v2(t_shell *shell, int proc_i)
{
	int		num_cmds;
	int		(*pipe_fds)[2];
	pid_t	*pids;

	num_cmds = shell->exec->nb_cmd[proc_i];
	pipe_fds = NULL;
	if (!create_pipeline_pipes(num_cmds, &pipe_fds))
	{
		exit(1);
	}
	pids = allocate_pids(num_cmds);
	if (!pids)
	{
		close_all_pipe_fds(num_cmds, pipe_fds);
		free(pipe_fds);
		exit(1);
	}
	execute_pipeline_commands(shell, proc_i, num_cmds, pipe_fds, pids);
	close_all_pipe_fds(num_cmds, pipe_fds);
	free(pipe_fds);
	wait_for_all_commands(num_cmds, pids, shell);
	init_signals();
	free(pids);
}
