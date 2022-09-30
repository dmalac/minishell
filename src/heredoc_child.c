/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <dmalacov@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/12 12:33:23 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/09/20 16:39:15 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"
#include "libft.h"
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <readline/readline.h>
#include <sys/wait.h>

static void	st_heredoc_handler_sigint(int sig)
{
	g_is_interupt = sig;
	rl_replace_line("", 0);
	rl_redisplay();
	rl_on_new_line();
	rl_done = 0;
	// write(1, "\n", 1);
	// write(1, "\n", 1);
	// exit(sig);
}

static int	check_global_var(void)
{
	if (rl_done == 0 && g_is_interupt == SIGINT)
	{
		rl_line_buffer = NULL;
		rl_done = 1;
	}
		return (0);
}

void	heredoc_child_receive_input(t_heredoc *hd_list)
{
	struct sigaction	sa_si;
	struct sigaction	sa_sq;
	char				*line;
	int					start;

	sa_si.sa_handler = &st_heredoc_handler_sigint;
	sa_sq.sa_handler = SIG_IGN;
	sigaction(SIGINT, &sa_si, NULL);
	sigaction(SIGQUIT, &sa_sq, NULL);
	while (hd_list)
	{
		line = NULL;
		start = 1;
		rl_event_hook = &check_global_var;
		while ((line && ft_strncmp(line, hd_list->limiter, \
		ft_strlen(hd_list->limiter) + 1) != 0) || start == 1)
		{
			start = 0;
			if (line)
			{
				ft_putendl_fd(line, hd_list->hd_pipe[W]);
				free(line);
			}
			line = readline("> ");
			// printf("< glob var is %d >", g_is_interupt);
		}
		printf("out of the loop, still in child; global var is %d\n", g_is_interupt);
		free(line);
		close(hd_list->hd_pipe[W]);
		hd_list = hd_list->next;
	}
	exit(EXIT_SUCCESS);
}
