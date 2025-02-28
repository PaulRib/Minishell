/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 15:03:41 by pribolzi          #+#    #+#             */
/*   Updated: 2025/02/28 13:07:25 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int	exit_handling(char *error)
{
	ft_putstr_fd(error, 2);
	garbagecollector;
	exit(1);
}

int	is_builtin(char *built, t_list *shell)
{
	if (!ft_strcmp(built, "echo"))
		return (fonction echo);
	if (!ft_strcmp(built, "cd"))
		return (cd_handling(shell));
	if (!ft_strcmp(built, "pwd"))
		return (pwd_handling(shell));
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
	t_list	*shell;

	getpwd(shell->data->cur_dir, 50000);
	while (1)
	{
		line = readline(cur_dir);
		if (!line)
		{
			printf("\033[1;33mexit\033[0m\n");
			break ;
		}
		get_args(&shell, line);
		free(line);
	}
	return (0);
}
