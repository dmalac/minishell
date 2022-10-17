/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_exit.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <dmalacov@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/13 18:02:40 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/10/17 15:00:27 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "builtin.h"

/* 
	This function verifies whether the string in the argument only contains 
	digits (and one sign '-' or '+') and returns 0 (FALSE) or 1 (TRUE).
 */
static int	st_only_digits(char *str)
{
	size_t	i;
	size_t	sign;

	i = 0;
	sign = 0;
	if (str[i] == '-' || str[i] == '+')
	{
		i++;
		sign = 1;
	}
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	if (i > sign)
		return (1);
	else
		return (0);
}

/* 
	This function verifies whether the number in the argument is between (and 
	including LONG MIN and LONG MAX) and returns 0 (FALSE) or 1 (TRUE).
 */
static int	st_is_long(char *str)
{
	char	*num;

	num = str;
	if (*num == '-' || *num == '+')
		(num)++;
	while (*num && *num == '0')
		(num)++;
	if (ft_strlen(num) > 19)
		return (FALSE);
	if (ft_strlen(num) < 19)
		return (TRUE);
	else if (str[0] == '-' && ft_strncmp(num, "9223372036854775808", 20) <= 0)
		return (TRUE);
	else if (ft_strncmp(num, "9223372036854775807", 20) <= 0)
		return (TRUE);
	else
		return (FALSE);
}

/* 
	This function verifies whether the argument is a valid exit code and returns 
	0 (FALSE) or 1 (TRUE).
 */
static int	st_is_valid_num_arg(char *arg)
{
	if (st_only_digits(arg) == FALSE)
		return (FALSE);
	else
		return (st_is_long(arg));
}

/* 
	The function exits the process with the correct exit code.
 */
int	bi_exit(char **args, int who)
{
	int	exit_code;

	if (!args[1] || st_is_valid_num_arg(args[1]) == FALSE || !args[2])
	{
		if (who == PARENT)
			ft_putendl_fd("exit", 2);
		if (!args[1])
			return (0 + BI_EXITED);
		else if (st_is_valid_num_arg(args[1]) == FALSE)
		{
			builtin_error("exit", args[1], "numeric argument required");
			return (255 + BI_EXITED);
		}
		else if (!args[2])
		{
			exit_code = ft_atol(args[1]);
			return (((exit_code % 256 + 256) % 256) + BI_EXITED);
		}
	}
	else
		builtin_error("exit", NULL, "too many arguments");
	return (1);
}
