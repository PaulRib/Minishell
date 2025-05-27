/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 16:40:32 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/26 20:02:56 by meel-war         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	ft_free_history(t_shell *shell)
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

static void	ft_free_data(t_shell *shell)
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

char	*safe_strdup(char *s, t_shell *shell)
{
	char	*dup;

	dup = ft_strdup(s);
	if (!dup)
		free_all(shell, 1);
	return (dup);
}

char	*safe_strjoin(char *s1, char *s2, t_shell *shell, int free_s1,
		int free_s2)
{
	char	*result;

	result = ft_strjoin(s1, s2);
	if (!result)
		free_all(shell, 1);
	if (free_s1 && s1)
		free(s1);
	if (free_s2 && s2)
		free(s2);
	return (result);
}
