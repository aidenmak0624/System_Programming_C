# MPI Collective Operations - Makefile

# Project configuration
PROG = mpi_collective_demo           ← NEW NAME
SRC_DIR = src                        ← NEW: Source directory
HDRS =
SRCS = $(SRC_DIR)/mpi_collective_demo.c   ← NEW PATH

# Build configuration
OBJDIR = object
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

# Compiler settings
CC = mpicc
CFLAGS = -Wall --std=c99 -L.
LIBS = -lm

# Build targets
all : $(OBJDIR) $(PROG)

$(PROG) : $(OBJS)
	$(CC) $(CFLAGS) $^ -o $(PROG) $(LIBS)

$(OBJDIR)/%.o : $(SRC_DIR)/%.c $(HDRS)    ← UPDATED PATH
	$(CC) -c $(CFLAGS) $< -o $@ $(LIBS)

$(OBJDIR) :
	mkdir -p $@/

clean:
	rm -rf object/
	rm -f $(PROG)

.PHONY: all clean