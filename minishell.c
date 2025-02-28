/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meel-war <meel-war@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 15:03:41 by pribolzi          #+#    #+#             */
/*   Updated: 2025/02/28 12:34:39 by meel-war         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

// int copy_env(t_list **list, char **env)
// {
// 	int		i;
// 	t_env	*tmp;

// 	if(!(*env))
// 		return(build_env(list));
// 	i = 0;
// 	*list = NULL;
// 	while(env[i])
// 	{
// 		tmp = malloc(sizeof (t_env));
// 		if(!tmp)
// 			return (free_lst(list));
// 		tmp->str = ft_strdup(env[i]);
// 		if(!tmp->str)
// 		{
// 			free(tmp);
// 			return(free_lst(list));
// 		}
// 		tmp->next = NULL;
// 		tmp->prev = NULL;
// 		ft_lstadd_back((t_env **)list, tmp); // OuaiPaul a dis c etais pas bon ...
// 		i++;
// 	}
// 	return(1);
// }

char **copy_env(char **env)
{
	int i = 0;
	int j = 0;
	char **new_env;

	if(!env || !env[0])
		return(build_env(env));
	while(env[i])
		i++;
	new_env = malloc((i + 1) * sizeof(char *));
	if(!new_env)
		return (NULL);
	while(j < i)
	{
		new_env[j] = ft_strdup(env[j]);
		if(new_env[j])
		{
			free_tab(new_env);
			return(NULL);
		}
		j++;
	}
	new_env[i] = NULL;
	return(new_env);
}
int	exit_handling(char *error)
{
	ft_putstr_fd(error, 2);
	garbagecollector;
	exit(1);
}

void pwd_handling(void)
{
	
}

int	is_builtin(char *built)
{
	if (!ft_strcmp(built, "echo"))
		return (fonction echo);
	if (!ft_strcmp(built, "cd"))
		return (fonction cd);
	if (!ft_strcmp(built, "pwd"))
		return (fonction pwd);
	if (!ft_strcmp(built, "export"))
		return (fonction export);
	if (!ft_strcmp(built, "unset"))
		return (fonction unset);
	if (!ft_strcmp(built, "env"))
		return (fonction env);
	if (!ft_strcmp(built, "exit"))
		return (exit_handling("Exit\n"));
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	char	*line;

	while (1)
	{
		line = readline("minishell$> ");
		if (!line)
		{
			printf("\033[1;33mexit\033[0m\n");
			break ;
		}
		free(line);
	}
	return (0);
}
