/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:01:22 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/20 14:34:12 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// char	*get_path(char *cmd, char **envp)
// {
// 	int		i;
// 	char	**path;
// 	char	*good_path;
// 	char	*temp;

// 	i = 0;
// 	if (access(cmd, X_OK) == 0)
// 		return (cmd);
// 	while (envp[i] && ft_strncmp(envp[i], "PATH", 4) != 0)
// 		i++;
// 	path = ft_split(envp[i] + 5, ':');
// 	i = 0;
// 	while (path[i])
// 	{
// 		temp = ft_strjoin(path[i], "/");
// 		good_path = ft_strjoin(temp, cmd);
// 		free(temp);
// 		if (access(good_path, X_OK) == 0)
// 			return (free_tab(path), good_path);
// 		i++;
// 		free(good_path);
// 	}
// 	return (NULL);
// }

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
