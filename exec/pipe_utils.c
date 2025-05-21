/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:25:19 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/21 18:30:51 by meel-war         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	close_all_pipe_fds(t_shell *shell, int (*pipe_fds)[2], int proc_i)
{
	int	i;

	i = 0;
	while (i < shell->exec->nb_cmd[proc_i] - 1)
	{
		if (pipe_fds[i][0] > 2)
			close(pipe_fds[i][0]);
		if (pipe_fds[i][1] > 2)
			close(pipe_fds[i][1]);
		i++;
	}
}

int	create_pipeline_pipes(t_shell *shell, int (**pipe_fds)[2], int proc_i)
{
	int	i;
	int	j;

	*pipe_fds = malloc(sizeof(int[2]) * (shell->exec->nb_cmd[proc_i] - 1));
	if (!(*pipe_fds))
		free_all(shell);
	i = 0;
	while (i < shell->exec->nb_cmd[proc_i] - 1)
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
			free_all(shell);
		}
		i++;
	}
	return (1);
}

static void	wait_for_command(t_shell *shell, pid_t pid, int is_last)
{
	int	status;
	int	sig;

	waitpid(pid, &status, 0);
	if (is_last)
	{
		if (WIFEXITED(status))
			shell->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			sig = WTERMSIG(status);
			if (sig == SIGQUIT)
			{
				shell->exit_status = 131;
				handle_sigquit_cmd(sig);
			}
			if(sig == SIGINT)
			{
				shell->exit_status = 130;
				handle_sigint_cmd(sig);
			}
		}
	}
}

void	wait_for_all_commands(t_shell *shell, pid_t *pids, int proc_i)
{
	int	i;
	int	is_last;

	i = 0;
	while (i < shell->exec->nb_cmd[proc_i])
	{
		if (pids[i] > 0)
		{
			if (i == shell->exec->nb_cmd[proc_i] - 1)
				is_last = shell->exec->nb_cmd[proc_i];
			else
				is_last = 0;
			wait_for_command(shell, pids[i], is_last);
		}
		i++;
	}
}
