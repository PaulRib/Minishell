/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 12:12:19 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/10 18:04:20 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		g_exit_status = 0;

void	initiate_all(t_shell *shell)
{
	shell->data = malloc(sizeof(t_data));
	shell->count = malloc(sizeof(t_count));
	ft_memset(shell->count, 0, sizeof(t_count));
	shell->history = NULL;
	shell->count = malloc(sizeof(t_count));
	ft_memset(shell->count, 0, sizeof(t_count));
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
	t_token	*tmp;

	tmp = shell->token;
	shell->token->str = line;
	shell->token->type = WORD;
	handling_quotes(shell);
	ft_split_word(shell);
	get_type(shell);
	expand_all_tokens(shell);
	second_token(shell);
	associate_options_commands(shell);
	while (tmp)
	{
		printf("\033[34;01mType de token : \033[00m-%i-\n", tmp->type);
		printf("\033[34;01mContenu du token : \033[00m-%s-\n", tmp->str);
		tmp = tmp->next;
	}
	if (is_builtin(shell, shell->token) == -1)
		return ;
	exec_hub(shell);
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

void	shell_loop(t_shell *shell)
{
	char	*line;

	while (1)
	{
		g_exit_status = 0;
		shell->prompt = ft_strjoin(shell->data->cur_dir, "$ ");
		line = readline(shell->prompt);
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
			ft_hub_parsing(shell, line);
		}
		//free(line);
		ft_free_node(shell);
		shell->exit_status = g_exit_status;
		printf("%d\n", shell->exit_status);
		ft_memset(shell->count, 0, sizeof(t_count));
	}
}

int	main(int ac, char **av, char **env)
{
	t_shell	*shell;

	(void)ac;
	(void)av;
	shell = malloc(sizeof(t_shell));
	initiate_all(shell);
	env_exists(env, shell);
	shell_loop(shell);
	free_tab(env);
	return (0);
}
