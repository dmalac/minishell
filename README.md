# minishell
Codam project in collaboration with [@greedymelon](https://github.com/greedymelon/greedymelon)

## shell from scratch
In this project, we wrote our own shell.

We implemented
- a prompt with a working history
- a parser that handles single `'` and double `"` quotes, the dollar sign `$` and pipes `|`
- redirections `<` `>` `>>` including here doc `<<`
- environment variables management and expansion (including `$?`)
- custom signal handlers for SIGINT and SIGQUIT
- execution of piped commands by child processes
- and all that while only using ONE global variable

We also wrote several builtin functions: `echo`, `cd`, `pwd`, `export`, `unset`, `env` and `exit`.

## inspired by bash
Our **minishell** executes commands just like bash. However, not all functionality falls in the scope of our project. 

Here are some examples of things we did not handle:
- `$1`, `$2`..., `$$`
- `\`, `;`, `*`
- `||`, `&&`
- certain flags/options in our builtin functions

## wanna see it in action?
The executable is called `minishell`.

Clone, `make` and enjoy
