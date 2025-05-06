/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 13:59:17 by pribolzi          #+#    #+#             */
/*   Updated: 2025/01/10 17:10:22 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE  1
# endif

typedef struct t_struct
{
	char	buffer[BUFFER_SIZE];
	ssize_t	bytes_read;
	ssize_t	pos;
}	t_struct;

char	*get_next_line(int fd);
char	*ft_strjoin_gnl(char *line, char *buff);
size_t	ft_strlen_gnl(char *str);
char	*ft_substr_gnl(char *buff, size_t start);
char	*return_value(t_struct *b, char *line);
int		check_error(t_struct *b, char *line);
ssize_t	check_line(char *line);
ssize_t	check_pos(char *buffer, size_t pos);
char	*check_after_line(t_struct *b, char *line);
char	*read_line(int fd, t_struct *b, char *line);

#endif