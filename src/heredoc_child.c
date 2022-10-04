/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc_child.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <dmalacov@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/04 18:47:02 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/10/04 18:47:02 by dmalacov      ########   odam.nl         */
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

/* 
	This function handles the SIGINT signal in the heredoc.
 */
static void	st_heredoc_handler_sigint(int sig)
{
	g_signal = sig;
	rl_done = 1;
	rl_on_new_line();
	rl_done = 0;
}

/* 
	This function checks whether the g_signal variable equals the SIGINT. If so, 
	it sets the g_signal variable to 1.
 */
static int	st_check_global_var(void)
{
	if (g_signal == SIGINT)
	{
		g_signal = 1;
		rl_done = 1;
	}
	return (0);
}

/* 
	This function assigns the signal handling functions to the struct sigaction 
	sa_si and sa_sq and calls the sigaction function. It also sets the 
	rl_event_hook.
 */
static void	st_heredoc_init_signal_handling(struct sigaction *sa_si, \
struct sigaction *sa_sq)
{
	(*sa_si).sa_handler = &st_heredoc_handler_sigint;
	(*sa_sq).sa_handler = SIG_IGN;
	sigaction(SIGINT, sa_si, NULL);
	sigaction(SIGQUIT, sa_sq, NULL);
	rl_event_hook = &st_check_global_var;
}

/* 
	This function receives the user input for each heredoc line by line and 
	writes each line into the corresponding pipe's reading end fd. Upon success, 
	it exits with exit code 0. If SIGINT was received, it exits with exit code 1.
 */
void	st_heredoc_child_receive_input(t_heredoc *hd_list)
{
	char	*line;
	int		start;

	start = 1;
	line = NULL;
	while ((g_signal == 0 && line && ft_strncmp(line, \
	hd_list->limiter, ft_strlen(hd_list->limiter) + 1) != 0) || start == 1)
	{
		start = 0;
		if (line)
		{
			ft_putendl_fd(line, hd_list->hd_pipe[W]);
			free(line);
		}
		line = readline("> ");
	}
	if (g_signal == 1)
	{
		heredoc_child_close_pipes(hd_list, W);
		exit(EXIT_FAILURE);
	}
	free(line);
	close(hd_list->hd_pipe[W]);
}

/* 
	This function manages the processing of the heredoc redirection.
 */
void	heredoc_child_process_redir(t_heredoc *hd_list)
{
	struct sigaction	sa_si;
	struct sigaction	sa_sq;

	st_heredoc_init_signal_handling(&sa_si, &sa_sq);
	heredoc_child_close_pipes(hd_list, R);
	while (hd_list)
	{
		st_heredoc_child_receive_input(hd_list);
		hd_list = hd_list->next;
	}
	exit(EXIT_SUCCESS);
}
