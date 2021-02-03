# Signals and pipes

- Related to file [`sig.c`](./sig.c)
  - Code a program that uses [`sig.c`](../snippets/sig.c).
  - Source must print out process PID.
  - Using terminal or htop send to PID signals like SIGTERM, SIGINT and SIGSEGV.
  - Edit the code to ignore those signals.
  - Verify with previous points and using `fork`, `exec` and `sigaction` if actions like "ignoring a signal" and "execute a function" are:
    - inherited by child process.
    - kept by a process that calls an `exec` call.

- Related to file [`pipes.c`](./pipes.c)
  - Code a program that uses [`pipes.c`](../snippets/pipes.c).
  - Use "`head -2 pipes.c | tail -10`" instead of "`ls -l | wc -l`".

- Related to file [`catmore.c`](./catmore.c)
  - Realize a pipe with 2 producers and 1 consumer.
    - Producers:
      1. `cat <file1>`
      2. `cat <file2>`
    - Consumer:
      1. `more`
  - Use `argv` to pass \<file1\> and \<file2\>
