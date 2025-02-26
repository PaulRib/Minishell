/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 15:03:41 by pribolzi          #+#    #+#             */
/*   Updated: 2025/02/26 16:06:54 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int	is_builtin(char *built)
{
	if (!ft_strcmp(built, "echo"))
		return (1);
	if (!ft_strcmp(built, "cd"))
		return (1);
	if (!ft_strcmp(built, "pwd"))
		return (1);
	if (!ft_strcmp(built, "export"))
		return (1);
	if (!ft_strcmp(built, "unset"))
		return (1);
	if (!ft_strcmp(built, "env"))
		return (1);
	if (!ft_strcmp(built, "exit"))
		return (1);
	return (0);
}

int	main(void)
{
	char	*line;

	while (1)
	{
		line = readline("minishell$> ");
		if (!line)
		{
			ft_printf("\033[1;33mexit\033[0m\n");
			break ;
		}
		free(line);
	}
	return (0);
}
