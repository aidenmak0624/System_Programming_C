# Parallel Matrix-Vector Multiplication - Makefile

###############################################
# Project Configuration
###############################################

# Name of executable
PROG = parallel_matrix_mult

# Source directory
SRC_DIR = src

# Header files
HDRS =

# Source files
SRCS = $(SRC_DIR)/parallel_matrix_mult.c

#######################
# Build Configuration
#######################

# Object directory
OBJDIR = object

# Object files
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

# Compiler
CC = mpicc

# Compiler flags
CFLAGS = -Wall --std=c99 -L.

# Libraries
LIBS = -lm

####################
# Build Rules
####################

all : $(OBJDIR) $(PROG)

$(PROG) : $(OBJS)
	$(CC) $(CFLAGS) $^ -o $(PROG) $(LIBS)

$(OBJDIR)/%.o : $(SRC_DIR)/%.c $(HDRS)
	$(CC) -c $(CFLAGS) $< -o $@ $(LIBS)

$(OBJDIR) :
	mkdir -p $@/

clean:
	rm -rf $(OBJDIR)/
	rm -f $(PROG)

.PHONY: all clean