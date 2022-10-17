/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   executor.h                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dmalacov <dmalacov@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/08 15:33:56 by dmalacov      #+#    #+#                 */
/*   Updated: 2022/10/11 20:24:09 by dmalacov      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include <stdlib.h>
# include "minishell.h"
// # include "symtab.h"

typedef struct s_heredoc
{
	int					hd_pipe[2];
	char				*limiter;
	int					expand;
	size_t				cmd_no;
	struct s_heredoc	*next;
}				t_heredoc;

typedef struct s_cmd_tools
{
	pid_t		id;
	size_t		cmd;
	size_t		total_cmds;
	int			input_fd;
	int			output_fd;
	char		**cmd_args;
	char		**paths;
	char		**env_var;
	t_heredoc	*heredoc;
	int			builtin_only;
	t_token_lst	*(*process_tokens[6])(struct s_cmd_tools *tools, \
	t_token_lst *input);
}				t_cmd_tools;

# define BEFORE 0
# define AFTER 1

# define NO_ERROR 0
# define CMD_ERROR -1
# define IO_FD_ERROR -2
# define REDIR_ERROR -3

# define R 0
# define W 1

/* executor.c */
// int	executor(t_token_lst *input, t_symtab *symtab, struct sigaction *sa);
int			executor(t_token_lst *input, t_symtab **symtab);
/* exec_parent.c */
int			ex_pipe_and_fork(int *id, t_cmd_tools *tools, int (*pipe_end)[2]);
void		ex_close_unnecessary_pipes(t_cmd_tools *tools, int pipe_end[2][2]);
int			ex_parent_exec_builtin(t_cmd_tools *tools, t_token_lst *input, \
t_symtab **symtab);
int			ex_wait_for_last_child(int id, size_t total_cmds);
/* exec_child.c */
void		ex_perform_cmd(t_cmd_tools *tools, t_token_lst *input, \
int pipe_end[2][2], t_symtab **symtab);
/* token_processing.c */
t_token_lst	*ex_process_word(t_cmd_tools *tools, t_token_lst *node);
t_token_lst	*ex_process_input_redir1(t_cmd_tools *tools, t_token_lst *node);
t_token_lst	*ex_process_input_redir2(t_cmd_tools *tools, t_token_lst *node);
t_token_lst	*ex_process_output_redir1(t_cmd_tools *tools, t_token_lst *node);
t_token_lst	*ex_process_output_redir2(t_cmd_tools *tools, t_token_lst *node);
/* get.c */
int			ex_get_args(t_cmd_tools *tools, t_token_lst *input);
int			ex_get_paths(t_symtab *symtab, t_cmd_tools *tools);
int			ex_get_env_var(t_symtab *symtab, t_cmd_tools *tools);
/* heredoc.c */
int			get_heredoc(t_heredoc *hd_list, t_symtab *symtab);
int			check_heredoc(t_token_lst *input, t_cmd_tools *tools);
void		cleanup_hd_list(t_heredoc **heredoc);
/* heredoc_child.c */
void		heredoc_child_process_redir(t_heredoc **hd_list, t_symtab **symtab);
/* heredoc_utils.c */
int			heredoc_open_pipes(t_heredoc *hd_list);
void		heredoc_close_pipes(t_heredoc *hd_list, int exit_code);
void		heredoc_child_close_pipes(t_heredoc *hd_list, int end);
void		heredoc_error_and_exit(t_symtab **symtab, t_heredoc **hd_list, \
t_heredoc *hd_node);
/* heredoc_var_exp.c */
char		*heredoc_expand_var(char *line, t_symtab **symtab, \
t_heredoc **hd_list, t_heredoc *hd_node);

/* exec_errors.c */
void		ex_free_array(char **array);
void		ex_child_error_and_exit(int error_code, t_cmd_tools *tools, \
char *name);
int			ex_print_error_message(int error_code, char *name);
void		ex_cleanup_tools(t_cmd_tools **tools);
int			ex_contains_char(char *str, char c);
/* init.c */
t_cmd_tools	*ex_tools_init(t_token_lst *input, t_symtab *symtab);

#endif