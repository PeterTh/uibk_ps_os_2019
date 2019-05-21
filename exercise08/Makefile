CFLAGS = -std=gnu11 -Wall -Werror -Wextra
LDLIBS = -lm

.PHONY: all clean

all: main

clean:
	$(RM) scheduling_sim scheduling_sim.o scheduling_utility.o

main: main.c scheduling_sim.o scheduling_utility.o
