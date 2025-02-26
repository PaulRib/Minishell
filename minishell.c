/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 15:03:41 by pribolzi          #+#    #+#             */
/*   Updated: 2025/02/26 16:47:29 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int	exit_handling(char *error)
{
	ft_putstr_fd(error, 2);
	garbagecollector;
	exit(1);
}

void pwd_handling(void)
{
	
}

int	is_builtin(char *built)
{
	if (!ft_strcmp(built, "echo"))
		return (fonction echo);
	if (!ft_strcmp(built, "cd"))
		return (fonction cd);
	if (!ft_strcmp(built, "pwd"))
		return (fonction pwd);
	if (!ft_strcmp(built, "export"))
		return (fonction export);
	if (!ft_strcmp(built, "unset"))
		return (fonction unset);
	if (!ft_strcmp(built, "env"))
		return (fonction env);
	if (!ft_strcmp(built, "exit"))
		return (exit_handling("Exit\n"));
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	char	*line;

	while (1)
	{
		line = readline("minishell$> ");
		if (!line)
		{
			printf("\033[1;33mexit\033[0m\n");
			break ;
		}
		free(line);
	}
	return (0);
}
