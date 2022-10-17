/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_putendl_fd.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/10/11 13:31:38 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/10/11 17:25:07 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"

int	ft_putendl_fd(char *s, int fd)
{
	if (s != NULL)
	{
		if (write(fd, s, ft_strlen(s)) < 0)
			return (-1);
	}
	return (write(fd, "\n", 1));
}
