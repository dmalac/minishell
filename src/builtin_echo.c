/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_echo.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <dmalacov@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/14 18:12:08 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/09/27 14:29:01 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "builtin.h"
#include "libft.h"

static int	st_skip_n_option(char **args, size_t *i)
{
	int		nl;
	size_t	j;

	nl = 1;
	while (args && args[*i])
	{
		j = 0;
		if (args[*i][j] != '-')
			return (nl);
		j++;
		while (args[*i][j] == 'n')
			j++;
		if (args[*i][j])
			return (nl);
		nl = 0;
		*i += 1;
	}
	return (nl);
}

int	bi_echo(char **args)
{
	int		nl;
	size_t	i;

	i = 1;
	nl = st_skip_n_option(args, &i);
	while (args && args[i])
	{
		if (printf("%s", args[i++]) < 0)
			return (builtin_error("echo", NULL, "Writing error"), 1);
		if (args[i])	// watch out for "" (should not be NULL)
			printf(" ");
	}
	if (nl == 1)
		printf("\n");
	return (0);
}
