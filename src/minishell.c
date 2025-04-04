/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 15:03:41 by pribolzi          #+#    #+#             */
/*   Updated: 2025/03/21 16:02:38 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	initiate_all(t_shell *shell)
{
	shell->data = malloc(sizeof(t_data));
	shell->token = malloc(sizeof(t_token));
	shell->number = malloc(sizeof(t_count));
	ft_memset(shell->number, 0, sizeof(t_count));
	getcwd(shell->data->cur_dir, 500);
	shell->data->shlvl = 1;
}

void	ft_hub_parsing(t_shell *shell, char *line)
{
	t_token	*tmp;

	tmp = shell->token;
	count_element(shell, line);
	ft_minisplit(line, shell);
	get_type(shell);
	ft_split_word(shell);
	tmp = shell->token;
	second_token(shell);
	associate_options_commands(shell);
	while (tmp->next)
	{
		printf("Type de token : -%i-\n", tmp->type);
		printf("Contenu du token : -%s-\n", tmp->str);
		tmp = tmp->next;
	}
}

int	main(int ac, char **av, char **env)
{
	char	*line;
	t_shell	*shell;

	(void)ac;
	(void)av;
	shell = malloc(sizeof(t_shell));
	initiate_all(shell);
	if (!env || !env[0])
		shell->data->new_env = build_env(shell->data);
	else
		shell->data->new_env = copy_env(env, shell->data);
	while (1)
	{
		line = readline("minishell$>"); // cur_dir a inserer ici
		if (!line)
		{
			printf("\033[1;33mexit\033[0m\n");
			break ;
		}
		if(*line)
		{
			// add_history(line);
			add_to_history(shell, line);
		}
		ft_hub_parsing(shell, line);
		// is_builtin(line, hub);
		free(line);
	}
	free_tab(env);
	return (0);
}
