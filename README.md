# minishell

A simple shell implementation in C, built as part of the 42 curriculum.

## overview

Recreation of basic bash functionality with custom parsing and execution engine.

## features

- Command execution with PATH resolution
- Built-in commands (`echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`)
- Pipes and redirections (`|`, `<`, `>`, `>>`)
- Environment variable expansion (`$VAR`)
- Signal handling (Ctrl-C, Ctrl-D, Ctrl-\)
- Quote parsing (single and double quotes)

## usage

```bash
make
./minishell
```
