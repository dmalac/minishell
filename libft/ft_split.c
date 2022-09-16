/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_split.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/10/26 12:02:38 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/08/15 12:44:53 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static void	erase_array(char **array, int i)
{
	int	j;

	j = 0;
	while (j <= i)
		free(array[j++]);
	free(array);
}

static char	*fill_array(char const *s, char c, unsigned int start, size_t len)
{
	char	*str;
	int		i;

	str = (char *) malloc((len + 1) * sizeof(char));
	if (str == NULL)
		return (NULL);
	else
	{
		i = 0;
		while ((s[start] != c) && (s[start] != '\0'))
			str[i++] = s[start++];
		str[i] = '\0';
		return (str);
	}
}

static char	*cut_string(char const *s, char c, unsigned int *start)
{
	unsigned int	i;
	size_t			len;
	char			*str;

	i = *start;
	len = 0;
	while (s[i] == c)
	{
		i++;
		*start = *start + 1;
	}
	while ((s[i] != c) && (s[i] != '\0'))
	{
		len++;
		i++;
	}
	str = fill_array(s, c, *start, len);
	if (str == NULL)
		return (NULL);
	else
	{	
		*start = i;
		return (str);
	}
}

static char	**make_frame(char const *s, char c, int *num_of_str)
{
	int		i;
	char	**array;

	i = 0;
	while (s[i] != '\0')
	{
		while (s[i] == c)
			i++;
		while ((s[i] != c) && (s[i] != 0))
		{
			if ((i == 0) || ((i > 0) && (s[i - 1] == c)))
				*num_of_str = *num_of_str + 1;
			i++;
		}
	}
	array = (char **) malloc((*num_of_str + 1) * sizeof(char *));
	return (array);
}

char	**ft_split(char const *s, char c)
{
	char			**array;
	int				i;
	int				num_of_str;
	unsigned int	start;

	i = 0;
	start = 0;
	num_of_str = 0;
	if (s == NULL)
		return (NULL);
	array = make_frame(s, c, &num_of_str);
	if (array == NULL)
		return (NULL);
	while (i < num_of_str)
	{
		array[i] = cut_string(s, c, &start);
		if (array[i] == NULL)
		{
			erase_array(array, i);
			return (NULL);
		}
		i++;
	}
	array[i] = NULL;
	return (array);
}
