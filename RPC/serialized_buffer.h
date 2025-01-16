#pragma once

#define SENTINEL_CHECK_SERIALIZATION(b, obj) \
    if(!obj) {  \
        unsigned int senitel = 0xFFFFFFFF; \
        serialize_data(b, &senitel, sizeof(unsigned int)); \
        return; \
    } \

#define SENTINEL_CHECK_DESERIALIZATION(b) \
    unsigned int senitel = 0; \
    deserialize_data(b, &senitel, sizeof(unsigned int)); \
    if(senitel == 0xFFFFFFFF) { \
        return NULL; \
    } \
    serialize_buffer_skip(b, -sizeof(unsigned int)); \

#define SERIALIZED_BUFFER_DEFAULT_SIZE 100

typedef enum OPERATIONS {
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVISION
} OPERATIONS;

typedef struct rpc_header_ {
    OPERATIONS op;
} rpc_header_t;

typedef struct serialized_buffer {
    rpc_header_t rpc_header;
    char *b;
    int size;
    int next;
} serialized_buffer_t;

void init_serializd_buffer(serialized_buffer_t ** b, int size);
void serialize_data(serialized_buffer_t *b, char * data, int nbytes);
void deserialize_data(serialized_buffer_t *b, char * data, int size);
void serialize_buffer_skip(serialized_buffer_t *b, int skip_size);
void free_serialize_buffer(serialized_buffer_t *b);