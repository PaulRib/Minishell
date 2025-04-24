/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:01:22 by pribolzi          #+#    #+#             */
/*   Updated: 2025/04/24 17:07:33 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	count_element(t_shell *shell)
{
	t_token	*current;

	current = shell->token;
	while (current)
	{
		if (current->type == PIPE)
			shell->count->nb_pipe++;
		if (current->type == REDIR_IN)
			shell->count->nb_redir_in++;
		if (current->type == REDIR_OUT)
			shell->count->nb_redir_out++;
		if (current->type == APPEND)
			shell->count->nb_append++;
		if (current->type == HEREDOC)
			shell->count->nb_heredoc++;
		current = current->next;
	}
	shell->exec->nb_cmd = 1;
	current = shell->token;
	while (current)
	{
		if (current->type == PIPE)
			shell->exec->nb_cmd++;
		current = current->next;
	}
}

static void	stock_all_eof(t_shell *shell)
{
	t_token	*current;
	int		i;

	i = 0;
	current = shell->token;
	shell->exec->eof_heredoc = malloc(sizeof(char *)
			* (shell->count->nb_heredoc) + 1);
	while (current)
	{
		if (current->type == END)
			shell->exec->eof_heredoc[i++] = current->str;
		current = current->next;
	}
	shell->exec->eof_heredoc[i] = NULL;
}

static void	open_outfile(t_shell *shell)
{
	int i;
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
				shell->exec->fd_out[i] = open(current->str, O_WRONLY | O_CREAT | O_TRUNC, 0777);
			else if (current->prev->type == APPEND)
				shell->exec->fd_out[i] = open(current->str, O_WRONLY | O_CREAT | O_APPEND, 0777);
			if (shell->exec->fd_out[i] == -1)
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(current->str, 2);
				ft_putstr_fd(": No such file or directory", 2);
			}
		}
		else if (current->type == PIPE)
			i++;
		current = current->next;
	}
}

char	*get_path(char *cmd, char **envp, char **exec_cmd)
{
	int		i;
	char	**path;
	char	*good_path;
	char	*temp;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH", 4) != 0)
		i++;
	path = ft_split(envp[i] + 5, ':');
	i = 0;
	while (path[i])
	{
		temp = ft_strjoin(path[i], "/");
		good_path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(good_path, F_OK) == 0)
			return (free_tab(path), good_path);
		i++;
		free(good_path);
	}
	free_tab(exec_cmd);
	free_tab(path);
	exit(3);
	return (NULL);
}

void	execute(char *cmd, char **envp)
{
	char	**exec_cmd;
	char	*path;

	if (!cmd || !cmd[0])
		exit(2);
	exec_cmd = ft_split(cmd, ' ');
	path = get_path(exec_cmd[0], envp, exec_cmd);
	if (execve(path, exec_cmd, envp) == -1)
	{
		free_tab(exec_cmd);
		free(path);
		exit(0);
	}
}

static void	open_infile(t_shell *shell)
{
	int i;
	t_token	*current;

	current = shell->token;
	i = 0;
	while (current)
	{
		if (current->type  != PIPE)
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
				}
				else if (current->type == HEREDOC)
				{
					if (shell->exec->fd_in[i])
					{
						close (shell->exec->fd_in[i]);
						shell->exec->fd_in[i] = 0;
					}
				}
			}
		}
		else if (current->type == PIPE)
			i++;
		current = current->next;
	}
}

void	child_process(char *cmd, char **envp, t_exec *exec, int *p_fd, int i)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		exit(0);
	if (pid == 0)
	{
		close(p_fd[0]);
		// dprintf(2, "%d\n", exec->fd_in[i]);
		// dprintf(2, "%s\n", cmd);
		// if (dup2(exec->fd_in[i], STDIN_FILENO) == -1)
		// 	exit(0);
		// if (dup2(1, STDOUT_FILENO) == -1)
		// 	exit(0);
		execute(cmd, envp);
	}
	wait(NULL);
}

void	parent_process(char *cmd, char **envp, int *p_fd)
{
	if (dup2(p_fd[0], STDIN_FILENO) == -1)
		exit(0);
	close(p_fd[0]);
	execute(cmd, envp);
}

char *give_curr_cmd(t_shell *shell, int i)
{
	char *str;

	str = ft_calloc(1, 1);
	t_token *current;

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

void execute_pipe(t_shell *shell)
{
	int		p_fd[2];
	int i;

	i = 0;

	while (i < shell->exec->nb_cmd)
	{
		if (pipe(p_fd) == -1)
			exit(0);
		child_process(give_curr_cmd(shell, i), shell->data->new_env, shell->exec, p_fd, i);
		close(p_fd[1]);
		i++;
	}
}

void	exec_hub(t_shell *shell)
{
	shell->exec = malloc(sizeof(t_exec));
	ft_memset(shell->exec, 0, sizeof(t_exec));
	count_element(shell);
	shell->exec->fd_in = malloc(sizeof(int) * shell->exec->nb_cmd);
	shell->exec->fd_out = malloc(sizeof(int) * shell->exec->nb_cmd);
	ft_memset(shell->exec->fd_in, 0, sizeof(int) * shell->exec->nb_cmd);
	ft_memset(shell->exec->fd_out, 1, sizeof(int) * shell->exec->nb_cmd);
	if (shell->count->nb_heredoc > 1)
		stock_all_eof(shell);
	if (shell->count->nb_redir_in > 0)
		open_infile(shell);
	if (shell->count->nb_redir_out > 0 || shell->count->nb_append > 0)
		open_outfile(shell);
	execute_pipe(shell);
}
