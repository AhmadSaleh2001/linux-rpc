#pragma once

#include "serialized_buffer.h"

typedef struct serialized_buffer_packet {
    rpc_header_t rpc_header;
    int size;    // Size of the actual buffer
    int next;    // Next pointer value (if applicable)
    char data[]; // Flexible array member for the actual buffer
} serialized_buffer_packet_t;

serialized_buffer_packet_t *create_serialized_packet(serialized_buffer_t *buf);
serialized_buffer_t deserialize_packet(char *recv_data);