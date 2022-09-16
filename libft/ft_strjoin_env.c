/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strjoin_env.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/10/11 15:43:44 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/09/08 17:53:59 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	fill_str(char *joined, char const *s1, char const *s2, char connect)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (s1[i] != '\0')
	{
		joined[i] = s1[i];
		i++;
	}
	joined[i] = connect;
	i++;
	while (s2[j] != '\0')
	{
		joined[i] = s2[j];
		i++;
		j++;
	}
	joined[i] = '\0';
}

char	*ft_strjoin_env(char const *s1, char const *s2, char connect)
{
	size_t	len1;
	size_t	len2;
	char	*joined;

	if ((s1 == NULL) || (s2 == NULL))
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	joined = (char *) malloc((len1 + len2 + 2) * sizeof(char));
	if (joined == NULL)
		return (NULL);
	fill_str(joined, s1, s2, connect);
	return (joined);
}
