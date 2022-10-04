/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   libft.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <dmalacov@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/05 18:32:48 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/10/04 15:34:18 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdlib.h>

enum	e_bool
{
	FALSE,
	TRUE,
};

int			ft_atoi(const char *str);
long long	ft_atol(const char *str);
void		ft_bzero(void *s, size_t n);
int			ft_isalnum(int c);
int			ft_isalpha(int c);
int			ft_isdigit(int c);
char		*ft_itoa(int n);
void		ft_putendl_fd(char *s, int fd);
void		ft_putstr_fd(char *s, int fd);
char		**ft_split(char const *s, char c);
char		*ft_strdup(const char *s1);
size_t		ft_strlcat(char *dest, const char *src, size_t dstsize);
size_t		ft_strlcpy(char *dst, const char *src, size_t dstsize);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
size_t		ft_strlen(const char *str);
char		*ft_strnstr(const char *haystack, const char *needle, size_t len);
char		*ft_strjoin_env(char const *s1, char const *s2, char connect);
char		*ft_strjoin(char const *s1, char const *s2);
char		*ft_substr(char const *s, unsigned int start, size_t len);

#endif