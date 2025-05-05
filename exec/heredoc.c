/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 13:36:54 by pribolzi          #+#    #+#             */
/*   Updated: 2025/05/05 13:39:33 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static void	here_doc_process(char *limiter, int len, int *p_fd)
{
	char	*line;

	while (1)
	{
		line = get_next_line(0);
		if (!line)
			exit_handling(0);
		if (!ft_strncmp(line, limiter, len) && line[len] == '\n')
		{
			free(line);
			close(p_fd[0]);
			close(p_fd[1]);
			exit(0);
		}
		ft_putstr_fd(line, p_fd[1]);
		free(line);
	}
}

static int	here_doc_hub(char *limiter)
{
	pid_t	pid;
	int		len;
	int		p_fd[2];

	if (pipe(p_fd) == -1)
		exit_handling(0);
	len = ft_strlen(limiter);
	pid = fork();
	if (pid == -1)
		exit_handling(0);
	if (pid == 0)
		here_doc_process(limiter, len, p_fd);
	close(p_fd[1]);
	wait(NULL);
	return (p_fd[0]);
}
