/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 15:03:41 by pribolzi          #+#    #+#             */
/*   Updated: 2025/03/12 17:06:17 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	initiate_all(t_shell **hub)
{
	*hub = malloc(sizeof(t_list));
	(*hub)->data = malloc(sizeof(t_data));
	// (*hub)->token = malloc(sizeof(t_token));
	(*hub)->data->cur_dir = malloc(500);
	getcwd((*hub)->data->cur_dir, 500);
	(*hub)->data->shlvl = 1;
}

int	main(int ac, char **av, char **env)
{
	char	*line;
	t_shell	*hub;
	// pid_t	pid;
	// int		status;

	(void)ac;
	(void)av;
	initiate_all(&hub);
	if (!env || !env[0])
		hub->data->new_env = build_env(hub->data);
	else
		hub->data->new_env = copy_env(env, hub->data);
	while (1)
	{
		line = readline("minishell$>"); // cur_dir a inserer ici
		if (!line)
		{
			printf("\033[1;33mexit\033[0m\n");
			break ;
		}
		// is_builtin(line, hub);
		free(line);
	}
	free_tab(env);
	return (0);
}
