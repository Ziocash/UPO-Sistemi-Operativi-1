# System calls

System calls are the real codebase, softwares use system calls:

- Create a file: e.g.: `creat` for POSIX standard
- Open or close a file: e.g.: `open` and `close`
- Read or write a file (or a buffer): `read` and `write`

## Snippets

[Snippets](./snippets) are provided by Professor Davide Cerotti

- Snippets directory does not contain a README file
- Snippets contains educational purpose code

## Exercise 1

Code a program that uses unbuffered I/O functions:

1. take a filename through `argv`
2. open or create file, if it creates the file must set read and write permissions to file owner and group
3. open file in append mode
4. write a string into the file (through `argv` arguments or statically set in `main`)
5. close file

Verify that multiple executions effectively use append mode.
