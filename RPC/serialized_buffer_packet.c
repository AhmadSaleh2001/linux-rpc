#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "serialized_buffer.h"
#include "serialized_buffer_packet.h"

serialized_buffer_packet_t *create_serialized_packet(serialized_buffer_t *buf) {
    // Allocate memory for the struct + actual data size
    serialized_buffer_packet_t *packet = malloc(sizeof(serialized_buffer_packet_t) + buf->size);
    if (!packet) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }

    // Fill the struct
    packet->rpc_header = buf->rpc_header;
    packet->size = buf->size;
    packet->next = buf->next;
    memcpy(packet->data, buf->b, buf->size); // Copy the actual buffer data

    return packet;
}


serialized_buffer_t deserialize_packet(char *recv_data) {
    // Cast the received data to the struct type
    serialized_buffer_packet_t *packet = (serialized_buffer_packet_t *)recv_data;

    // Create the deserialized structure
    serialized_buffer_t buf;
    buf.rpc_header = packet->rpc_header;
    buf.size = packet->size;
    buf.next = packet->next;
    buf.b = malloc(buf.size);
    if (!buf.b) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }

    memcpy(buf.b, packet->data, buf.size); // Copy the actual data into the buffer

    return buf;
}
