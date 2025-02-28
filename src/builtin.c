/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 13:12:19 by pribolzi          #+#    #+#             */
/*   Updated: 2025/02/28 13:12:57 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	cd_handling(t_list *shell)
{
	cur_dir = ft_strjoin(shell->data->cur_dir, shell->str);
	chdir(shell->str);
}

void	pwd_handling(t_list *shell)
{
	printf("%s\n", shell->data->cur_dir);
}