# Exercise Sheet 9

In this exercise you will implement and benchmark different types of memory management in a threaded parallel application.


# Task 1

Create an application `membench T N S` which launches a configurable (program launch parameter `T`) number of threads. 
Each thread should perform a given number of memory allocations and deallocations (`N`) of a given size in bytes (`S`).

For example
```
./membench 8 10000 1024
```
would launch 8 threads, each of which performs 10000 allocations of 1 kiB each.


# Task 2

Implement a naive *best fit* memory allocator (see lecture slides) and use it in your application.
The allocator should provide its own versions of the `malloc` and `free` functions.

In this task, use a single global allocator (which of course needs to be locked when used) that is shared by all threads.

Compare the performance of your allocator to the system-provided `malloc` and `free` function using "membench", with a few different numbers of threads and allocation sizes, and a number of allocations/deallocations that gives a reasonable total runtime (more than a second and less than a minute).


# Task 3

For this task, instead of a single global allocator, create a separate local allocator for each thread. 
For creating local instances of the allocator, you can use thread-local storage in GCC using `__thread` (see e.g. http://gcc.gnu.org/onlinedocs/gcc-7.3.0/gcc/Thread-Local.html for further information)

Compare the performance of Task 2 and 3 in "membench", with a few different numbers of threads and allocation sizes, and a number of allocations/deallocations that gives a reasonable total runtime (more than a second and less than a minute).
