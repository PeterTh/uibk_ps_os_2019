CFLAGS = -std=gnu11 -pthread -Wall -Werror -Wextra -g
LDFLAGS = -pthread

.PHONY: all clean

all: membench_1 membench_2 membench_3

clean:
	$(RM) membench_1 membench_2 membench_3

membench_1: membench.c task1.c
	$(CC) $(CFLAGS) $^ -o $@

membench_2: membench.c task2_3.c
	$(CC) $(CFLAGS) $^ -o $@

membench_3: membench.c task2_3.c
	$(CC) $(CFLAGS) $^ -DPER_THREAD_ALLOCATOR -o $@

