/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 15:03:41 by pribolzi          #+#    #+#             */
/*   Updated: 2025/02/26 16:04:37 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

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
		if ()
		free(line);
	}
	return (0);
}
gehtbrngfngebf b