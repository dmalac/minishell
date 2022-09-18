/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strlcat.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/10/05 17:50:54 by dmalacov      #+#    #+#                 */
/*   Updated: 2021/11/05 16:29:04 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t dstsize)
{
	size_t	len_dest;
	size_t	len_src;
	size_t	i;

	len_src = ft_strlen(src);
	if (dstsize == 0)
		return (len_src);
	len_dest = ft_strlen(dest);
	i = 0;
	while (((len_dest + i + 1) < dstsize) && (src[i] != '\0'))
	{
		dest[len_dest + i] = src[i];
		i++;
	}
	dest[i + len_dest] = '\0';
	if (len_dest >= dstsize)
		return (dstsize + len_src);
	else
		return (len_dest + len_src);
}
