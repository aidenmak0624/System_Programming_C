CC = clang
CFLAGS = -Wall -Wpedantic -Wextra -Werror -pthread

MLFQ: MLFQ.c
	$(CC) $(CFLAGS) -o MLFQ MLFQ.c

run: MLFQ tasks.txt
	./MLFQ 8 200 tasks.txt

.PHONY: clean
clean:
	rm -f MLFQ
