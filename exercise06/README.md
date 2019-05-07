# Exercise Sheet 6

In this exercise sheet you will use POSIX threads and test out different
methods for mutual exclusion and synchronization. As before,
**provide a Makefile** for all executables.

# Task 1

Write a program which spawns 10 threads.

- Each thread waits for a random time between 0 and 3 seconds, then creates a
  file called `threadN.txt` (where `N` is the thread number between 0 and 9)
  and writes its thread id (returned by `pthread_self` or
  `syscall(SYS_gettid)`) to this file. Ensure that the file is closed in
  case a thread opens it and is subsequently interrupted (study the
  `pthread_cleanup` family of functions).
- After spawning all threads, the main program decides for each thread whether
  to randomly cancel it, with a chance of cancellation of 50%.
- Afterwards, the main program waits for all threads to exit.
- What is the difference between the two ways of obtaining a thread ID?

# Task 2

In this exercise you will implement a _single producer multiple consumer_
pattern.

Your program should spawn 5 threads, the _consumers_, which all try to read
elements from a queue. Queued elements are of type `unsigned`.

> **Note**: You can use the basic C queue that is provided in
> [myqueue.h](task2/myqueue.h) (a short example demonstrating its usage is provided
> in [producer_consumer.c](task2/producer_consumer.c)).

- When a consumer thread successfully reads an element, it adds it to its
  local `sum`. When the element is 0, it prints out the sum and exits.
- The main thread acts as the _producer_. After spawning all 5 consumers, it
  feeds 10.000 entries of value `1` into the queue, followed by 5 entries of
  value `0`.

Access to the queue should be protected by using the `pthread_mutex` facilities.

# Task 3

In the provided file [philosophers.c](task3/philosophers.c) you can find an implementation of
the classic [Dining philosophers problem](https://en.wikipedia.org/wiki/Dining_philosophers_problem). However,
the implementation has an issue: In some situations multiple philosophers
will wait forever for a chopstick being released, constituting a
**deadlock**.

- Explain the program.
- How can a deadlock occur? Does it always happen?
- Change the program to prevent any potential deadlocks from occurring.
  Explain how your changes prevent deadlocks.
