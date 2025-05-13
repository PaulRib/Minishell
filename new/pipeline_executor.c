/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_executor.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 17:42:55 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/13 17:50:17 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "pipeline_executor.h"
#include "../includes/minishell.h"

// External functions that will be linked from other C files.
// These should ideally be in minishell.h or specific headers if not already.
extern char *give_curr_cmd(t_shell *shell, int i);
extern void execute_external_or_builtin(char *full_cmd_str, t_shell *shell);

// Child helper: sets up FDs for a command in a pipeline.
// Called by the child process after fork.
void setup_pipe_fds(t_shell *shell, int proc_idx, int cmd_in_proc_idx)
{
	init_signals_child();
	close(shell->exec->p_fd[0]); // Child closes read end of its output pipe

	if (shell->exec->prev_fd[proc_idx] != STDIN_FILENO)
	{
		dup2(shell->exec->prev_fd[proc_idx], STDIN_FILENO);
		close(shell->exec->prev_fd[proc_idx]);
	}

	if (cmd_in_proc_idx != shell->exec->nb_cmd[proc_idx] - 1)
	{
		dup2(shell->exec->p_fd[1], STDOUT_FILENO);
	}
	else if (shell->exec->fd_out[proc_idx] != STDOUT_FILENO)
	{
		dup2(shell->exec->fd_out[proc_idx], STDOUT_FILENO);
		if (shell->exec->fd_out[proc_idx] > STDERR_FILENO)
			close(shell->exec->fd_out[proc_idx]);
	}
	close(shell->exec->p_fd[1]);
}

// Parent helper: waits for a pipeline command, sets exit status, frees command string.
void wait_for_pipeline_command(pid_t pid, t_shell *shell, int proc_idx, char *cmd_str)
{
	int status;
	int sig;

	(void)shell; // g_exit_status is global
	(void)proc_idx; // Not directly used in this wait logic part
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		g_exit_status = 128 + sig;
		if (sig == SIGQUIT)
			ft_putendl_fd("Quit (core dumped)", STDERR_FILENO);
	}
	if (cmd_str)
		free(cmd_str);
}

// Forks, child executes a pipeline command, parent waits.
void execute_pipeline_command(t_shell *shell, char *cmd_str, int proc_idx, int cmd_in_proc_idx)
{
	pid_t pid;

	pid = fork();
	if (pid < 0)
	{
		perror("minishell: fork");
		if (cmd_str)
			free(cmd_str);
		g_exit_status = 1;
		return;
	}
	if (pid == 0)
	{
		setup_pipe_fds(shell, proc_idx, cmd_in_proc_idx);
		execute_external_or_builtin(cmd_str, shell); // Exits on its own
		exit(g_exit_status); // Fallback exit if exec/builtin somehow returns
	}
	wait_for_pipeline_command(pid, shell, proc_idx, cmd_str);
}

// Parent helper: manages its pipe FDs after a child is launched and waited for.
static void parent_manage_fds_after_cmd(t_shell *shell, int proc_idx)
{
	if (shell->exec->prev_fd[proc_idx] > STDIN_FILENO)
		close(shell->exec->prev_fd[proc_idx]);

	close(shell->exec->p_fd[1]);
	shell->exec->prev_fd[proc_idx] = shell->exec->p_fd[0];
}

// Main function to execute a pipeline of commands.
void execute_pipeline(t_shell *shell)
{
	int p_idx;
	int c_idx_p;
	int overall_cmd_idx;
	char *cmd_s;

	overall_cmd_idx = 0;
	p_idx = 0;
	while (p_idx < shell->exec->process)
	{
		if (shell->exec->prev_fd[p_idx] == STDIN_FILENO)
			shell->exec->prev_fd[p_idx] = shell->exec->fd_in[p_idx];
		c_idx_p = 0;
		while (c_idx_p < shell->exec->nb_cmd[p_idx])
		{
			if (pipe(shell->exec->p_fd) == -1)
			{perror("minishell: pipe"); g_exit_status = 1; return;}
			cmd_s = give_curr_cmd(shell, overall_cmd_idx);
			if (!cmd_s) {g_exit_status = 1; close(shell->exec->p_fd[0]);
				close(shell->exec->p_fd[1]); return;}
			execute_pipeline_command(shell, cmd_s, p_idx, c_idx_p);
			parent_manage_fds_after_cmd(shell, p_idx);
			c_idx_p++;
			overall_cmd_idx++;
		}
		if (shell->exec->prev_fd[p_idx] > STDIN_FILENO)
			{close(shell->exec->prev_fd[p_idx]); shell->exec->prev_fd[p_idx] = STDIN_FILENO;}
		p_idx++;
	}
}

