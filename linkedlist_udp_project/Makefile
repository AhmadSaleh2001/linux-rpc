# Makefile to compile mx_udp_server.c, udp_client.c, and additional source files into .exe files

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -g

# Source files for each executable
SERVER_SRC = mx_udp_server.c
CLIENT_SRC = udp_client.c
COMMON_SRCS = department.c student.c serialized_buffer.c linkedlist/node.c linkedlist/linkedlist.c serialized_buffer_packet.c

# Object files for common sources
COMMON_OBJS = $(COMMON_SRCS:.c=.o)

# Output executables
SERVER_TARGET = mx_udp_server.exe
CLIENT_TARGET = udp_client.exe

# Default rule
all: $(SERVER_TARGET) $(CLIENT_TARGET)

# Rule to create the server executable
$(SERVER_TARGET): $(SERVER_SRC) $(COMMON_OBJS)
	$(CC) $(CFLAGS) -o $@ $< $(COMMON_OBJS)

# Rule to create the client executable
$(CLIENT_TARGET): $(CLIENT_SRC) $(COMMON_OBJS)
	$(CC) $(CFLAGS) -o $@ $< $(COMMON_OBJS)

# Rule to compile .c files into .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(SERVER_TARGET) $(CLIENT_TARGET) $(COMMON_OBJS)