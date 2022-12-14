/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strdup.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/10/13 15:40:51 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/09/20 17:45:55 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*copy;
	size_t	i;
	size_t	len;

	len = ft_strlen(s1);
	copy = (char *) malloc(len + 1);
	if (copy == NULL || s1 == NULL)
		return (NULL);
	else
	{
		i = 0;
		while (i <= len)
		{
			copy[i] = s1[i];
			i++;
		}
		return (copy);
	}
}
