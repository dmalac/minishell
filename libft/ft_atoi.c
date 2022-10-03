/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_atoi.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/10/11 13:22:12 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/05/12 11:15:27 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static int	find_number(const char *str, int neg, int i)
{
	int	number;

	number = 0;
	while ((str[i] >= '0') && (str[i] <= '9'))
		number = number * 10 + (str[i++] - '0');
	if (neg == -1)
		number = number * -1;
	return (number);
}

int	ft_atoi(const char *str)
{
	int	neg;
	int	i;
	int	number;

	neg = 1;
	i = 0;
	while (((str[i] >= 9) && (str[i] <= 13)) || (str[i] == 32))
		i++;
	if (str[i] == '-')
		neg = neg * -1;
	if ((str[i] == '+') || (str[i] == '-'))
		i++;
	number = find_number(str, neg, i);
	return (number);
}
