/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 16:40:32 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/22 15:13:10 by meel-war         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void ft_free_history(t_shell *shell)
{
	t_history	*tmp;

	rl_clear_history();
	while (shell->history)
	{
		if (shell->history->next)
			tmp = shell->history->next;
		else
			tmp = NULL;
		if (shell->history->command)
			free(shell->history->command);
		free(shell->history);
		shell->history = tmp;
	}
}

static void ft_free_data(t_shell *shell)
{
	if (shell->data->new_env)
		free_tab(shell->data->new_env);
	free(shell->data);
}

void	free_all(t_shell *shell, int status)
{
	if (shell)
	{
		if (shell->heredoc)
			ft_free_heredoc(shell);
		if (shell->exec)
			ft_free_exec(shell);
		if (shell->count)
			free(shell->count);
		if (shell->token)
			ft_free_node(shell);
		if (shell->history)
			ft_free_history(shell);
		if (shell->data)
			ft_free_data(shell);
	}
	exit(status);
}
