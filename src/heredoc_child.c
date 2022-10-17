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
#include "libft.h"
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>

/* 
	This function handles the SIGINT signal in the here_doc.
 */
static void	st_heredoc_handler_sigint(int sig)
{
	g_signal = sig;
	rl_done = 1;
	write(STDERR_FILENO, "\n", 1);
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
	This function assigns the signal handling function to the struct sigaction 
	sa and calls the sigaction function. It also sets the rl_event_hook.
 */
static void	st_heredoc_init_signal_handling(struct sigaction *sa)
{
	(*sa).sa_handler = &st_heredoc_handler_sigint;
	sigaction(SIGINT, sa, NULL);
	signal(SIGQUIT, SIG_IGN);
	rl_event_hook = &st_check_global_var;
}

/* 
	This function receives the user input for each here_doc line by line and 
	writes each line into the corresponding pipe's reading end fd. Upon success, 
	it exits with exit code 0. If SIGINT was received, it exits with exit code 1.
 */
void	st_heredoc_child_receive_input(t_heredoc **hd_list, t_heredoc *hd_node, \
t_symtab **symtab)
{
	char	*line;
	int		start;

	start = 1;
	line = NULL;
	while ((g_signal == 0 && line && ft_strncmp(line, \
	hd_node->limiter, ft_strlen(hd_node->limiter) + 1) != 0) || start == 1)
	{
		start = 0;
		if (line)
		{
			if (hd_node->expand == 1)
				line = heredoc_expand_var(line, symtab, hd_list, hd_node);
			ft_putendl_fd(line, hd_node->hd_pipe[W]);
			free(line);
		}
		line = readline("> ");
	}
	if (g_signal == 1)
	{
		heredoc_child_close_pipes(hd_node, W);
		exit(EXIT_FAILURE);
	}
	free(line);
	close(hd_node->hd_pipe[W]);
}

/* 
	This function manages the processing of the here_doc redirection.
 */
void	heredoc_child_process_redir(t_heredoc **hd_list, t_symtab **symtab)
{
	struct sigaction	sa;
	t_heredoc			*hd_node;

	signal(SIGINT, SIG_DFL);
	st_heredoc_init_signal_handling(&sa);
	hd_node = *hd_list;
	heredoc_child_close_pipes(*hd_list, R);
	while (hd_node)
	{
		st_heredoc_child_receive_input(hd_list, hd_node, symtab);
		hd_node = hd_node->next;
	}
	exit(EXIT_SUCCESS);
}
