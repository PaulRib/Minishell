/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 15:03:41 by pribolzi          #+#    #+#             */
/*   Updated: 2025/03/03 15:58:49 by meel-war         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// int	exit_handling(char *error)
// {
// 	ft_putstr_fd(error, 2);
// 	// garbagecollector;
// 	exit(1);
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
void	print_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		printf("%s\n", env[i]);
		i++;
	}
}

int	main(int ac, char **av, char **env)
{
	char	*line;
	t_data	data;
	pid_t	pid;
	int		status;

	(void)ac;
	(void)av;
	data.cur_dir = NULL;
	data.SHLVL = 1;
	if (!env || !env[0])
		data.new_env = build_env(data);
	else
		data.new_env = copy_env(env, data);
	while (1)
	{
		line = readline("minishell$>"); // cur_dir a inserer ici
		if (!line)
		{
			printf("\033[1;33mexit\033[0m\n");
			break ;
		}
		if (!ft_strcmp(line, "env"))
			print_env(data.new_env);
		else if (!ft_strcmp(line, "./minishell"))
		{
			pid = fork();
			if (pid == -1)
			{
				perror("fork");
				return (1);
			}
			if (pid == 0)
			{
				execve("./minishell", av, data.new_env);
				perror("execve");
				exit(EXIT_FAILURE);
			}
			else
			{
				waitpid(pid, &status, 0);
			}
		}
		free(line);
	}
	free_tab(env);
	return (0);
}
