# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -O2

# Common source files
COMMON_SRCS = serialized_buffer.c serialized_buffer_packet.c

# Source files for multiply.exe
MULTIPLY_SRCS = $(COMMON_SRCS) multiply.c

# Source files for mx_udp_server.exe
MX_UDP_SERVER_SRCS = $(COMMON_SRCS) mx_udp_server.c

# Object files (replace .c with .o)
MULTIPLY_OBJS = $(MULTIPLY_SRCS:.c=.o)
MX_UDP_SERVER_OBJS = $(MX_UDP_SERVER_SRCS:.c=.o)

# Output executables
MULTIPLY_TARGET = multiply.exe
MX_UDP_SERVER_TARGET = mx_udp_server.exe

# Default target
all: $(MULTIPLY_TARGET) $(MX_UDP_SERVER_TARGET)

# Link object files to create multiply.exe
$(MULTIPLY_TARGET): $(MULTIPLY_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Link object files to create mx_udp_server.exe
$(MX_UDP_SERVER_TARGET): $(MX_UDP_SERVER_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile each .c file into a .o file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(MULTIPLY_OBJS) $(MX_UDP_SERVER_OBJS) $(MULTIPLY_TARGET) $(MX_UDP_SERVER_TARGET)

.PHONY: all clean
