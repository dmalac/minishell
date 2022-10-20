/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_echo.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <dmalacov@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/14 18:12:08 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/10/20 15:30:44 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "builtin.h"
#include "libft.h"

/* 
	This function verifies whether the -n option was specified and identifies 
	the first argument that does not contain the option -n and needs to be 
	printed. It returns 1 (TRUE) if a newline is required and 0 (FALSE) if not.
 */
static int	st_skip_n_option(char **args, size_t *i)
{
	int		nl;
	size_t	j;

	nl = TRUE;
	while (args && args[*i])
	{
		j = 0;
		if (args[*i][j] != '-' || args[*i][j + 1] != 'n')
			return (nl);
		j++;
		while (args[*i][j] == 'n')
			j++;
		if (args[*i][j])
			return (nl);
		nl = FALSE;
		*i += 1;
	}
	return (nl);
}

/* 
	This function prints the arguments in the stdout, followed by a newline. 
	If the -n option is specified, no newline is printed.
	It returns 0 upon successful completion and 1 in case of a writing error.
 */
int	bi_echo(char **args)
{
	int		nl;
	size_t	i;

	i = 1;
	nl = st_skip_n_option(args, &i);
	while (args && args[i])
	{
		if (ft_putstr_fd(args[i++], STDOUT_FILENO) < 0)
			return (builtin_error("echo", NULL, "Writing error"), EXIT_FAILURE);
		if (args[i])
			write(STDOUT_FILENO, " ", 1);
	}
	if (nl == 1)
		write(STDOUT_FILENO, "\n", 1);
	return (EXIT_SUCCESS);
}
