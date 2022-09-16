/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strncmp.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/10/13 14:53:55 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/08/15 12:21:25 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned int	i;
	int				dif;

	i = 0;
	dif = 0;
	while (i < n)
	{
		dif = ((unsigned char *)s1)[i] - ((unsigned char *)s2)[i];
		if ((dif != 0) || (s1[i] == '\0') || (s2[i] == '\0'))
			return (dif);
		i++;
	}
	return (dif);
}
