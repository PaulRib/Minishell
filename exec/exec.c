/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:01:22 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/14 17:13:43 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_path(char *cmd, char **envp)
{
	int		i;
	char	**path;
	char	*good_path;
	char	*temp;

	i = 0;
	if (access(cmd, X_OK) == 0)
		return (cmd);
	while (envp[i] && ft_strncmp(envp[i], "PATH", 4) != 0)
		i++;
	path = ft_split(envp[i] + 5, ':');
	i = 0;
	while (path[i])
	{
		temp = ft_strjoin(path[i], "/");
		good_path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(good_path, X_OK) == 0)
			return (free_tab(path), good_path);
		i++;
		free(good_path);
	}
	return (NULL);
}

void	execute(char *cmd, char **envp)
{
	char	**exec_cmd;
	char	*path;

	if (!cmd || !cmd[0])
		exit(2);
	exec_cmd = ft_split(cmd, ' ');
	path = get_path(exec_cmd[0], envp);
	if (execve(path, exec_cmd, envp) == -1)
	{
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": command not found\n", 2);
		g_exit_status = 127;
	}
}

char	*give_curr_cmd(t_shell *shell, int i)
{
	char	*str;
	t_token	*current;

	str = ft_calloc(1, 1);
	current = shell->token;
	while (current && i)
	{
		if (current->type == PIPE)
			i--;
		current = current->next;
	}
	while (current && current->type != PIPE)
	{
		if (current->type == WORD || current->type == CMD)
			str = ft_strjoin(str, current->str);
		if (current->next && current->next->type == WORD)
			str = ft_strjoin(str, " ");
		current = current->next;
	}
	return (str);
}

void	child_process(char *cmd, t_shell *shell, int proc, int i)
{
	pid_t	pid;
	int		status;
	int		sig;

	pid = fork();
	if (pid == -1)
		exit(0);
	if (pid == 0)
	{
		init_signals_child();
		close(shell->exec->p_fd[0]);
		if (shell->exec->prev_fd[proc] > 0)
			dup2(shell->exec->prev_fd[proc], STDIN_FILENO);
		if (i != shell->exec->nb_cmd[proc] - 1)
			dup2(shell->exec->p_fd[1], STDOUT_FILENO);
		else if (shell->exec->fd_out[proc] != 1)
			dup2(shell->exec->fd_out[proc], STDOUT_FILENO);
		close(shell->exec->p_fd[1]);
		execute(cmd, shell->data->new_env);
	}
	else
	{
		if (shell->exec->prev_fd[proc] > 0
			&& shell->exec->prev_fd[proc] != STDIN_FILENO)
			close(shell->exec->prev_fd[proc]);
		close(shell->exec->p_fd[1]);
		free(cmd);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			g_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			sig = WTERMSIG(status);
			if (sig == SIGQUIT)
			{
				ft_putendl_fd("Quit", STDERR_FILENO);
				g_exit_status = 131;
			}
			else if (sig == SIGINT)
			{
				write(STDOUT_FILENO, "\n", 1);
				g_exit_status = 128;
			}
		}
	}
}

// void	exec_hub(t_shell *shell)
// {
// 	initiate_exec(shell);
// 	count_process(shell);
// 	count_element(shell);
// 	if (shell->count->nb_redir_in > 0)
// 		if (open_infile(shell) == -1)
// 			return ;
// 	if (shell->count->nb_redir_out > 0 || shell->count->nb_append > 0)
// 		open_outfile(shell);
// 	if (shell->count->nb_heredoc > 0)
// 		here_doc_hub(shell);
// 	if (shell->count->nb_pipe == 0)
// 		execute_one_cmd(shell);
// 	else
// 		execute_pipe(shell);
// 	ft_free_exec(shell);
// }
