#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "serialized_buffer.h"

void init_serializd_buffer(serialized_buffer_t ** b, int size) {
    (*b) = calloc(1, sizeof(serialized_buffer_t));
    if(size <= 0)size = SERIALIZED_BUFFER_DEFAULT_SIZE;
    (*b)->b = calloc(size, sizeof(char));
    (*b)->size = size;
    (*b)->next = 0;
}

void serialize_data(serialized_buffer_t *b, char * data, int nbytes) {
    int available_size = b->size - b->next;
    bool should_resize = false;
    while(available_size < nbytes) {
        b->size<<=1;
        available_size = b->size - b->next;
        should_resize = true;
    }

    if(should_resize) {
        b->b = realloc(b->b, b->size);
    }

    
    memcpy((char*)b->b + b->next, data, nbytes);
    b->next+=nbytes;
}

void deserialize_data(serialized_buffer_t *b, char * data, int size) {
    memcpy(data, (char*)b->b + b->next, size);
    b->next+=size;
}

void serialize_buffer_skip(serialized_buffer_t *b, int skip_size) {
    if(b->next + skip_size >= 0 && b->next + skip_size < b->size) {
        b->next+=skip_size;
    }
}

void free_serialize_buffer(serialized_buffer_t *b) {
    free(b->b);
    free(b);
}