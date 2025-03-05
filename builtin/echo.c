static int args_nb(char **temp)
{
	int		count;

	count = 0;
	while(temp[count])
		count++;
	return(count);
}

int	echo_handling(char **temp)
{
	int i;
	int n_param;

	i = 1;
	n_param = 0;
	if(args_nb(temp) > 1)
	{
		while(temp[i] && ft_strcmp(temp[i], "-n") == 0)
		{
			n_param = 1;
			i++;
		}
		while(temp[i])
		{
			ft_putstr_fd(temp[i], 1);
			if(temp[i + 1])
				write(1, " ", 1);
			i++;
		}
	}
	if(n_param == 0)
	{
		write(1, "\n", 1);
	}
}