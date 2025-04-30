/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:01:22 by pribolzi          #+#    #+#             */
/*   Updated: 2025/04/30 15:24:01 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	count_element(t_shell *shell)
{
	t_token	*current;
	int		i;

	i = 0;
	current = shell->token;
	while (current->next)
	{
		if (current->type == PIPE && current->next->type != REDIR_IN &&
			current->next->type != HEREDOC)
		{
			shell->count->nb_pipe++;
			shell->exec->nb_cmd[i]++;
		}
		if (current->type == REDIR_IN)
			shell->count->nb_redir_in++;
		if (current->type == REDIR_OUT)
			shell->count->nb_redir_out++;
		if (current->type == APPEND)
			shell->count->nb_append++;
		if (current->type == HEREDOC)
			shell->count->nb_heredoc++;
		if (current->type == FILE_OUT && current->next->type == PIPE)
			i++;
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
		if (shell->exec->fd_out[i] != 1 && current->type == PIPE)
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
		if (current->type == PIPE && (current->next->type == REDIR_IN
			|| current->next->type == HEREDOC))
			i++;
		current = current->next;
	}
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

void	child_process(char *cmd, t_shell *shell, int *p_fd, int fd, int proc, int i)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		exit(0);
	if (pid == 0)
	{
		close(p_fd[0]);
		if (fd > 0)
			dup2(fd, STDIN_FILENO);
		// printf("FD IN : %d\n", shell->exec->fd_in[proc]);
		// printf("FD OUT : %d\n", shell->exec->fd_out[proc]);
		if (i != shell->exec->nb_cmd[proc] - 1)
			dup2(p_fd[1], STDOUT_FILENO);
		else if (shell->exec->fd_out[proc] != 1)
			dup2(shell->exec->fd_out[proc], STDOUT_FILENO);
		close(p_fd[1]);
		execute(cmd, shell->data->new_env);
	}
	else
	{
		if (fd > 0 && fd != STDIN_FILENO)
			close(fd);
		close(p_fd[1]);
		free(cmd);
		waitpid(pid, NULL, 0);
	}
}

void execute_pipe(t_shell *shell)
{
	int		p_fd[2];
	int		i;
	int		prev_fd;
	int		proc;
	int		cmd;

	cmd = 0;
	proc = 0;
	while (proc < shell->exec->process)
	{
		prev_fd = shell->exec->fd_in[proc];
		i = 0;
		while (i < shell->exec->nb_cmd[proc])
		{
			if (pipe(p_fd) == -1)
				exit(0);
			child_process(give_curr_cmd(shell, cmd), shell, p_fd, prev_fd, proc, i);
			if (prev_fd > 0)
				close(prev_fd);
			close(p_fd[1]);
			prev_fd = p_fd[0];
			i++;
			cmd++;
		}
		printf("%d ", proc);
		printf("%d\n", shell->exec->nb_cmd[proc]);
		proc++;
	}
}

void	count_process(t_shell *shell)
{
	t_token *current;

	current = shell->token;
	shell->exec->process = 1;
	while (current->next)
	{
		if (current->type == PIPE && (current->next->type == REDIR_IN
			|| current->next->type == HEREDOC))
			shell->exec->process++;
		current = current->next;
	}
	printf("Proc : %d\n", shell->exec->process);
}

void	exec_hub(t_shell *shell)
{
	shell->exec = malloc(sizeof(t_exec));
	ft_memset(shell->exec, 0, sizeof(t_exec));
	count_process(shell);
	shell->exec->fd_in = malloc(sizeof(int) * shell->exec->process);
	shell->exec->fd_out = malloc(sizeof(int) * shell->exec->process);
	shell->exec->nb_cmd = malloc(sizeof(int) * shell->exec->process);
	int i = 0;
	while (i < shell->exec->process)
	{
		shell->exec->fd_in[i] = 0;
		shell->exec->fd_out[i] = 1;
		shell->exec->nb_cmd[i] = 1;
		i++;
	}
	count_element(shell);
	if (shell->count->nb_heredoc > 0)
		stock_all_eof(shell);
	if (shell->count->nb_redir_in > 0)
		open_infile(shell);
	if (shell->count->nb_redir_out > 0 || shell->count->nb_append > 0)
		open_outfile(shell);
	execute_pipe(shell);
}
