/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 13:12:19 by pribolzi          #+#    #+#             */
/*   Updated: 2025/03/21 16:06:36 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


// int	exit_handling(char *error)
// {
// 	ft_putstr_fd(error, 2);
// 	ft_free_all()
// 	exit(1);
// }

int	is_builtin(char *built, t_list *hub)
{
	if (!ft_strcmp(built, "echo"))
		return (echo(hub));
	//if (!ft_strcmp(built, "cd"))
		// return (fonction cd);
	if (!ft_strcmp(built, "pwd"))
		return (pwd_handling(hub));
	//if (!ft_strcmp(built, "export"))
		// return (fonction export);
	//if (!ft_strcmp(built, "unset"))
		// return (fonction unset);
	if (!ft_strcmp(built, "history"))
		return (history_handling(hub));
	if (!ft_strcmp(built, "env"))
		return (print_env(hub));
	if (!ft_strcmp(built, "exit"))
		return (exit_handling("Exit\n"));
	return (0);
}