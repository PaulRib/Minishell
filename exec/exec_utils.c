/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 16:01:44 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/13 18:17:57 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	count_element(t_shell *shell)
{
	t_token	*current;
	int		i;

	i = 0;
	current = shell->token;
	while (current->next)
	{
		if (current->type == PIPE && current->next->type != REDIR_IN
			&& current->next->type != HEREDOC)
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

void initiate_exec(t_shell *shell) // shell->exec->process doit être déjà défini par count_process
{
    int i;

    shell->exec = malloc(sizeof(t_exec));
    if (!shell->exec)
        exit(1); // Ou gestion d'erreur plus propre
    ft_memset(shell->exec, 0, sizeof(t_exec));

    shell->exec->fd_in = malloc(sizeof(int) * shell->exec->process);
    shell->exec->fd_out = malloc(sizeof(int) * shell->exec->process);
    shell->exec->nb_cmd = malloc(sizeof(int) * shell->exec->process);
    // Assurez-vous que prev_fd est aussi alloué pour shell->exec->process éléments de type int
    shell->exec->prev_fd = malloc(sizeof(int) * shell->exec->process);

    if (!shell->exec->fd_in || !shell->exec->fd_out || 
        !shell->exec->nb_cmd || !shell->exec->prev_fd)
        exit(1); // Ou gestion d'erreur

    i = 0;
    while (i < shell->exec->process)
    {
        shell->exec->prev_fd[i] = STDIN_FILENO;  // Initialiser à l'entrée standard
        shell->exec->fd_in[i] = STDIN_FILENO;   // Initialiser à l'entrée standard
        shell->exec->fd_out[i] = STDOUT_FILENO; // Initialiser à la sortie standard
        shell->exec->nb_cmd[i] = 1; // Supposons 1 commande par segment pour l'instant
        i++;
    }
}


// void	initiate_exec(t_shell *shell)
// {
// 	shell->exec = malloc(sizeof(t_exec));
// 	ft_memset(shell->exec, 0, sizeof(t_exec));
// 	shell->exec->fd_in = malloc(sizeof(int) * shell->exec->process);
// 	shell->exec->fd_out = malloc(sizeof(int) * shell->exec->process);
// 	shell->exec->nb_cmd = malloc(sizeof(int) * shell->exec->process);
// 	shell->exec->prev_fd = malloc(sizeof(bool) * shell->exec->process);
// 	if (!shell->exec->fd_in || !shell->exec->fd_out || !shell->exec->nb_cmd
// 		|| !shell->exec->prev_fd)
// 		exit(0);
// }

// void	count_process(t_shell *shell)
// {
// 	t_token	*current;
// 	int		i;

// 	i = 0;
// 	current = shell->token;
// 	shell->exec->process = 1;
// 	while (current->next)
// 	{
// 		if (current->type == PIPE && (current->next->type == REDIR_IN
// 				|| current->next->type == HEREDOC))
// 			shell->exec->process++;
// 		current = current->next;
// 	}
// 	while (i < shell->exec->process)
// 	{
// 		shell->exec->prev_fd[i] = 0;
// 		shell->exec->fd_in[i] = 0;
// 		shell->exec->fd_out[i] = 1;
// 		shell->exec->nb_cmd[i] = 1;
// 		i++;
// 	}
// }

void count_process(t_shell *shell)
{
    t_token *current;

    shell->exec->process = 1; // Il y a toujours au moins un segment
    current = shell->token;
    while (current)
    {
        if (current->type == PIPE)
        {
            shell->exec->process++;
        }
        current = current->next;
    }
    // L'initialisation des tableaux fd_in, fd_out, etc., sera faite dans initiate_exec
}


void	ft_free_exec(t_shell *shell)
{
	if (shell->exec)
	{
		if (shell->exec->nb_cmd)
			free(shell->exec->nb_cmd);
		if (shell->exec->fd_in)
			free(shell->exec->fd_in);
		if (shell->exec->fd_out)
			free(shell->exec->fd_out);
		if (shell->exec->prev_fd)
			free(shell->exec->prev_fd);
		close_fd_exec(shell);
		shell->exec = NULL;
	}
}

void execute_fork()
{
	
}