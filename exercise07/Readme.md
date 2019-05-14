# Exercise Sheet 7

In this exercise sheet you will continue to use POSIX threads and test different methods for mutual exclusion and synchronization.
As before, **provide a Makefile** for all executables.

# Task 1

Implement a single producer multiple consumer pattern as requested in Exercise Sheet 6 Task 2 ([link](../exercise06/README.md)), but use `pthread_spin_lock` to protect queue access. What are the advantages and disadvantages compared to `pthread_mutex`?

Measure the execution time with `/usr/bin/time -v [executable]`, compare it to the execution time of last week's task and interpret the results.

# Task 2

Repeat Task 1 (including the execution time measurement and interpretation), but use pthread condition variables to signal the availability of new elements. What are the advantages and disadvantages compared to the other two approaches?

# Task 3

## a)
Write a program that initializes an integer value `X` to 0 and subsequently creates 1000 threads. Each of these newly created threads should execute a loop of 10000 iterations. In each of these iterations, the value `X` should be increased by one. The main thread waits for all the other threads to finish and then writes the value `X` to the output.

## b)
Instead of simply incrementing by 1, use C11 atomics to perform the increment operation. How does the program behaviour differ? Measure the execution time of both versions.
