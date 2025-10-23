CC=gcc
CFLAGS=-I. -Wall -Wpedantic -Wextra -Werror
DEPS = Dir.h FAT32.h FsInfo.h
OBJ = fat32Reader.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

fat32Reader: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(OBJ) fat32Reader

