# Makefile to compile main.c and serialized_buffer.c

# Compiler\CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -g

# Source files
SRCS = main.c department.c student.c serialized_buffer.c linkedlist/node.c linkedlist/linkedlist.c \
	generic_linkedlist/generic_node.c generic_linkedlist/generic_linkedlist.c

# Object files
OBJS = $(SRCS:.c=.o)

# Output executable
TARGET = main.exe

# Default rule
all: $(TARGET)

# Rule to create the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

# Rule to compile .c files into .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)
