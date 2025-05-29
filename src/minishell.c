/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 12:12:19 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/29 15:04:28 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		g_exit_status = 0;

void	initiate_all(t_shell *shell)
{
	shell->data = malloc(sizeof(t_data));
	shell->count = malloc(sizeof(t_count));
	if (!shell->data || !shell->count)
		free_all(shell, 1);
	ft_memset(shell->count, 0, sizeof(t_count));
	shell->history = NULL;
	ft_memset(shell->data, 0, sizeof(t_data));
	ft_memset(shell->history, 0, sizeof(t_history));
	getcwd(shell->data->old_dir, PATH_MAX);
	getcwd(shell->data->cur_dir, PATH_MAX);
	shell->data->shlvl = 1;
	shell->exit_status = 0;
	init_signals();
}

void	ft_hub_parsing(t_shell *shell, char *line)
{
	shell->token->str = line;
	shell->token->type = WORD;
	if (handling_quotes(shell) == -1)
		return ;
	expand_all_tokens(shell);
	join_quote(shell);
	ft_split_word(shell);
	get_type(shell);
	second_token(shell);
	count_process(shell);
	initiate_exec(shell);
	count_element(shell);
	if (syntax_hub(shell) == -1)
		return ;
	if (!check_one_builtin(shell))
		execute_parsed_line(shell);
	ft_free_exec(shell);
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
		shell->token = NULL;
		shell->token = tmp;
	}
}

void	shell_loop(t_shell *shell)
{
	char	*line;

	while (1)
	{
		shell->prompt = ft_strjoin(shell->data->cur_dir, "$ ");
		line = readline(shell->prompt);
		free(shell->prompt);
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		if (*line && line[0] != '\0')
		{
			shell->token = malloc(sizeof(t_token));
			ft_memset(shell->token, 0, sizeof(t_token));
			add_history(line);
			add_to_history(shell, line);
			handle_sigint_status(shell);
			ft_hub_parsing(shell, line);
		}
		ft_free_node(shell);
		ft_memset(shell->count, 0, sizeof(t_count));
	}
}

int	main(int ac, char **av, char **env)
{
	t_shell	*shell;

	(void)ac;
	(void)av;
	shell = malloc(sizeof(t_shell));
	if (!shell)
		free_all(shell, 1);
	ft_memset(shell, 0, sizeof(t_shell));
	initiate_all(shell);
	env_exists(env, shell);
	shell_loop(shell);
	free_all(shell, 0);
	return (0);
}
