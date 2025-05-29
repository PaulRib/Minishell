/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:01:33 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/29 15:45:38 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_pwd(t_shell *shell)
{
	printf("%s\n", shell->data->cur_dir);
	return (0);
}

int	check_pwd(t_shell *shell, t_token *token_ptr)
{
	(void)shell;
	if (ft_strcmp(token_ptr->str, "pwd") != 0)
		return (-1);
	return (ft_pwd(shell));
}
