/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pribolzi <pribolzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 16:03:35 by pribolzi          #+#    #+#             */
/*   Updated: 2025/03/05 16:33:37 by pribolzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	cd_handling(t_list *hub)
{
	if (ft_strncmp(hub->str, "~/", 2) == 0)
	{
		erase_all(hub);
		hub->data->cur_dir = malloc(sizeof(char) * (ft_strlen(hub->str)));
		hub->data->cur_dir = ft_strjoin(hub->data->cur_dir, hub->str);
	}
	if (ft_strncmp(hub->str, "../", 3) == 0)
	{
		going_back(hub);
		hub->data->cur_dir = ft_strjoin(hub->data->cur_dir, hub->str);
	}
	else if (ft_strncmp(hub->str, "..", 2) == 0)
		going_back(hub)
	else
		hub->data->cur_dir = ft_strjoin(hub->data->cur_dir, hub->str);
	chdir(hub->str);
}

void	going_back(t_list *hub)
{
	int		i;

	i = 0;
	while (hub->data->cur_dir[i])
		i++;
	while (hub->data->cur_dir[i] != "/")
		i--;
	hub->data->cur_dir[i] = '\0';
}

void	erase_all(t_list *hub)
{
	free(hub->data->cur_dir);
	hub->data->cur_dir[0] = '/0';
}