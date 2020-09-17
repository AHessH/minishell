/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: froxanne <froxanne@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/14 17:13:01 by imicah            #+#    #+#             */
/*   Updated: 2020/09/17 18:11:56 by froxanne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int		create_fd(t_arguments *args)
{
	int i;
	int	fd;

	i = 0;
	fd = 1;
	while (args->parameters[i])
	{
		if (!ft_strncmp(">>", args->parameters[i], 2))
			fd = open(args->parameters[++i], O_CREAT | O_APPEND | O_RDWR, 0777);
		else if (!ft_strncmp(">", args->parameters[i], 1))
			fd = open(args->parameters[++i], O_CREAT | O_TRUNC | O_RDWR, 0777);
		else if (!ft_strncmp("<", args->parameters[i], 1))
			i++;
		if (args->parameters[i])
			i++;
	}
	return (fd);
}

char			**rebuild_args(t_arguments *arguments)
{
	char **new;
	int i;
	int j;

	i = -1;
	j = 0;
	while (arguments->parameters[++i])
		if (*arguments->parameters[i] == '>'
				|| *arguments->parameters[i] == '<')
			i++;
		else
			j++;
	new = (char **)malloc(sizeof(char *) * (j + 1));
	i = -1;
	j = 0;
	while (arguments->parameters[++i])
		if (*arguments->parameters[i] == '>'
				|| *arguments->parameters[i] == '<')
			i++;
		else
			new[j++] = ft_strdup(arguments->parameters[i]);
	arguments->command = new[0];
	// free_double_array(arguments->parameters);
	new[j] = 0;
	return (new);
}

int				echo(t_arguments *arguments, t_list *env_list)
{
	int		i;
	int		flag;
	int		fd;

	flag = 1;
	i = 1;
	fd = 1;
	if (!ft_strcmp(arguments->parameters[1], "-n"))
	{
		i++;
		flag = 0;
	}
	fd = create_fd(arguments);
	printf("fd = %d str = %s\n", fd, arguments->parameters[1]);
	// arguments->parameters = rebuild_args(arguments);
	while (arguments->parameters[i])
	{
		ft_putstr_fd(arguments->parameters[i++], fd);
		if (arguments->parameters[i])
			write(fd, " ", 1);
	}
	write(fd, "\n", (size_t)flag);
	close(fd);
	return (errno);
}
