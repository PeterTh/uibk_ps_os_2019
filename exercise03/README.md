# Exercise Sheet 3

Note that starting with this exercise all implementation work
should be completed in **C**. Ensure that your programs compile 
with `-Wall -Werror`.

In this exercise sheet you will work with processes and signals.

**Hint**: Remember that a process can be created with `fork()`, `vfork()`, and in
Linux also with `clone()`. A parent process can wait for its children using `wait()` or
`waitpid()`. Choose the most suitable option for process creation and waiting for
every situation.

## Task 1

### a)

Write a program in which the parent process creates exactly 7 child processes
and waits for them to finish. Every child process finishes immediately after
being created.

### b)

Write a program which creates exactly 12 child processes. After that, the parent
process prints on the standard output the message: `12 child processes have been created`.

Every child process simply prints its `pid` on the standard output. Analyse the
obtained output. Is the order of the messages (parent and child ones)
consistent? Can the order of these messages be predicted? What does it depend
on?

## Task 2

Write a program in which the parent creates exactly 1 child process. The child
process should print its `pid` to the standard output and then finish. The
parent process waits until it is sure that the child has terminated.
The parent terminates after it has waited for the child process.

Note that you **may not** use `wait()` or `waitpid()` in this task.

**Hint**: Check the man page of `fork()` to see whether a parent is notified of
child termination via any signal. Also see `sigaction()` to determine how to
respond to an incoming signal.

## Task 3

Write a program in which the parent process creates exactly 1 child. After
creating the child, the behaviour of the **parent** process is as follows:
Five seconds after creating the child, the parent process sends `SIGUSR2`
to the child. Afterwards it continously sends `SIGUSR1` every five seconds
until the child process has terminated. To implement this behaviour the
parent process must use the following functions: `alarm()` and `pause()`.

The behaviour of the **child** is as follows: On startup, it first blocks `SIGUSR2`,
and unblocks it after 13 seconds. Upon receiving a signal the child
should react as follows:

- `SIGUSR1` prints a message to standard out.
- `SIGUSR2` terminates the process.

**Hint**: Check the man page of `alarm()`, `pause()`, `sigprocmask()` and
`sigaction()`. For a general overview see `man 7 signal`.
