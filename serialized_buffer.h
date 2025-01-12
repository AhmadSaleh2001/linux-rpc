#pragma once
#include "student.h"
#include "department.h"
#include "linkedlist/node.h"
#include "linkedlist/linkedlist.h"
#include "generic_linkedlist/generic_node.h"
#include "generic_linkedlist/generic_linkedlist.h"

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

typedef struct serialized_buffer {
    char *b;
    int size;
    int next;
} serialized_buffer_t;

void init_serializd_buffer(serialized_buffer_t ** b);
void serialize_data(serialized_buffer_t *b, char * data, int nbytes);
void deserialize_data(serialized_buffer_t *b, char * data, int size);
void serialize_buffer_skip(serialized_buffer_t *b, int skip_size);
void free_serialize_buffer(serialized_buffer_t *b);


void serialize_student(serialized_buffer_t *b, student_t * std);
student_t * dserialize_student(serialized_buffer_t *b);

void serialize_department(serialized_buffer_t *b, department_t * dep);
department_t * dserialize_department(serialized_buffer_t *b);

void serialize_node(serialized_buffer_t *b, node_t * node);
node_t * dserialize_node(serialized_buffer_t *b);

void serialize_linkedlist(serialized_buffer_t *b, linkedlist_t * linkedlist);
linkedlist_t * dserialize_linkedlist(serialized_buffer_t *b);

void generic_serialize_node(serialized_buffer_t *b, generic_node_t * node);
generic_node_t * generic_dserialize_node(serialized_buffer_t *b);