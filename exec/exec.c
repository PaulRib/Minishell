/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:01:22 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/06 19:03:09 by pribolzi         ###   ########.fr       */
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

static	void open_outfile(t_shell *shell)
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
		}
		if (shell->exec->fd_out[i] > 1 && current->type == PIPE)
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
	(void)exec_cmd;
	// free_tab(exec_cmd);
	// free_tab(path);
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
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": command not found\n", 2);
		g_exit_status = 127;
		free_tab(exec_cmd);
		free(path);
	}
}

static int	open_infile(t_shell *shell)
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
				return (-1);
			}
			if (current->type == HEREDOC)
				shell->exec->fd_in[i] = 0;
		}
		if (current->type == PIPE && (current->next->type == REDIR_IN
			|| current->next->type == HEREDOC))
			i++;
		current = current->next;
	}
	return (0);
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

void	child_process(char *cmd, t_shell *shell, int proc, int i)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		exit(0);
	if (pid == 0)
	{
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
		if (shell->exec->prev_fd[proc] > 0 && shell->exec->prev_fd[proc] != STDIN_FILENO)
			close(shell->exec->prev_fd[proc]);
		close(shell->exec->p_fd[1]);
		free(cmd);
		waitpid(pid, NULL, 0);
	}
}

void execute_pipe(t_shell *shell)
{
	int		i;
	int		proc;
	int		cmd;

	cmd = 0;
	proc = 0;
	while (proc < shell->exec->process)
	{
		if (shell->exec->prev_fd[proc] == 0)
			shell->exec->prev_fd[proc] = shell->exec->fd_in[proc];
		i = 0;
		while (i < shell->exec->nb_cmd[proc])
		{
			if (pipe(shell->exec->p_fd) == -1)
				exit(0);
			child_process(give_curr_cmd(shell, cmd), shell, proc, i);
			if (shell->exec->prev_fd[proc] > 0)
				close(shell->exec->prev_fd[proc]);
			close(shell->exec->p_fd[1]);
			shell->exec->prev_fd[proc] = shell->exec->p_fd[0];
			i++;
			cmd++;
		}
		proc++;
	}
}

void	count_process(t_shell *shell)
{
	t_token *current;
	int		i;

	i = 0;
	current = shell->token;
	shell->exec->process = 1;
	while (current->next)
	{
		if (current->type == PIPE && (current->next->type == REDIR_IN
			|| current->next->type == HEREDOC))
			shell->exec->process++;
		current = current->next;
	}
	while (i < shell->exec->process)
	{
		shell->exec->prev_fd[i] = 0;
		shell->exec->fd_in[i] = 0;
		shell->exec->fd_out[i] = 1;
		shell->exec->nb_cmd[i] = 1;
		//shell->heredoc->hrd[i] = false;
		i++;
	}
}

void initiate_exec(t_shell *shell)
{
	shell->exec = malloc(sizeof(t_exec));
	ft_memset(shell->exec, 0, sizeof(t_exec));
	shell->exec->fd_in = malloc(sizeof(int) * shell->exec->process);
	shell->exec->fd_out = malloc(sizeof(int) * shell->exec->process);
	shell->exec->nb_cmd = malloc(sizeof(int) * shell->exec->process);
	shell->exec->prev_fd = malloc(sizeof(bool) * shell->exec->process);
	if (!shell->exec->fd_in || !shell->exec->fd_out || !shell->exec->nb_cmd
		|| !shell->exec->prev_fd)
		exit(0);
}

void	exec_hub(t_shell *shell)
{
	initiate_exec(shell);
	count_process(shell);
	count_element(shell);
	if (shell->count->nb_redir_in > 0)
		if (open_infile(shell) == -1)
			return ;
	if (shell->count->nb_redir_out > 0 || shell->count->nb_append > 0)
		open_outfile(shell);
	if (shell->count->nb_heredoc > 0)
		here_doc_hub(shell);
	execute_pipe(shell);
}

void	free_exec(t_shell *shell)
{
	if (shell->exec)
	{
		if (shell->exec->nb_cmd)
			free(shell->exec->nb_cmd);
		if (shell->exec->fd_in)
			free(shell->exec->fd_in);
		if (shell->exec->fd_out)
			free(shell->exec->fd_out);
		if (shell->heredoc->eof_heredoc)
			free_tab(shell->heredoc->eof_heredoc);
		if (shell->exec->prev_fd)
			free(shell->exec->prev_fd);
		shell->exec = NULL;
	}
}
