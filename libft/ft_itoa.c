/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_itoa.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/10/11 13:29:26 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/04/11 16:37:17 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static size_t	power_of_ten(int n)
{
	if (n / 10 == 0)
		return (0);
	return (power_of_ten(n / 10) + 1);
}

static char	*int_to_str(int n, char *str, size_t len)
{
	size_t	i;

	i = 0;
	if (n == 0)
		str[i++] = '0';
	else
	{
		if (n < 0)
			str[i] = '-';
		if (n > 0)
			n = n * -1;
		while ((n / -10 != 0) || (n % 10 != 0))
		{
			str[len - 1 - i++] = -(n % 10) + '0';
			n = n / 10;
		}
	}
	str[len] = '\0';
	return (str);
}

char	*ft_itoa(int n)
{
	char	*str;
	size_t	len;

	len = power_of_ten(n);
	len++;
	if (n < 0)
		len++;
	str = (char *) malloc(len + 1);
	if (str == NULL)
		return (NULL);
	int_to_str(n, str, len);
	return (str);
}
