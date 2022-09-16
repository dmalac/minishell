/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_errors.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <dmalacov@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/13 12:13:15 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/09/16 15:57:03 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <unistd.h>
#include "executor.h"
#include "libft.h"

void	free_array(char **array)
{
	size_t	i;

	i = 0;
	while (array && array[i])
	{
		free(array[i]);
		array[i++] = NULL;
	}
	free (array);
	array = NULL;
}

void	cleanup(t_cmd_tools *tools)
{
	t_heredoc	*node;
	t_heredoc	*to_free;

	if (tools)
	{
		free_array(tools->cmd_args);
		free_array(tools->paths);
		free_array(tools->env_var);
		node = tools->heredoc;
		while (node)
		{
			to_free = node;
			node = node->next;
			free(to_free);
		}
	}
}

int	print_error_message(int error_code, char *name)
{
	char	*error_message;	

	ft_putstr_fd("Error: ", 2);
	if (error_code == CMD_ERROR)
	{
		error_code = 127;
		ft_putstr_fd("Command not found", 2);
	}
	else
	{
		error_message = strerror(error_code);
		ft_putstr_fd(error_message, 2);
	}
	if (name)
	{
		write(2, ": ", 2);
		ft_putstr_fd(name, 2);
	}
	write(2, "\n", 1);
	return (error_code);
}

void	child_error_and_exit(int error_code, t_cmd_tools *tools, \
char *name)
{
	if (error_code > 0 || error_code == CMD_ERROR)
		error_code = print_error_message(error_code, name);
	cleanup(tools);
	if (error_code < 0)
		error_code = 1;
	if (error_code == 13)
		error_code = 126;
	exit(error_code);
}
