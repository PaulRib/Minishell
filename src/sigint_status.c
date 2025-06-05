/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sigint_status.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 18:45:16 by meel-war          #+#    #+#             */
/*   Updated: 2025/06/04 14:07:20 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_sigint_status(t_shell *shell)
{
	if (g_exit_status == 130)
	{
		shell->exit_status = 130;
		g_exit_status = 0;
	}
}

void	enable_echoctl(void)
{
	struct termios	term;

	ft_memset(&term, 0, sizeof(struct termios));
	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag |= ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void	disable_echoctl(void)
{
	struct termios	term;

	ft_memset(&term, 0, sizeof(struct termios));
	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~(ECHOCTL);
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}
