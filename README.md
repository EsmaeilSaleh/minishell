*This project has been created as part of the 42 curriculum by esaleh, dkpg-md-.*

---

# minishell

## Description

minishell is a minimal Unix shell written in C, built as part of the 42 school curriculum. The goal is to reproduce the core behavior of bash by implementing command parsing, execution, pipes, redirections, environment variable handling, signal management, and all required built-in commands from scratch.

The project covers a wide range of systems programming topics: process creation with `fork` and `execve`, inter-process communication with `pipe` and `dup2`, signal handling with `sigaction`, terminal control with `readline` and `tcsetattr`, and dynamic memory management throughout.

**Supported features:**

- Interactive prompt with command history (readline)
- Single and double quote handling
- Environment variable expansion (`$VAR`, `$?`)
- Pipelines (`cmd1 | cmd2 | cmd3`)
- Redirections: `<`, `>`, `>>`, `<<` (heredoc)
- Multi-line input with unclosed quotes (continuation prompt `>`)
- Signal handling: `Ctrl+C`, `Ctrl+D`, `Ctrl+\`
- Built-in commands: `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`

---

## Instructions

### Requirements

- GCC or Clang
- GNU Readline library (`libreadline-dev` on Ubuntu/Debian)
- Linux (the project uses Linux-specific terminal flags)

On Ubuntu/Debian:
```bash
sudo apt-get install gcc make libreadline-dev
```

### Compilation

```bash
make
```

This produces the `minishell` binary in the project root.

Other make targets:

```bash
make clean    # remove object files
make fclean   # remove object files and binary
make re       # fclean + all
```

### Running

```bash
./minishell
```

The shell starts in interactive mode. Type commands at the `minishell$` prompt.

To run non-interactively (pipe input):
```bash
echo "echo hello" | ./minishell
```

### Usage examples

```bash
minishell$ echo "hello $USER"
minishell$ ls -la | grep .c | wc -l
minishell$ export MY_VAR=42
minishell$ echo $MY_VAR
minishell$ cat << EOF
> line one
> line two
> EOF
minishell$ cd .. && pwd
minishell$ exit 0
```

---

## Resources

### Shell and POSIX

- [GNU Bash Manual](https://www.gnu.org/software/bash/manual/bash.html) — the primary reference for expected behavior
- [POSIX Shell Command Language](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html) — formal specification
- [Advanced Programming in the UNIX Environment — Stevens & Rago](https://www.apuebook.com/) — in-depth coverage of `fork`, `execve`, `pipe`, `signal`

### Readline

- [GNU Readline Library Manual](https://tiswww.case.edu/php/chet/readline/rltop.html)
- [Readline signal handling](https://tiswww.case.edu/php/chet/readline/readline.html#SEC44)

### Systems programming

- `man 2 fork`, `man 2 execve`, `man 2 pipe`, `man 2 dup2`
- `man 2 sigaction`, `man 7 signal`
- `man 3 tcgetattr`, `man 3 tcsetattr`
- [Linux man pages online](https://man7.org/linux/man-pages/)

### AI usage

AI tools (Claude) were used during this project in the following ways:

- **Understanding concepts**: asking for explanations of POSIX behavior, signal handling semantics, and terminal control functions — then verifying those explanations against the man pages and testing them directly.
- **Debugging assistance**: describing observed bugs and asking what could cause them, then tracing through the code ourselves to confirm or reject the suggested cause before making any change.
- **Identifying edge cases**: asking what edge cases bash handles for specific builtins (e.g. `export`, `cd -`, `unset`) to make sure we tested them, then verifying the behavior ourselves with bash.
- **Code review questions**: asking whether specific patterns (e.g. signal handling, buffer flushing before fork) were correct, using the answers as a starting point for our own investigation.

Every piece of code in this project was written, read, understood, and tested by us. AI was used as a reference tool — similar to asking a more experienced peer a question — not as a code generator. Any suggestion we could not fully explain and justify was not used.
