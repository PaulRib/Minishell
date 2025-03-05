/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 13:12:19 by pribolzi          #+#    #+#             */
/*   Updated: 2025/03/05 16:01:32 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// void	cd_handling(t_list *shell)
// {
// 	cur_dir = ft_strjoin(shell->data->cur_dir, shell->str);
// 	chdir(shell->str);
// }

// int	exit_handling(char *error)
// {
// 	ft_putstr_fd(error, 2);
// 	ft_free_all()
// 	exit(1);
// }

int	is_builtin(char *built, t_list *hub)
{
	//if (!ft_strcmp(built, "echo"))
		// return (fonction echo);
	//if (!ft_strcmp(built, "cd"))
		// return (fonction cd);
	if (!ft_strcmp(built, "pwd"))
		return (pwd_handling(hub));
	//if (!ft_strcmp(built, "export"))
		// return (fonction export);
	//if (!ft_strcmp(built, "unset"))
		// return (fonction unset);
	if (!ft_strcmp(built, "env"))
		return (print_env(hub));
	if (!ft_strcmp(built, "exit"))
		return (exit_handling("Exit\n"));
	return (0);
}