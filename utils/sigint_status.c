/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sigint_status.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 18:45:16 by meel-war          #+#    #+#             */
/*   Updated: 2025/05/21 18:46:06 by meel-war         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_sigint_status(t_shell *shell)
{
	if(g_exit_status == 130)
	{
		shell->exit_status = 130;
		g_exit_status = 0;
	}
}
