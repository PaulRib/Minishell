/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:38:15 by meel-war          #+#    #+#             */
/*   Updated: 2025/04/16 16:52:41 by meel-war         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_export(t_shell *shell, t_token *token_ptr)
{
	if (ft_strcmp(token_ptr->str, "export") != 0)
		return (-1);
	if (token_ptr && !token_ptr->next)
	{
		if (!export_no_args(shell))
			return (-1);
		return (0);
	}
	else
	return(0);
	// if (token_ptr->next)
	// 	return (ft_cd(shell->data, token_ptr->next->str));
	// else
	// 	return (ft_cd(shell->data, NULL));
}