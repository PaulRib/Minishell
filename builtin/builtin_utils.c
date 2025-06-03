/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 16:14:28 by pribolzi          #+#    #+#             */
/*   Updated: 2025/06/03 17:09:37 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_many_args(t_token *token_ptr)
{
	if (token_ptr->type == PIPE || token_ptr->next->type == PIPE)
		return (0);
	if (token_ptr->type == REDIR_IN || token_ptr->type == REDIR_OUT)
		return (0);
	if (token_ptr->next->type == REDIR_IN || token_ptr->next->type == REDIR_OUT)
		return (0);
	if (token_ptr->type == APPEND || token_ptr->next->type == APPEND)
		return (0);
	if (token_ptr->type == HEREDOC || token_ptr->next->type == HEREDOC)
		return (0);
	return (1);
}

char	**build_env(t_shell *shell)
{
	char	**new_env;
	char	*temp;
	char	pwd[PATH_MAX];

	new_env = malloc(4 * sizeof(char *));
	if (!new_env)
		free_all(shell, 1);
	temp = ft_itoa(shell->data->shlvl);
	if (!temp)
	{
		free(new_env);
		free_all(shell, 1);
	}
	new_env[1] = ft_strjoin("SHLVL=", temp);
	free(temp);
	if (!getcwd(pwd, PATH_MAX))
		free_all(shell, 1);
	new_env[0] = ft_strjoin("PWD=", pwd);
	new_env[2] = ft_strdup("_=/usr/bin/env");
	if (!new_env[0] || !new_env[1] || !new_env[2])
	{
		free_tab(new_env);
		free_all(shell, 1);
	}
	return (new_env[3] = NULL, new_env);
}
