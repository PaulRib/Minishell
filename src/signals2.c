/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 21:46:14 by meel-war          #+#    #+#             */
/*   Updated: 2025/05/22 21:51:04 by meel-war         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	init_signals_cmd(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	enable_echoctl();
	sa_int.sa_handler = handle_sigint_cmd;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = handle_sigquit_cmd;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

void	handle_sigint_cmd(int sig)
{
	(void)sig;
	write(1, "\n", 1);
}

void	handle_sigquit_cmd(int sig)
{
	(void)sig;
	write(1, "Quit\n", 5);
}

void	signal_block(void)
{
	struct sigaction sa_ignore;

	sa_ignore.sa_handler = SIG_IGN;
	sigemptyset(&sa_ignore.sa_mask);
	sa_ignore.sa_flags = 0;
	sigaction(SIGQUIT, &sa_ignore, NULL);
	sigaction(SIGINT, &sa_ignore, NULL);
}