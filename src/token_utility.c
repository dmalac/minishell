/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   token_utility.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmonfrin <dmonfrin@student.codam.n>          +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/04 15:07:50 by dmonfrin      #+#    #+#                 */
/*   Updated: 2022/10/04 15:07:53 by dmonfrin      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	**ft_free_mem(char **str_of_str, int count)
{
	int	i;

	i = 0;
	if (!str_of_str)
		return (NULL);
	if (!*str_of_str)
	{
		free(str_of_str);
		str_of_str = NULL;
		return (NULL);
	}
	while (i < count && str_of_str[i])
	{
		free_set_null(str_of_str[i]);
		i++;
	}	
	free(str_of_str);
	str_of_str = NULL;
	return (NULL);
}

char	*ft_strjoinfree(char *s1, char *s2)
{
	char	*nws;
	size_t	nwssize;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (free_set_null(s2));
	if (!s2)
		return (free_set_null(s1));
	nwssize = ft_strlen(s1) + ft_strlen(s2) + 1;
	nws = (char *)malloc(nwssize * sizeof(char));
	if (!nws)
	{	
		free_set_null(s1);
		return (free_set_null(s2));
	}
	ft_strlcpy(nws, s1, nwssize);
	free_set_null(s1);
	ft_strlcat(nws, s2, nwssize);
	free_set_null(s2);
	return (nws);
}

int	ft_str_strlen(char **tokens)
{
	int	count;

	count = 0;
	if (!tokens)
		return (count);
	while (tokens[count])
		count++;
	return (count);
}

char	*free_set_null(char *str)
{
	if (!str)
		return (NULL);
	free(str);
	str = NULL;
	return (NULL);
}

char	ft_isvar_empty(char *str)
{
	return (str[0] == '$' && (ft_isalpha(str[1]) || str[1] == '_'));
}
