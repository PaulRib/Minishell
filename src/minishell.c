/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 12:12:19 by pribolzi          #+#    #+#             */
/*   Updated: 2025/06/05 16:48:46 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		g_exit_status = 0;

void verify_space(t_shell *shell)
{
	t_token *current;
	int		len;

	current = shell->token;
	while (current)
	{
		if (current->last_space == false)
		{
			len = ft_strlen(current->str);
			if (len > 0)
			{
				if (current->str[len - 1] == ' ' || current->str[len - 1] == '\t')
					current->last_space = true;
			}
		}
		current = current->next;
	}
}

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
	verify_space(shell);
	ft_split_word(shell);
	get_type(shell);
	expand_all_tokens(shell);
	join_quote(shell);
	second_token(shell);
	if (syntax_hub(shell) == -1)
		return ;
	count_process(shell);
	initiate_exec(shell);
	count_element(shell);
	if (!check_one_builtin(shell))
		execute_parsed_line(shell);
	ft_free_exec(shell);
	ft_memset(shell->count, 0, sizeof(t_count));
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

static int line_empty(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] != ' ' && line[i] != '\t')
			return (0);
		i++;
	}
	return (1);
}

void	shell_loop(t_shell *shell)
{
	char	*line;

	while (1)
	{
		shell->prompt = safe_strjoin(shell->data->cur_dir, "$ ", shell, 0);
		line = readline(shell->prompt);
		free(shell->prompt);
		if (!line)
		{
			printf("exit\n");
			free_all(shell, shell->exit_status);
		}
		if (!line_empty(line))
		{
			shell->token = malloc(sizeof(t_token));
			ft_memset(shell->token, 0, sizeof(t_token));
			add_history(line);
			add_to_history(shell, line);
			handle_sigint_status(shell);
			ft_hub_parsing(shell, line);
			ft_free_node(shell);
		}
		else
			free(line);
	}
}
