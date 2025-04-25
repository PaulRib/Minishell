/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 12:58:09 by meel-war          #+#    #+#             */
/*   Updated: 2025/04/25 14:42:10 by meel-war         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void handle_sigint()
{
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	// g_exit_status = 130;
}

static void	handle_sigquit()
{
	rl_on_new_line();
	rl_redisplay();
}

void init_signals(void)
{
	struct sigaction sa_int;
	struct sigaction sa_quit;

	sa_int.sa_handler = handle_sigint;
	sigaction(SIGINT, &sa_int, NULL);

	sa_quit.sa_handler = handle_sigquit;
	sigaction(SIGQUIT, &sa_quit, NULL);
}