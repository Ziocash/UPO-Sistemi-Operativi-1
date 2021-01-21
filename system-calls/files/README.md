# Unbuffered I/O

Related to file [`append.c`](./append.c)

This part refers to system calls chapter.

Using an unbuffered I/O operation (`open` to open file and `write` to edit it) this code appends some text onto a given file.

Using `argv` params the user can pass a filepath.
