# Makefile for compiling lab2 projects into specific test executables

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -O2

# Source files and their corresponding executable names
SRC = lab2_0.c lab2_1.c lab2_3.c lab2_4.c
EXEC = test0 test1 test3 test4

# Default target
all: $(EXEC)

# Rule to compile each .c file to the corresponding executable
test0: lab2_0.c
	$(CC) $(CFLAGS) $< -o $@

test1: lab2_1.c
	$(CC) $(CFLAGS) $< -o $@

test3: lab2_3.c
	$(CC) $(CFLAGS) $< -o $@

test4: lab2_4.c
	$(CC) $(CFLAGS) $< -o $@

# Test target to run all executables
test: $(EXEC)
	@echo "Running tests..."
	@for exe in $(EXEC); do \
		echo "Testing $$exe:"; \
		./$$exe; \
		echo ""; \
	done

# Clean target to remove compiled files
clean:
	rm -f $(EXEC)

.PHONY: all clean test
