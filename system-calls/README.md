# System calls

System calls are the real codebase, softwares use system calls:

- Create a file: e.g.: `creat` for POSIX standard
- Open or close a file: e.g.: `open` and `close`
- Read or write a file (or a buffer): `read` and `write`

## Snippets

[Snippets](./snippets) are provided by Professor Davide Cerotti

- Snippets directory does not contain a README file
- Snippets contains educational purpose code
- Snippets directory contains original code

## Exercise 1

Related to [`append.c`](./files/append.c)

Code a program that uses unbuffered I/O functions:

1. takes a filename through `argv`
2. opens or creates file, if it creates the file must set read and write permissions to file owner and group
3. opens file in append mode
4. writes a string into the file (through `argv` arguments or statically set in `main`)
5. closes file

Verify that multiple executions effectively use append mode.

## Exercise 2

Related to [`stdoutredir.c`](./redirection/stdoutredir.c)

Code a program that:

1. takes a filename through `argv`
2. opens or creates file
3. redirects `stdout` onto opened file
4. writes a string onto `stdout` (through `argv` arguments or statically set in `main`)
5. closes file

Verify that the string is effectively redirected onto opened file.

File [`redir.c`](./snippets/redir.c) is quite similar.

## Exercise 3

Related to [`stdinredir.c`](./redirection/stdinredir.c)

Code a program that:

1. takes a filename through `argv`
2. if a filename is provided:
   1. redirects `stdin` using provided filename
3. uses `scanf` to read input string
4. using this approach:
   1. with no arguments, string will be read from keyboard insertion
   2. passing a filename, string will be automatically read from file
5. handles properly error conditions

## Exercise 4

Related to [`looplf.c`](./snippets/looplf.c) and [`loopsc.c`](./snippets/loopsc.c)

Time differences between looplf and loopsc are consistently different due to buffered and unbuffered I/O.

## Exercise 5

Related to [`hellolf.c`](./snippets/hellolf.c) and [`hellosc.c`](./snippets/hellosc.c)

Time differences between hellolf and hellosc are consistently different due to buffered and unbuffered I/O.

We can use `fflush(stdout)` in unbuffered I/O to set the behaviour equal to buffered operation.
