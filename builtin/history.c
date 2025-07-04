/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 17:08:23 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/28 13:09:57 by meel-war         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	add_to_history(t_shell *shell, char *command)
{
	t_history	*new;
	t_history	*tmp;

	if (!command || !*command)
		return ;
	new = malloc(sizeof(t_history));
	if (!new)
		free_all(shell, 1);
	new->command = ft_strdup(command);
	if (!new->command)
	{
		free(new);
		free_all(shell, 1);
	}
	new->next = NULL;
	if (!shell->history)
	{
		shell->history = new;
		return ;
	}
	tmp = shell->history;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

int	ft_history(t_shell *hub)
{
	t_history	*tmp;
	int			i;

	tmp = hub->history;
	i = 1;
	while (tmp)
	{
		printf("%5d %s\n", i++, tmp->command);
		tmp = tmp->next;
	}
	return (1);
}
