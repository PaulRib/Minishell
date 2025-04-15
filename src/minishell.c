/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 15:03:41 by pribolzi          #+#    #+#             */
/*   Updated: 2025/04/15 14:51:07 by meel-war         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	initiate_all(t_shell *shell)
{
	shell->data = malloc(sizeof(t_data));
	shell->history = NULL;
	shell->number = malloc(sizeof(t_count));
	ft_memset(shell->number, 0, sizeof(t_count));
	ft_memset(shell->data, 0, sizeof(t_data));
	ft_memset(shell->history, 0, sizeof(t_history));
	getcwd(shell->data->old_dir, 500);
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
	handling_quotes(shell);
	ft_split_word(shell);
	second_token(shell);
	associate_options_commands(shell);
	// while (tmp)
	// {
	// 	printf("\033[34;01mType de token : \033[00m-%i-\n", tmp->type);
	// 	printf("\033[34;01mContenu du token : \033[00m-%s-\n", tmp->str);
	// 	tmp = tmp->next;
	// }
}

void	ft_free_node(t_shell *shell)
{
	t_token	*tmp;

	if (!shell || !shell->token)
		return ;
	while (shell->token)
	{
		tmp = shell->token->next;
		if (shell->token->str)
			free(shell->token->str);
		free(shell->token);
		shell->token = tmp;
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
		shell->prompt = ft_strjoin(shell->data->cur_dir, "$ ");
		line = readline(shell->prompt);
		if (!line)
		{
			printf("\033[1;33mexit\033[0m\n");
			break ;
		}
		if (*line)
		{
			shell->token = malloc(sizeof(t_token));
			ft_memset(shell->token, 0, sizeof(t_token));
			add_to_history(shell, line);
		}
		ft_hub_parsing(shell, line);
		is_builtin(shell, shell->token);
		free(line);
		ft_free_node(shell);
	}
	free_tab(env);
	return (0);
}
