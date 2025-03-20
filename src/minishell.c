/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 15:03:41 by pribolzi          #+#    #+#             */
/*   Updated: 2025/03/19 15:21:17 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	initiate_all(t_shell *shell)
{
	shell = malloc(sizeof(t_shell));
	shell->data = malloc(sizeof(t_data));
	shell->token = malloc(sizeof(t_token));
	getcwd((*hub)->data->cur_dir, 500);
	shell->data->shlvl = 1;
}

void ft_hub_parsing(t_token *token, char *line)
{
	
}

int	main(int ac, char **av, char **env)
{
	char	*line;
	t_shell	*shell;

	(void)ac;
	(void)av;
	initiate_all(shell);
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
