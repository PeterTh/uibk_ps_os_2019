# Exercise Sheet 10

This week we continue with memory management.

# Task 1

Extend your `membench` application from last week to make its memory allocation
behavior a bit more interesting:

- First, perform `N` allocations of random sizes between `S` and `8*S`. Make
  sure to use `rand_r` as your random number generator.
- Then, free **some** of the allocations you've made. For this, go over your
  allocations and randomly (with a 50% chance) decide whether to free them or
  not.
- Allocate another `N/2` blocks of random size, as before.
- Finally, free all allocated blocks.

# Task 2

Implement a **free list allocator** (see lecture slides) which uses free lists
for requests up to `2^16` Bytes (one separate free list for each power of 2).

As in last week's exercise, create one version that uses a global set of free
lists with access locks, and another which uses a separate allocator for each
thread.

# Task 3

Use the new `membench` implemented in Task 1 to compare all 4 of your
allocators from this week and last week as well as the default system
allocator. Perform your comparisons across 1, 2, 4 and 8 threads, and for
allocations of `S=2^X` Bytes for `X=4...20`.
