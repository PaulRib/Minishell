/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 13:12:19 by pribolzi          #+#    #+#             */
/*   Updated: 2025/03/05 13:26:28 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../includes/minishell.h"

// void	cd_handling(t_list *shell)
// {
// 	cur_dir = ft_strjoin(shell->data->cur_dir, shell->str);
// 	chdir(shell->str);
// }

// void	pwd_handling(t_list *shell)
// {
// 	printf("%s\n", shell->data->cur_dir);
// }

// int	is_builtin(char *built)
// {
// 	if (!ft_strcmp(built, "echo"))
// 		// return (fonction echo);
// 	if (!ft_strcmp(built, "cd"))
// 		// return (fonction cd);
// 	if (!ft_strcmp(built, "pwd"))
// 		// return (fonction pwd);
// 	if (!ft_strcmp(built, "export"))
// 		// return (fonction export);
// 	if (!ft_strcmp(built, "unset"))
// 		// return (fonction unset);
// 	if (!ft_strcmp(built, "env"))
// 		// return (fonction env);
// 	if (!ft_strcmp(built, "exit"))
// 		return (exit_handling("Exit\n"));
// 	return (0);
// }