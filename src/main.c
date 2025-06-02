/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 12:37:52 by pribolzi          #+#    #+#             */
/*   Updated: 2025/06/02 12:38:00 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int ac, char **av, char **env)
{
	t_shell	*shell;

	(void)ac;
	(void)av;
	shell = malloc(sizeof(t_shell));
	if (!shell)
		free_all(shell, 1);
	ft_memset(shell, 0, sizeof(t_shell));
	initiate_all(shell);
	env_exists(env, shell);
	shell_loop(shell);
	free_all(shell, 0);
	return (0);
}
