#include "minishell.h"

void add_to_history(t_shell *shell, char *command)
{
	t_history *new;
	t_history *tmp;

	if (!command || !*command)
		return ;
	
		new = malloc(sizeof(t_history));
		if (!new)
			return ;
		new->command = ft_strdup(command);
		new->next = NULL;

		if(!shell->history)
		{
			shell->history = new;
			return ;
		}
		tmp = shell->history;
		while(tmp->next)
			tmp = tmp->next;
		tmp->next = new;
}

int history_handling(t_shell *hub)
{
	t_history *tmp;
	int i;

	tmp = hub->history;
	i = 1;
	while (tmp)
	{
		printf("%5d %s\n", i++, tmp->command);
		tmp = tmp->next;
	}
	return(0);
}