/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exec_errors.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <dmalacov@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/13 12:13:15 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/10/11 17:33:42 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <unistd.h>
#include "executor.h"
#include "libft.h"

/* 
	A function that frees the memory allocated for a 2D char array.
 */
void	ex_free_array(char **array)
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

/* 
	This function frees all memory allocated for the struct tools.
 */
void	ex_cleanup_tools(t_cmd_tools **tools)
{
	t_heredoc	*node;
	t_heredoc	*to_free;

	if (tools)
	{
		ex_free_array((*tools)->paths);
		ex_free_array((*tools)->env_var);
		free((*tools)->cmd_args);
		node = (*tools)->heredoc;
		while (node)
		{
			to_free = node;
			node = node->next;
			free(to_free);
		}
		free(*tools);
	}
}

/* 
	This function iterates over the characters in a string and checks whether 
	the string contains the character '/'. It then returns either 1 (TRUE) or 
	0 (FALSE).
 */
int	ex_contains_char(char *str, char c)
{
	while (*str)
	{
		if ((*str) == c)
			return (TRUE);
		str++;
	}
	return (FALSE);
}

/* 
	Prints an error message based on the error no. in the stderr and returns an 
	exit code corresponding to the error no.
 */
int	ex_print_error_message(int error_no, char *name)
{
	int	exit_code;

	exit_code = error_no;
	if (error_no != 12)
		ft_putstr_fd("mish: ", 2);
	if (name)
	{
		ft_putstr_fd(name, 2);
		write(STDERR_FILENO, ": ", 2);
	}
	if (error_no == CMD_ERROR && ex_contains_char(name, '/') == FALSE)
		ft_putstr_fd("Command not found", 2);
	else if (error_no == CMD_ERROR)
		ft_putstr_fd(strerror(2), 2);
	else if (error_no == REDIR_ERROR)
		ft_putstr_fd ("Ambiguous redirect", STDERR_FILENO);
	else
		ft_putstr_fd(strerror(error_no), 2);
	write(STDERR_FILENO, "\n", 1);
	return (exit_code);
}

/* 
	The function manages exit of a child process, including printing of an error 
	message and freeing allocated memory. It exits with the correct exit code.
 */
void	ex_child_error_and_exit(int error_code, t_cmd_tools *tools, \
char *name)
{
	int	exit_code;

	if ((error_code > 0 && name) || error_code == CMD_ERROR || error_code == \
	REDIR_ERROR)
		exit_code = ex_print_error_message(error_code, name);
	ex_cleanup_tools(&tools);
	if (error_code == 13 || error_code == 21)
		exit_code = 126;
	if (error_code == CMD_ERROR)
		exit_code = 127;
	if (error_code < CMD_ERROR)
		exit_code = 1;
	exit(exit_code);
}
